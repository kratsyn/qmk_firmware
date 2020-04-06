#pragma once

#ifdef KEYBOARD_keebio_bdn9_rev1
    #include "rev1.h"
#elif KEYBOARD_keebio_bdn9_rev2
    #include "rev2.h"
#ifdef KEYBOARD_keebio_bdn9_proton_c
    #include "proton_c.h"
#endif

#include "quantum.h"

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
    KA1, KA2, KA3, \
    KB1, KB2, KB3, \
    KC1, KC2, KC3 \
) \
{ \
    { KA1, KA2, KA3 }, \
    { KB1, KB2, KB3 }, \
    { KC1, KC2, KC3 } \
}
