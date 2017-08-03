/*
 * window_functions.h
 */

#pragma once

#include "lmtypes.h"

#ifndef WINDOW_FUNCIONS_TEST_FRIENDS
#define WINDOW_FUNCIONS_TEST_FRIENDS
#endif

class WindowFunctions {

WINDOW_FUNCIONS_TEST_FRIENDS;

public:
    static void applyHamming(amplitude_t *, uint32_t);
    static void applyHann(amplitude_t *, uint32_t);
    static void applyBlackman(amplitude_t *, uint32_t);
    static void applyDefault(amplitude_t *, uint32_t);
    static const char * toString(window_func_t);
};
