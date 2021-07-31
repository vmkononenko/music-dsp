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
#include "viterbi.h"

#ifndef CHORD_DETECTOR_TEST_FRIENDS
#define CHORD_DETECTOR_TEST_FRIENDS
#endif

struct segment_t {
    uint32_t    startIdx;
    uint32_t    endIdx;
    chord_t     chord;
    bool        silence;
};

namespace anatomist {

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
    PitchCalculator& __mPitchCalculator = PitchCalculator::getInstance();
    ChordTplCollection *tpl_collection_;

    FFT * GetFft_(td_t &td, uint32_t samplerate);

    /**
     * Implementation of the main chord detection algorithm
     *
     * @param   fft   pointer to FFT
     * @return  detected chord
     */
    chord_t GetChordFromFft_(FFT *fft);

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
     * @param   sampleRate  sample rate of x
     * @param   l           listener to report progress to if \p segments is null
     */
    void Process_(std::vector<segment_t> *segments, const td_t &x,
                  uint32_t sr, ResultsListener *l, chromagram_t *c);

    float Tune_(tft_t *tft);

    Viterbi::prob_matrix_t GetScoreMatrix_(chromagram_t &chromagram);

    chromagram_t ChromagramFromSpectrogram_(tft_t *tft);

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

    pcp_t * GetPCP(amplitude_t *x, uint32_t samples, uint32_t samplerate);

    chromagram_t GetChromagram(amplitude_t *x, uint32_t samples, uint32_t samplerate);
};

}

/** @} */
