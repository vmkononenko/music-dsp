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
    note_D,
    note_E,
    note_F,
    note_G,
    note_A,
    note_B,
    note_Unknown
} note_t;
