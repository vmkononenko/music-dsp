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
 * @file        window_functions.h
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <vector>

#include "lmtypes.h"

#ifndef WINDOW_FUNCIONS_TEST_FRIENDS
#define WINDOW_FUNCIONS_TEST_FRIENDS
#endif


class WindowFunctions {

WINDOW_FUNCIONS_TEST_FRIENDS;

public:
    static void applyHamming(td_t td);
    static void applyHann(td_t td);
    static void applyBlackman(td_t td);
    static void applyDefault(td_t td);

    static std::vector<amplitude_t> getHamming(uint32_t len, uint32_t offset);

    static const char * toString(uint32_t);
};

/** @} */
