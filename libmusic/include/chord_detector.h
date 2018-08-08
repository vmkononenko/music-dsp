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

public:
    /**
     * Client listener to track analysis progress
     */
    class ResultsListener {
        public:
            /**
             * Called to report a preprocessing progress
             *
             * Anything prior to actual chord recognition loop is considered
             * preprocessing, e.g envelope and beat detection etc.
             */
            virtual void onPreprocessingProgress(float) = 0;

            /**
             * Send recognized chord segment with progress value to the client
             */
            virtual void onChordSegmentProcessed(segment_t &, float) = 0;

            /**
             * Called after all processing has been finished
             */
            virtual void onChordAnalysisFinished() = 0;
};

private:
    struct FFTResults {
        amplitude_t *freqDomain;
        uint32_t    fftSize;
        uint32_t    sampleRate;
        uint32_t    freqDomainLen;
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
     *  @param fftRes   reference to FFTResults
     *  @param freq     frequency to cut to
     */
    void __attLowFreq(FFTResults& fftRes, freq_hz_t freq);

    FFTResults __getFftResults(amplitude_t *x, uint32_t samples, uint32_t sampleRate);

    /**
     * Implementation of the main chord detection algorithm
     *
     * @param   fftRes  reference to FFTResults
     * @return detected chord
     */
    chord_t __getChordFromFftResults(FFTResults& fftRes);

    pcp_t * __FFT2PCP(FFTResults& fftRes);

    chord_t __getChordFromPCPBuf(PCPBuf *pcpBuf);

    void __processSegment(std::vector<segment_t> *segments, uint32_t startIdx,
                          uint32_t endIdx, bool silence, PCPBuf *pcpBuf,
                          ResultsListener *listener, uint32_t samples);

    /**
     * Detect chords based on input time domain data
     *
     * Given a full single channel of time domain data it either fills
     * \p segments with the detected sequence of chord segments or notifies
     * listener \p l on segment by segment retrieval
     *
     * @param   segments    output vector of segments
     * @param   x           full channel time domain data
     * @param   samples     number of samples in x
     * @param   sampleRate  sample rate of x
     * @param   l           listener to report progress to if \p segments is null
     */
    void __getSegments(std::vector<segment_t> *segments, amplitude_t *x,
                       uint32_t samples, uint32_t sr, ResultsListener *l);

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
     * Performs single chord detection with a fixed window size.
     * For sequence detection use getSegments().
     *
     * @param   x           time domain data
     * @param   samples     number of samples in x
     * @param   sampleRate  sample rate of x
     * @return  detected chord
     */
    chord_t getChord(amplitude_t *x, uint32_t samples, uint32_t sampleRate);

    /**
     * Same as __getSegments(segments, x, samples, sr, nullptr);
     */
    void getSegments(std::vector<segment_t>& segments, amplitude_t *x,
                     uint32_t samples, uint32_t sampleRate);

    /**
     * Same as __getSegments(nullptr, x, samples, sr, listener);
     */
    void getSegments(amplitude_t *x, uint32_t samples, uint32_t sampleRate,
                     ResultsListener *listener);

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
