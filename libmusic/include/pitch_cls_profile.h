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

    /**
     * Normalize obtained PCP amplitudes
     *
     * Because amplitudes in chord type templates are defined as 0 and 1
     */
    void __normalize(amplitude_t magMax);
public:
    /**
     * Constructor
     */
    PitchClsProfile(amplitude_t *freqDomainMagnitudes, uint32_t fftSize,
                    uint32_t sampleRate, uint32_t pointsCnt);

    /**
     * Get pitch class value for the specified note
     * @param note  note to get PCP for
     * @return      PCP value
     */
    amplitude_t getPitchCls(note_t note);
} pcp_t;

/** @} */
