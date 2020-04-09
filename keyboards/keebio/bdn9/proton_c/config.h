/*
Copyright 2019 kratsyn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xCB10
#define PRODUCT_ID      0x1133
#define DEVICE_VER      0x0101
#define MANUFACTURER    Keebio
#define PRODUCT         BDN9
#define DESCRIPTION     3x3 Macropad with Rotary Encoders

/* key matrix size */
#define MATRIX_ROWS 3
#define MATRIX_COLS 3

#define DIRECT_PINS         \
    {                       \
        { A10, B5, A2 },    \
        { B3, B13, B14 },   \
        { B2, B1, B15 }     \
    }

#define LOCKING_SUPPORT_ENABLE
#define LOCKING_RESYNC_ENABLE

#define ENCODERS_PAD_A { B6, A0 }
#define ENCODERS_PAD_B { B7, A1 }
#define ENCODER_RESOLUTION 4

#define BACKLIGHT_PIN B0
#define BACKLIGHT_PAL_MODE 2
#define BACKLIGHT_PWM_DRIVER PWMD3  // TIM3
#define BACKLIGHT_PWM_CHANNEL 2     // TIM3_2

#define BACKLIGHT_LEVELS 5
#define BACKLIGHT_BREATHING
#define BREATHING_PERIOD 6

#define WS2812_PWM_DRIVER PWMD2
#define WS2812_PWM_CHANNEL 2
#define WS2812_PWM_PAL_MODE 10
#define WS2812_DMA_STREAM STM32_DMA1_STREAM2
#define WS2812_DMA_CHANNEL 2

#define RGB_DI_PIN A9
#ifdef RGB_DI_PIN
#   define RGBLED_NUM 4
#   define RGBLIGHT_HUE_STEP 8
#   define RGBLIGHT_SAT_STEP 8
#   define RGBLIGHT_VAL_STEP 8
#   define RGBLIGHT_LIMIT_VAL 255 /* The maximum brightness level */
#   define RGBLIGHT_SLEEP         /* If defined, the RGB lighting will be switched off when the host goes to sleep */
#   define RGBLIGHT_EFFECT_ALTERNATING
#   define RGBLIGHT_EFFECT_BREATHING
#   define RGBLIGHT_EFFECT_KNIGHT
#   define RGBLIGHT_EFFECT_RAINBOW_MOOD
#   define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#   define RGBLIGHT_EFFECT_SNAKE
#endif
