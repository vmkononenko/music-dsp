/**
 * @file        cheby1_filter.h
 * @brief       Defines API for Chebyshev type 1 filter
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "filter.h"

#define CHEBYSHEV_POLES ((uint8_t) 4)

class Cheby1Filter : public Filter {

private:
    /*
     * 4 pole low pass filter coefficients
     * Calculated with Octave for cutoff frequency 0.0027211 (240 Hz for 22050)
     */
    double a1 = -3.98634597829217;
    double a2 = 5.959203624346703;
    double a3 = -3.959368305507797;
    double a4 = 0.986510662801611;

    double b0 = 2.051277523274928e-10;
    double b1 = 8.205110093099713e-10;
    double b2 = 1.230766513964957e-09;
    double b3 = 8.205110093099713e-10;
    double b4 = 2.051277523274928e-10;

    std::vector<amplitude_t> leftToRight(amplitude_t *td, uint32_t samples);
    std::vector<amplitude_t> rightToLeft(amplitude_t *td, uint32_t samples);

public:
    Cheby1Filter();

    /**
     *  Performs zero-phase filtering
     *
     *  Zero phase filtering is achieved by processing the input data
     *  in both the forward and reverse directions.
     *
     *  @param td       time domain of the input signal
     *  @param samples  length of the \p td
     *  @return         filtered data
     */
    std::vector<amplitude_t> process(amplitude_t *td, uint32_t samples) override;

    ~Cheby1Filter();
};
