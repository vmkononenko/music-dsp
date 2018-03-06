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

typedef double amplitude_t;
typedef double freq_hz_t;
typedef std::complex<amplitude_t> complex_t;

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
    OCTAVE_MAX = OCTAVE_8,
    OCTAVES_CNT = OCTAVE_MAX - OCTAVE_MIN + 1
} octave_t;

typedef enum {
    cq_major,
    cq_minor,
    cq_7th,
    cq_major_7th,
    cq_minor_7th,
    cq_6th,
    cq_minor_6th,
    cq_diminished,
    cq_diminished_7th,
    cq_half_diminished_7th,
    cq_9th,
    cq_major_9th,
    cq_added_9th,
    cq_minor_9th,
    cq_suspended_4th,
    cq_suspended_2nd,
    cq_5th,
    cq_Min = cq_major,
    cq_Max = cq_5th
} chord_quality_t;

typedef enum {
    WINDOW_FUNC_RECTANGULAR,
    WINDOW_FUNC_BLACKMAN,
    WINDOW_FUNC_HAMMING,
    WINDOW_FUNC_HANN,
    WINDOW_FUNC_MIN = WINDOW_FUNC_RECTANGULAR,
    WINDOW_FUNC_MAX = WINDOW_FUNC_HANN
} window_func_t;

typedef struct Chord {

private:
    note_t          __mRootNote;
    chord_quality_t __mQuality;

public:
    Chord(note_t n, chord_quality_t q) : __mRootNote(n), __mQuality(q) {}

    friend bool operator==(const Chord& c1, const Chord& c2)
    {
        return ((c1.__mRootNote == c2.__mRootNote) && (c1.__mQuality == c2.__mQuality));
    }

    friend bool operator!=(const Chord& c1, const Chord& c2)
    {
        return !(c1 == c2);
    }

    friend std::ostream& operator<<(std::ostream& os, const Chord& c)
    {
        os << noteToString(c.__mRootNote) << qualityToString(c.__mQuality);
        return os;
    }

    /**
     * Return string representation of the given note
     *
     * @param   note    note for which string representation is requested
     * @return string representation of the note
     */
    static std::string noteToString(note_t note) {
        if (((note < note_Min) && (note > note_Max)) && (note != note_Unknown)) {
            throw std::invalid_argument("Invalid note");
        }

        std::map<note_t, std::string> n2sMap;

        n2sMap[note_C]          = "C";
        n2sMap[note_C_sharp]    = "C#";
        n2sMap[note_D]          = "D";
        n2sMap[note_D_sharp]    = "D#";
        n2sMap[note_E]          = "E";
        n2sMap[note_F]          = "F";
        n2sMap[note_F_sharp]    = "F#";
        n2sMap[note_G]          = "G";
        n2sMap[note_G_sharp]    = "G#";
        n2sMap[note_A]          = "A";
        n2sMap[note_A_sharp]    = "A#";
        n2sMap[note_B]          = "B";
        n2sMap[note_Unknown]    = "??";

        return std::string(n2sMap[note]);
    }

    static std::string qualityToString(chord_quality_t quality) {
        if ((quality < cq_Min) && (quality > cq_Max)) {
            throw std::invalid_argument("Invalid quality");
        }

        std::map<chord_quality_t, std::string> q2sMap;

        q2sMap[cq_major]                = "";
        q2sMap[cq_minor]                = "m";
        q2sMap[cq_7th]                  = "7";
        q2sMap[cq_major_7th]            = "maj7";
        q2sMap[cq_minor_7th]            = "m7";
        q2sMap[cq_6th]                  = "6";
        q2sMap[cq_minor_6th]            = "m6";
        q2sMap[cq_diminished]           = "dim";
        q2sMap[cq_diminished_7th]       = "dim7";
        q2sMap[cq_half_diminished_7th]  = "m7b5";
        q2sMap[cq_9th]                  = "9";
        q2sMap[cq_major_9th]            = "maj9";
        q2sMap[cq_added_9th]            = "add9";
        q2sMap[cq_minor_9th]            = "m9";
        q2sMap[cq_suspended_4th]        = "sus4";
        q2sMap[cq_suspended_2nd]        = "sus2";
        q2sMap[cq_5th]                  = "5";

        return std::string(q2sMap[quality]);
    }
} chord_t;

/** @} */
