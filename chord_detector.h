/*
 * chord_detector.h
 *
 *  Chord recognition API
 */

#pragma once

#include <stdint.h>
#include <vector>

#include "fft.h"
#include "lmtypes.h"
#include "pitch_detector.h"

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif


typedef struct Chord {
    note_t      mainNote;
    octave_t    octave;
    bool        isMinor;

    Chord() : mainNote(note_Unknown) {}
} chord_t;


class ChordDetector {

TEST_FRIENDS;

private:
    FFT *__mFft;
    PitchDetector *__mPitchDetector;

    /**
     *  Cuts off unwanted frequency range from the input frequency domain
     *
     *  @param x            input frequency domain
     *  @param sampleRate   sample rate of x
     *  @param freq         frequency to cut from
     *  @param cutHigh      cut range higher than freq if set to true
     *                      fill range lower than freq to 0 (but not cut the
     *                      range off from vector to be able to calculate Hz
     *                      from index in x)
     */
    void __cutoffFreq(std::vector<complex_t> &, uint32_t, amplitude_t, bool);

public:
    ChordDetector();

    /**
     * Detect chord corresponding to the given time domain
     *
     * @param   x           time domain data
     * @param   samples     number of samples in x
     * @param   sampleRate  sample rate of x
     * @return  detected chord
     */
    chord_t getChord(amplitude_t *, uint32_t, uint32_t);
};
