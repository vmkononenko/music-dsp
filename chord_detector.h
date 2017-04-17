/*
 * chord_detector.h
 *
 *  Chord recognition API
 */

#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>

#include "chord_tpl_collection.h"
#include "fft.h"
#include "lmhelpers.h"
#include "lmtypes.h"
#include "music_scale.h"
#include "pitch_calculator.h"

#ifndef CHORD_DETECTOR_TEST_FRIENDS
#define CHORD_DETECTOR_TEST_FRIENDS
#endif


class ChordDetector {

CHORD_DETECTOR_TEST_FRIENDS;

private:
    FFT *__mFft;
    PitchCalculator& __mPitchCalculator = PitchCalculator::getInstance();
    ChordTplCollection *__mChordTplColl = new ChordTplCollection();

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
     * Implementation of the main chord detection algorithm
     *
     * @param   freqDomainMagnitudes    polar magnites from FFT results
     * @param   fftSize                 size of the FFT
     * @param   sampleRate              sample rate
     * @param   pointsCnt               freqDomainMagnitudes length
     * @return detected chord
     */
    chord_t __getChordFromFftResults(amplitude_t *freqDomainMagnitudes,
            uint32_t fftSize, uint32_t sampleRate, uint32_t pointsCnt);

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
