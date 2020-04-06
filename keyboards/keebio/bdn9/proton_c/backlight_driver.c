/* Copyright 2019 kratsyn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <hal.h>
#include "debug.h"
#include "eeconfig.h"
#include "backlight.h"
#include "backlight_driver.h"

#ifndef BACKLIGHT_BREATHING_STEPS
#   define BACKLIGHT_BREATHING_STEPS    128
#endif

extern backlight_config_t backlight_config;

static uint32_t backlight_duty = 0;
static uint16_t breath_intval  = (uint16_t)BREATHING_PERIOD * 256 / BACKLIGHT_BREATHING_STEPS;
static uint32_t breath_duty    = 0;
static uint8_t  breath_index   = 0;
static uint16_t breath_count   = 0;

static const PWMConfig pwmCfg = {
    .frequency = 0xFFFF,
    .period    = 256,
    .callback  = backlight_callback,  // register our callback as a part of the pwm cycle
    .channels =
        {
            [0 ... 3]               = {.mode = PWM_OUTPUT_DISABLED, .callback = NULL},
            [BACKLIGHT_PWM_CHANNEL] = {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL},
        },
    .cr2  = 0,
    .dier = 0,
};

static const uint8_t breathing_table[BACKLIGHT_BREATHING_STEPS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 17, 20, 24, 28, 32, 36, 41, 46, 51, 57, 63, 70, 76, 83, 91, 98, 106, 113, 121, 129, 138, 146, 154, 162, 170, 178, 185, 193, 200, 207, 213, 220, 225, 231, 235, 240, 244, 247, 250, 252, 253, 254, 255, 254, 253, 252, 250, 247, 244, 240, 235, 231, 225, 220, 213, 207, 200, 193, 185, 178, 170, 162, 154, 146, 138, 129, 121, 113, 106, 98, 91, 83, 76, 70, 63, 57, 51, 46, 41, 36, 32, 28, 24, 20, 17, 15, 12, 10, 8, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// See http://jared.geek.nz/2013/feb/linear-led-pwm
static uint16_t cie_lightness(uint16_t v) {
    if (v <= 5243)     // if below 8% of max
        return v / 9;  // same as dividing by 900%
    else {
        uint32_t y = (((uint32_t)v + 10486) << 8) / (10486 + 0xFFFFUL);  // add 16% of max and compare
        // to get a useful result with integer division, we shift left in the expression above
        // and revert what we've done again after squaring.
        y = y * y * y >> 8;
        if (y > 0xFFFFUL)  // prevent overflow
            return 0xFFFFU;
        else
            return (uint16_t)y;
    }
}

void backlight_init_ports(void) {
#   ifndef USE_GPIOV1
    palSetLineMode(BACKLIGHT_PIN, PAL_MODE_ALTERNATE(BACKLIGHT_PAL_MODE));
#   else
    palSetLineMode(BACKLIGHT_PIN, PAL_MODE_STM32_ALTERNATE_PUSHPULL);
#   endif

    pwmStart(&BACKLIGHT_PWM_DRIVER, &pwmCfg);
    backlight_reset_callback();
    dprint("[BL] Startup complete.\n");
}

void backlight_reset_callback(void) {
    set_callback(false);
    set_callback(true);
}

void backlight_stop(void) {
    pwmDisableChannel(&BACKLIGHT_PWM_DRIVER, BACKLIGHT_PWM_CHANNEL);
    set_callback(false);
}

void backlight_set(uint8_t level) {
    backlight_duty = (uint32_t)(cie_lightness(0xFFFF * (uint32_t)backlight_config.level / BACKLIGHT_LEVELS));

    if (backlight_config.level == BACKLIGHT_LEVELS + 1) {
        backlight_config.level = 0;
    } else if (level > BACKLIGHT_LEVELS) {
        backlight_config.level = BACKLIGHT_LEVELS;
    }

    if (level == 0) {
        backlight_stop();
    } else if (!is_backlight_breathing()) {
        pwmEnableChannel(&BACKLIGHT_PWM_DRIVER, BACKLIGHT_PWM_CHANNEL, PWM_FRACTION_TO_WIDTH(&BACKLIGHT_PWM_DRIVER, 0xFFFF, backlight_duty));
    } else {
        backlight_reset_callback();
    }
    dprintf("[BL] Backlight level set. (L:%d) (D:%d)\n", backlight_config.level, backlight_duty);
}

void breathing_enable() { backlight_reset_callback(); }

void breathing_disable() {
    backlight_reset_callback();
    backlight_set(backlight_config.level);
}

static inline uint16_t backlight_scale(uint16_t value) { return (value / BACKLIGHT_LEVELS * backlight_config.level); }

static void backlight_callback(PWMDriver *pwmp) {
    // be wary, long operations will impact performance

    // breathing logic
    if (is_backlight_breathing()) {
        breath_count = (breath_count + 1) % (BREATHING_PERIOD * 256);
        breath_index = breath_count / breath_intval % BACKLIGHT_BREATHING_STEPS;
        breath_duty  = cie_lightness(backlight_scale(breathing_table[breath_index] * 256));

        // lock, set new val, unlock
        chSysLockFromISR();
        pwmEnableChannelI(&BACKLIGHT_PWM_DRIVER, BACKLIGHT_PWM_CHANNEL, PWM_FRACTION_TO_WIDTH(&BACKLIGHT_PWM_DRIVER, 0xFFFF, breath_duty));
        chSysUnlockFromISR();
    }

    // others?
}

// track down where this is actually used. at first glance, our callback seems to be a duplicate.
// 
void backlight_task(void) {}

void set_callback(bool on) {
    chSysLockFromISR();
    if (on) {
        pwmEnablePeriodicNotificationI(&BACKLIGHT_PWM_DRIVER);
    } else {
        pwmDisablePeriodicNotificationI(&BACKLIGHT_PWM_DRIVER);
    }
    chSysUnlockFromISR();
}
