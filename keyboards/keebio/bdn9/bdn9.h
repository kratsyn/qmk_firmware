#pragma once

#ifdef KEYBOARD_keebio_bdn9_rev1
    #include "rev1.h"
#elif KEYBOARD_keebio_bdn9_rev2
    #include "rev2.h"
#ifdef KEYBOARD_keebio_bdn9_proton_c
    #include "proton_c.h"
#endif
