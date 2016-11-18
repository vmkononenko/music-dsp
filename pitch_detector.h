/*
 * pitch_detector.h
 */

#pragma once

#include <vector>

#include "lmtypes.h"

#define FREQ_INVALID    ((freq_hz_t) -1)

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif

class PitchDetector {

TEST_FRIENDS;

private:
    bool __isPitch(freq_hz_t freq);

public:
    /**
     * Correspond frequency to the pitch
     *
     * In other words round up provided frequency value to one of the pitch
     * frequencies
     *
     * @param   freq arbitrary frequency
     * @return  pitch frequency corresponding to freq
     */
    freq_hz_t getPitch(freq_hz_t freq);

    /**
     * Find frequency with the highest amplitude
     *
     * @param   x frequency domain data
     * @return  frequency value
     */
    freq_hz_t getTonic(std::vector<complex_t> x);

    /**
     * Find note corresponding to the given pitch
     *
     * @param   pitch frequency in Hz
     * @return  note corresponding to the given pitch
     */
    note_t pitchToNote(freq_hz_t freq);
};

