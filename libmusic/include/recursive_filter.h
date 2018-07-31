/**
 * @file        cheby1_filter.h
 * @brief       Defines API for Recursive filter
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "filter.h"


class RecursiveFilter : public Filter {

private:
    std::vector<amplitude_t> leftToRight(amplitude_t *td, uint32_t samples);
    std::vector<amplitude_t> rightToLeft(amplitude_t *td, uint32_t samples);

protected:
    std::vector<double> mB;
    std::vector<double> mA;
    uint8_t             mPoles;

    RecursiveFilter() {};

public:
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

    virtual ~RecursiveFilter();
};
