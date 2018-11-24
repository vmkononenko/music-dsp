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
#include "q_transform.h"


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

    PitchClsProfile(std::vector<amplitude_t> &fd, QTransform *q_transform);

    /**
     * Get pitch class value for the specified note
     * @param note  note to get PCP for
     * @return      PCP value
     */
    amplitude_t getPitchCls(note_t note) const;

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
} pcp_t;

typedef std::vector<pcp_t> chromagram_t;

}

/** @} */
