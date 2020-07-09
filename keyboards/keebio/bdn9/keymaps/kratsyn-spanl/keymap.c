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

int  cur_dance(qk_tap_dance_state_t *state);
void ql_finished(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *user_date);

// clang-format off
led_config_t g_led_config = {
	{
		{ NO_LED, NO_LED, NO_LED },
		{ NO_LED, NO_LED, NO_LED },
		{ NO_LED, NO_LED, NO_LED }
	}, {
		{ 224, 0   }, { 192, 0   }, { 160, 0   }, { 128, 0   }, { 96 , 0   }, { 64 , 0   }, { 32 , 0   }, { 0  , 0   },
		{ 0  , 32  }, { 32 , 32  }, { 64 , 32  }, { 96 , 32  }, { 128, 32  }, { 160, 32  }, { 192, 32  }, { 224, 32  },
		{ 224, 64  }, { 192, 64  }, { 160, 64  }, { 128, 64  }, { 96 , 64  }, { 64 , 64  }, { 32 , 64  }, { 0  , 64  },
		{ 0  , 96  }, { 32 , 96  }, { 64 , 96  }, { 96 , 96  }, { 128, 96  }, { 160, 96  }, { 192, 96  }, { 224, 96  },
		{ 224, 128 }, { 192, 128 }, { 160, 128 }, { 128, 128 }, { 96 , 128 }, { 64 , 128 }, { 32 , 128 }, { 0  , 128 },
		{ 0  , 160 }, { 32 , 160 }, { 64 , 160 }, { 96 , 160 }, { 128, 160 }, { 160, 160 }, { 192, 160 }, { 224, 160 },
		{ 224, 192 }, { 192, 192 }, { 160, 192 }, { 128, 192 }, { 96 , 192 }, { 64 , 192 }, { 32 , 192 }, { 0  , 192 },
		{ 0  , 224 }, { 32 , 224 }, { 64 , 224 }, { 96 , 224 }, { 128, 224 }, { 160, 224 }, { 192, 224 }, { 224, 224 }
	}, {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	}
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // User 0
    [0] = LAYOUT(
        KC_MPLY, TD(TD_LAYR), KC_MUTE,
        KC_PGUP, KC_UP, KC_PGDOWN,
        KC_LEFT, KC_DOWN, KC_RGHT
    ),
	// User 1
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
        RGB_TOG, _______, _______, 
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

void keyboard_post_init_user(void) { 
}

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
            break;
        case DOUBLE_TAP:
            layer_on(1);
            break;
        case TRIPLE_TAP:
            layer_on(2);
            break;
        case QUAD_TAP:
            layer_on(3);
            break;
        case QUIN_TAP:
            layer_on(4);
            break;
    }
}

void                  ql_reset(qk_tap_dance_state_t *state, void *user_data) { ql_tap_state.state = 0; }
qk_tap_dance_action_t tap_dance_actions[] = {[TD_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset)};
