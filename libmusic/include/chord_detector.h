/**
 * @file        chord_detector.h
 * @brief       Chord recognition API
 * @addtogroup  libmusic
 * @{
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
#include "pcp_buf.h"
#include "pitch_calculator.h"
#include "pitch_cls_profile.h"

#ifndef CHORD_DETECTOR_TEST_FRIENDS
#define CHORD_DETECTOR_TEST_FRIENDS
#endif

struct segment_t {
    uint32_t    startIdx;
    uint32_t    endIdx;
    chord_t     chord;
    bool        silence;
};

/**
 * @class   ChordDetector
 */
class ChordDetector {

CHORD_DETECTOR_TEST_FRIENDS;

private:
    struct FFTResults {
        amplitude_t *freqDomain;
        uint32_t    fftSize;
        uint32_t    sampleRate;
        uint32_t    highFreqThresholdIdx;
    };

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

    FFTResults __getFftResults(amplitude_t *x, uint32_t samples, uint32_t sampleRate);

    /**
     * Implementation of the main chord detection algorithm
     *
     * @param   fftRes  reference to FFTResults
     * @return detected chord
     */
    chord_t __getChordFromFftResults(FFTResults& fftRes);

    pcp_t * __FFT2PCP(FFTResults& fftRes);

    void processSegment(std::vector<segment_t>& segments, uint32_t startIdx,
                        uint32_t endIdx, bool silence, PCPBuf *pcpBuf);

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
    * Given a full set of time domain data, it fills up the 'segments'
    * vector with the detected sequence of chord segments
    *
    * @param   segments    output vector of segments
    * @param   x           full time domain data
    * @param   samples     number of samples in x
    * @param   sampleRate  sample rate of x
    */
    void getSegments(std::vector<segment_t>& segments, amplitude_t *x,
                     uint32_t samples, uint32_t sampleRate);

    /**
     * Build major or minor scale from the main note
     *
     * @param   mainNote    main note to build the scale from
     * @param   isMinor     specifies major or minor scale
     * @return  requested scale
     */
    std::vector<note_t> getScale(note_t mainNote, bool isMinor);

    PitchClsProfile getPCP(amplitude_t *x, uint32_t samples, uint32_t sampleRate);
};

/** @} */
