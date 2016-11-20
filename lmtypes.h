/*
 * lmtypes.h
 *
 *  Defines types specific for libmusic
 */

#pragma once

#include <complex>
#include <stdint.h>

typedef double amplitude_t;
typedef double freq_hz_t;
typedef std::complex<amplitude_t> complex_t;
typedef uint8_t octave_t;

typedef enum {
    note_C,
    note_C_sharp,
    note_D,
    note_D_sharp,
    note_E,
    note_E_sharp,
    note_F,
    note_F_sharp,
    note_G,
    note_G_sharp,
    note_A,
    note_A_sharp,
    note_B,
    note_B_sharp,
    note_Unknown
} note_t;
