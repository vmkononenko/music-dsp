/*
 * Copyright 2019 Volodymyr Kononenko
 *
 * This file is part of Music-DSP.
 *
 * Music-DSP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Music-DSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Music-DSP. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file        lmtypes.h
 * @brief       Defines types specific for libmusic
 * @defgroup    libmusic
 * Library documentation
 * @{
 */

#pragma once

#include <complex>
#include <map>
#include <stdint.h>
#include <vector>

#include "config.h"

#define WINDOW_FUNC_RECTANGULAR     1
#define WINDOW_FUNC_BLACKMAN        2
#define WINDOW_FUNC_HAMMING         3
#define WINDOW_FUNC_HANN            4
#define WINDOW_FUNC_MIN             WINDOW_FUNC_RECTANGULAR
#define WINDOW_FUNC_MAX             WINDOW_FUNC_HANN

#define TFT_TYPE_FFT        1
#define TFT_TYPE_CONSTANTQ  2

typedef double amplitude_t;
typedef double freq_hz_t;
typedef double prob_t;
typedef std::complex<amplitude_t> complex_t;
typedef std::vector<amplitude_t> td_t;  /* time domain      */
typedef std::vector<amplitude_t> fd_t;  /* frequency domain */

typedef enum {
    note_Unknown = 0,   // all note_t variables will be initialized as "unknown" by default
    note_C,
    note_C_sharp,
    note_D,
    note_D_sharp,
    note_E,
    note_F,
    note_F_sharp,
    note_G,
    note_G_sharp,
    note_A,
    note_A_sharp,
    note_B,
    note_Min = note_C,
    note_Max = note_B,
    notes_Total = note_Max - note_Min + 1
} note_t;

note_t& operator+(note_t& note, int32_t term);
std::ostream& operator<<(std::ostream& os, const note_t& n);

typedef enum : int32_t {
    OCTAVE_0,
    OCTAVE_1,
    OCTAVE_2,
    OCTAVE_3,
    OCTAVE_4,
    OCTAVE_5,
    OCTAVE_6,
    OCTAVE_7,
    OCTAVE_8,
    OCTAVE_9,
    OCTAVE_MIN = OCTAVE_2,
    OCTAVE_MAX = OCTAVE_7,
    OCTAVES_CNT = OCTAVE_MAX - OCTAVE_MIN + 1
} octave_t;

typedef enum {
    cq_unknown = 0,
    cq_maj,
    cq_min,
    cq_5,
    cq_7,
    cq_maj7,
    cq_min7,
    cq_sus2,
    cq_sus4,
    cq_hdim7,
    cq_aug,
    cq_dim,
    cq_dim7,  /* Minimum recommended value for cq_Max */
    cq_maj_add9,
    cq_min_add9,
    cq_maj6,
    cq_min6,
    cq_maj9,
    cq_min9,
    cq_maj_add11,
    cq_7_add9sharp,
    cq_9,
    cq_aug7,
    cq_maj11,
    cq_min11,
    cq_maj13,
    cq_min13,
    cq_Min = cq_maj,
    cq_Max = cq_min13
} chord_quality_t;

std::ostream& operator<<(std::ostream& os, const chord_quality_t& q);

typedef struct Chord {

private:
    note_t          __mRootNote;
    note_t          __mBassNote;
    int8_t          __mBassInterval;
    chord_quality_t __mQuality;

public:
    Chord(note_t n, chord_quality_t q, note_t b = note_Unknown, int8_t bi = -1) :
        __mRootNote(n), __mBassNote(b),  __mBassInterval(bi), __mQuality(q) {}
    Chord() : Chord(note_Unknown, cq_unknown) {} // Delegate to the other constructor.

    friend bool operator==(const Chord &c1, const Chord &c2)
    {
        return ((c1.__mRootNote == c2.__mRootNote)  &&
                (c1.__mQuality == c2.__mQuality)    &&
                (c1.__mBassNote == c2.__mBassNote) &&
                (c1.__mBassInterval == c2.__mBassInterval));
    }

    friend bool operator!=(const Chord &c1, const Chord &c2)
    {
        return !(c1 == c2);
    }

    friend bool operator<(const Chord &c1, const Chord &c2)
    {
        if (c1 == c2) {
            return false;
        }

        return (((c1.__mRootNote != c2.__mRootNote) && (c1.__mRootNote < c2.__mRootNote)) ||
                ((c1.__mRootNote == c2.__mRootNote) && (c1.__mQuality < c2.__mQuality)));
    }

    std::string toHarte() const
    {
        std::ostringstream ss;

        ss << __mRootNote;
        if (__mRootNote != note_Unknown) {
            ss << ":" << __mQuality;
        }
        if (__mBassInterval > 1) {
            ss << "/" << (int)__mBassInterval;
        }

        return ss.str();
    }

    std::string toUser() const
    {
        std::ostringstream ss;

        ss << __mRootNote << __mQuality;
        if (__mBassInterval > 1) {
            ss << "/" << __mBassNote;
        }

        return ss.str();
    }

    std::string toString() const
    {
#if defined(CFG_HARTE_SYNTAX) && CFG_HARTE_SYNTAX == 1
        return toHarte();
#else
        return toUser();
#endif /* CFG_HARTE_SYNTAX */
    }

    friend std::ostream& operator<<(std::ostream& os, const Chord &c)
    {
        os << c.toString();
        return os;
    }

} chord_t;

/** @} */
