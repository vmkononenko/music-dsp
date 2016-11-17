/*
 * window_functions.h
 */

#pragma once

#include "lmtypes.h"

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif

class WindowFunctions {

TEST_FRIENDS;

public:
    static void applyHamming(amplitude_t *, uint32_t);
    static void applyBlackman(amplitude_t *, uint32_t);
};
