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
#include QMK_KEYBOARD_H

typedef struct {
    bool is_press_action;
    int  state;
} tap;

enum { SINGLE_TAP = 1, DOUBLE_TAP = 2, TRIPLE_TAP = 3, QUAD_TAP = 4, QUIN_TAP = 5 };
enum { TD_LAYR = 0 };

const rgblight_segment_t PROGMEM        default_layer[]   = RGBLIGHT_LAYER_SEGMENTS({0, 4, HSV_PURPLE});
const rgblight_segment_t PROGMEM        macro_layer[]     = RGBLIGHT_LAYER_SEGMENTS({0, 1, HSV_PURPLE});
const rgblight_segment_t PROGMEM        backlight_layer[] = RGBLIGHT_LAYER_SEGMENTS({1, 1, HSV_PURPLE});
const rgblight_segment_t PROGMEM        rgb_layer[]       = RGBLIGHT_LAYER_SEGMENTS({2, 1, HSV_PURPLE});
const rgblight_segment_t PROGMEM        quantum_layer[]   = RGBLIGHT_LAYER_SEGMENTS({3, 1, HSV_RED});
const rgblight_segment_t *const PROGMEM lighting_layers[] = RGBLIGHT_LAYERS_LIST(default_layer, macro_layer, backlight_layer, rgb_layer, quantum_layer);

int  cur_dance(qk_tap_dance_state_t *state);
void ql_finished(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *user_date);

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // User 0 (VIA MAPPED)
    [0] = LAYOUT(
        KC_MPLY, TD(TD_LAYR), KC_MUTE,
        KC_PGUP, KC_UP, KC_PGDOWN,
        KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT(
        _______, _______, _______,
        _______, _______, _______,
        _______, _______, _______
    ),
    // LED Backlight
    [2] = LAYOUT(
        BL_TOGG, _______, BL_BRTG,
        BL_STEP, BL_INC, KC_NO,
        BL_ON, BL_DEC, BL_OFF
    ),
    // RGB Underglow
    [3] = LAYOUT(
        RGB_TOG, _______, RGB_MOD, 
        RGB_SPI, RGB_VAI, RGB_SPD,
        RGB_MOD, RGB_VAD, RGB_RMOD
    ),
    // Quantum
    [4] = LAYOUT(
        KC_NO, _______, EEP_RST,
        KC_NO, RESET, KC_NO,
        KC_NO, DEBUG, KC_NO
    )
};
// clang-format on

void encoder_update_user(uint8_t index, bool clockwise) {
    uint8_t layer = biton32(layer_state);

    // Encoder 0
    if (index == 0) {
        switch (layer) {
            // mouse wheel as default
            default:
                tap_code((clockwise) ? KC_WH_D : KC_WH_U);
                break;
            // F13 - F14
            case 1:
                tap_code((clockwise) ? KC_F13 : KC_F14);
                break;
            // backlight on layer 2
            case 2:
                (clockwise) ? backlight_increase() : backlight_decrease();
                break;
            // rgb hue on layer 3
            case 3:
                (clockwise) ? rgblight_increase_hue() : rgblight_decrease_hue();
                break;
        }
        // Encoder 1
    } else if (index == 1) {
        switch (layer) {
            // volume as default
            default:
                tap_code((clockwise) ? KC_VOLU : KC_VOLD);
                break;
            // F15 - F16
            case 1:
                tap_code((clockwise) ? KC_F15 : KC_F16);
                break;
            // rgb saturation on layer 3
            case 3:
                (clockwise) ? rgblight_increase_sat() : rgblight_decrease_sat();
                break;
        }
    }
}

void eeconfig_init_user(void) {  // EEPROM Reset
    dprint("[SYS] EEPROM Reset to default values.\n");
}

void keyboard_post_init_user(void) { rgblight_layers = lighting_layers; }

int cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        return SINGLE_TAP;
    }
    if (state->count == 2) {
        return DOUBLE_TAP;
    }
    if (state->count == 3) {
        return TRIPLE_TAP;
    }
    if (state->count == 4) {
        return QUAD_TAP;
    }
    if (state->count == 5) {
        return QUIN_TAP;
    } else
        return 8;
}

static tap ql_tap_state = {.is_press_action = true, .state = 0};

void ql_finished(qk_tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case SINGLE_TAP:
            layer_move(0);
            rgblight_blink_layer(default_layer, 1000);
            break;
        case DOUBLE_TAP:
            layer_on(1);
            rgblight_blink_layer(macro_layer, 1000);
            break;
        case TRIPLE_TAP:
            layer_on(2);
            rgblight_blink_layer(backlight_layer, 1000);
            break;
        case QUAD_TAP:
            layer_on(3);
            rgblight_blink_layer(rgb_layer, 1000);
            break;
        case QUIN_TAP:
            layer_on(4);
            rgblight_blink_layer(quantum_layer, 1000);
            break;
    }
}

void                  ql_reset(qk_tap_dance_state_t *state, void *user_data) { ql_tap_state.state = 0; }
qk_tap_dance_action_t tap_dance_actions[] = {[TD_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset)};
