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
 * @file        pitch_cls_profile.h
 * @brief       Pitch Class Profile header file
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "fft.h"
#include "lmtypes.h"
#include "tft.h"


namespace anatomist {

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

    PitchClsProfile(FFT *fft);

    PitchClsProfile(fd_t &fd, tft_t *tft);

    /**
     * Get pitch class value for the specified note
     * @param note  note to get PCP for
     * @param is_treble true for treble notes, false for bass notes. Default is true.
     * @return      PCP value
     */
    amplitude_t getPitchCls(note_t note, bool is_treble = true) const;

    size_t size();

    template<typename T> amplitude_t euclideanDistance(std::vector<T> &v)
    {
        if (v.size() != __mPCP.size()) {
            throw std::invalid_argument("euclideanDistance(): wrong vector size");
        }

        amplitude_t sum = 0;

        for (uint16_t i = 0; i < __mPCP.size(); i++) {
            amplitude_t diff = (__mPCP[i] - v[i]);
            sum += diff * diff;
        }

        return sqrt(sum);
    }

    amplitude_t euclideanDistance(PitchClsProfile &pcp);

    amplitude_t divergenceKullbackLeibler(std::vector<amplitude_t> &v);

    amplitude_t sumProduct(std::vector<amplitude_t> &v);

    PitchClsProfile & operator+=(const PitchClsProfile& pcp);

    PitchClsProfile & operator/=(float denominator);

    friend std::ostream& operator<<(std::ostream& os, const PitchClsProfile& pcp);

    std::string toCSV();
} pcp_t;

typedef std::vector<pcp_t> chromagram_t;

}

/** @} */
