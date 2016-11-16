/*
 * lmhelpers.h
 *
 * Header file for helpers
 */

#pragma once

#include <stdint.h>
#include <vector>
#include "lmtypes.h"

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif

class Helpers {

TEST_FRIENDS;

public:
	static uint32_t nextPowerOf2(uint32_t);
	static std::vector<complex_t> timeDomain2ComplexVector(amplitude_t *, uint32_t);
};
