/**
 * @file        pitch_cls_profile.h
 * @brief       Pitch Class Profile header file
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"


typedef class PitchClsProfile {

private:

    std::vector<amplitude_t> __mPCP;
    amplitude_t __mPitchClsMax = 0;

    /**
     * Normalize obtained PCP amplitudes
     *
     * Because amplitudes in chord type templates are defined as 0 and 1
     */
    void __normalize();
public:
    /**
     * Constructor for creating an empty profile
     */
    PitchClsProfile();

    PitchClsProfile(amplitude_t *freqDomainMagnitudes, uint32_t fftSize,
                    uint32_t sampleRate, uint32_t pointsCnt);

    /**
     * Get pitch class value for the specified note
     * @param note  note to get PCP for
     * @return      PCP value
     */
    amplitude_t getPitchCls(note_t note) const;

    template<typename T> amplitude_t euclideanDistance(std::vector<T> &v)
    {
        amplitude_t sum = 0;

        for (int n = note_Min; n <= note_Max; n++) {
            note_t note = (note_t)n;

            amplitude_t diff = ((amplitude_t)__mPCP[note - note_Min] - v[note - note_Min]);
            sum += diff * diff;
        }

        return sqrt(sum);
    }

    amplitude_t euclideanDistance(PitchClsProfile &pcp);

    PitchClsProfile & operator+=(const PitchClsProfile& pcp);

    PitchClsProfile & operator/=(float denominator);

    friend std::ostream& operator<<(std::ostream& os, const PitchClsProfile& pcp);
} pcp_t;

/** @} */
