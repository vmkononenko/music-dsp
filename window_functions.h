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
	static void hamming(amplitude_t *, uint32_t);
	static void blackman(amplitude_t *, uint32_t);
};
