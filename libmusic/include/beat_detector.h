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
 * @file        beat_detector.h
 * @brief       API for BPM detection
 *
 * Main algorithm steps:
 *   1. Take the 1st derivative of the envelope
 *   2. Take FFT of the derivative obtained in step 1
 *   3. Find max frequency from the FFT
 *   4. Normalize obtained frequency to the human readable range
 *
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "envelope.h"
#include "lmtypes.h"


namespace anatomist {

class BeatDetector {

private:
    uint32_t __mOffset;
    uint32_t __mBeatIdxInterval;
    float __mBPM;

    void detectBeat(Envelope *e, uint32_t sampleRate);

    /**
     * Generate vector of indices at which attack happens
     *
     * @param offset        index with a beat with the highest amplitude.
     *                      The rest of indices will be evenly spread around this one
     * @param interval      distance between two beats
     * @param upperLimit    right boundary
     */
    void createBeatIndices(int32_t offset, int32_t interval, int32_t upperLimit);

    void normalizeInterval();

    /**
     * Convert beat frequency in hertz to beats per minute with normalization
     *
     * Saves calculated value in the member field
     *
     * @param beatHz    beat frequency in hertz
     */
    void hz2BPM(freq_hz_t beatHz);

    BeatDetector();

public:
    BeatDetector(amplitude_t *td, uint32_t samples, uint32_t sampleRate);

    BeatDetector(Envelope *e, uint32_t sampleRate);

    uint32_t getOffset();

    uint32_t getIdxInterval();

    float getBPM();

    ~BeatDetector();
};

}
