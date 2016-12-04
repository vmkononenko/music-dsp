/*
 * chord_detector.h
 *
 *  Chord recognition API
 */

#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>

#include "fft.h"
#include "lmhelpers.h"
#include "lmtypes.h"
#include "music_scale.h"
#include "pitch_calculator.h"

#ifndef CHORD_DETECTOR_TEST_FRIENDS
#define CHORD_DETECTOR_TEST_FRIENDS
#endif


typedef struct Chord {
    note_t      mainNote;
    bool        isMinor;
    octave_t    octave;

    Chord() : mainNote(note_Unknown), isMinor(false) {}

    friend bool operator==(const Chord& c1, const Chord& c2)
    {
        return ((c1.mainNote == c2.mainNote) &&
                (c1.isMinor == c2.isMinor));
    }

    friend bool operator!=(const Chord& c1, const Chord& c2)
    {
        return !(c1 == c2);
    }

    friend std::ostream& operator<<(std::ostream& os, const Chord& c)
    {
        os << Helpers::noteToString(c.mainNote) << (c.isMinor ? "m" : "");
        return os;
    }
} chord_t;


class ChordDetector {

CHORD_DETECTOR_TEST_FRIENDS;

private:
    FFT *__mFft;
    PitchCalculator *__mPitchCalculator;
    std::vector<MusicScale *> __mScales;

    /**
     *  Attenuate frequencies lower than freq to 0
     *
     *  Attenuate instead of cutting of the range to be able to do conversion
     *  between frequencies and indexes of FFT points
     *
     *  @param freqDomain   input frequency domain
     *  @param len          number of points in freqDomain
     *  @param freq         frequency to cut from
     *  @param fftSize      taken FFT length
     *  @param sampleRate   sample rate of the analyzed signal
     */
    void __attLowFreq(amplitude_t *, uint32_t, freq_hz_t, uint32_t, uint32_t);

    /**
     * Initialize __mScales
     */
    void __initScales();

    /**
     * Implementation of the main chord detection algorithm
     *
     * @param   fftPQ       FFT results in a form of a priority queue
     * @param   fftSize     size of the FFT
     * @param   sampleRate  sample rate
     * @param   lowFreqThresholdIdx do not analyze points below this index
     * @return detected chord
     */
    chord_t __getChordFromFftResults(PriorityQueue *fftPQ, uint32_t fftSize,
                            uint32_t sampleRate, uint32_t lowFreqThresholdIdx);

    /**
     * Check if scales defined by scalesIndexes contain note
     *
     * @param note  note to be checked
     * @return true if note is present in at least one scale
     */
    bool __isNotePresentInScales(std::vector<uint8_t> &scalesIndexes, note_t note);

public:
    /**
     * Constructor
     */
    ChordDetector();

    /**
     * Destructor
     */
    ~ChordDetector();

    /**
     * Detect chord corresponding to the given time domain
     *
     * @param   x           time domain data
     * @param   samples     number of samples in x
     * @param   sampleRate  sample rate of x
     * @return  detected chord
     */
    chord_t getChord(amplitude_t *x, uint32_t samples, uint32_t sampleRate);

    /**
     * Build major or minor scale from the main note
     *
     * @param   mainNote    main note to build the scale from
     * @param   isMinor     specifies major or minor scale
     * @return  requested scale
     */
    std::vector<note_t> getScale(note_t mainNote, bool isMinor);
};
