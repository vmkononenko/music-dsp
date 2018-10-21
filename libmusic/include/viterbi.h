/**
 * @file        viterbi.h
 * @brief       Viterbi algorithm API
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"

#ifndef VITERBI_TEST_FRIENDS
#define VITERBI_TEST_FRIENDS
#endif

class Viterbi {

VITERBI_TEST_FRIENDS;

public:
    typedef std::vector<std::vector<double>> obs_matrix_t;

    static std::vector<uint32_t> GetPath(obs_matrix_t &obs,
                                         std::vector<double> init_p,
                                         std::vector<std::vector<double>> trans_p);

private:
    static void ValidateMatrix_(const std::vector<std::vector<double>> &obs);
    static bool ValidateProbVector_(const std::vector<double> &v);
    static void ValidateInitProbs_(const std::vector<double> &init_p);
};

/** @} */
