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
