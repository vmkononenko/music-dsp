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
    typedef std::vector<std::vector<prob_t>> prob_matrix_t;

    static std::vector<uint32_t> GetPath(std::vector<prob_t> &init_p,
                                         prob_matrix_t &obs,
                                         prob_matrix_t &trans_p);

private:
    static void ValidateMatrix_(const prob_matrix_t &obs);
    static bool ValidateProbVector_(const std::vector<prob_t> &v);
    static void ValidateInitProbs_(const std::vector<prob_t> &init_p);
};

/** @} */
