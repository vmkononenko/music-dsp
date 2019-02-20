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
    cq_major,
    cq_minor,
    cq_7th,
    cq_major_7th,
    cq_minor_7th,
    cq_6th,
    cq_minor_6th,
    cq_diminished,
    cq_half_diminished_7th,
    cq_9th,
    cq_major_9th,
    cq_added_9th,
    cq_minor_9th,
    cq_suspended_4th,
    cq_suspended_2nd,
    cq_diminished_7th,
    cq_5th,
    cq_Min = cq_major,
    cq_Max = cq_minor
} chord_quality_t;

std::ostream& operator<<(std::ostream& os, const chord_quality_t& q);

typedef struct Chord {

private:
    note_t          __mRootNote;
    chord_quality_t __mQuality;

public:
    Chord(note_t n, chord_quality_t q) : __mRootNote(n), __mQuality(q) {}
    Chord() : Chord(note_Unknown, cq_unknown) {} // Delegate to the other constructor.

    friend bool operator==(const Chord &c1, const Chord &c2)
    {
        return ((c1.__mRootNote == c2.__mRootNote) && (c1.__mQuality == c2.__mQuality));
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

    friend std::ostream& operator<<(std::ostream& os, const Chord &c)
    {
        os << c.__mRootNote << c.__mQuality;
        return os;
    }

    std::string toString()
    {
        std::ostringstream ss;
        ss << __mRootNote << __mQuality;
        return ss.str();
    }
} chord_t;

/** @} */
