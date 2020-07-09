#pragma once

#ifdef KEYBOARD_keebio_bdn9_rev1
#    define ENCODER_DIRECTION_FLIP
#    define BACKLIGHT_BREATHING
#endif

#define TAP_CODE_DELAY 20
#define TAPPING_TERM 300

// default to 5 dynamic keymap layers (VIA)
#define DYNAMIC_KEYMAP_LAYER_COUNT 5

// clear default RGB defines
#undef RGBLED_NUM
#undef RGBLIGHT_HUE_STEP
#undef RGBLIGHT_SAT_STEP
#undef RGBLIGHT_VAL_STEP
#undef RGBLIGHT_LIMIT_VAL
//#undef RGBLIGHT_ANIMATIONS

// RGB
#define DRIVER_LED_TOTAL 64
#define RGB_MATRIX_CENTER { 112, 112 }
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 175
