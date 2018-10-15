/**
 * @file        viterbi.h
 * @brief       Viterbi algorithm API
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"

class Viterbi {

public:
    typedef std::vector<std::vector<double>> obs_matrix_t;

    static std::vector<uint32_t> GetPath(obs_matrix_t &obs,
                                         std::vector<double> init_p,
                                         std::vector<std::vector<double>> trans_p);
};

/** @} */
