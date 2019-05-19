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

#include "cute.h"
#include "helpers_test.h"
#include "lmhelpers.h"

void TestNextPowerOf2::__test()
{
	ASSERT_EQUAL(1, Helpers::nextPowerOf2(0));
	/** @TODO check why nextPowerOf2 for 1 is 2 */
	//ASSERT_EQUAL(1, Helpers::nextPowerOf2(1));
	ASSERT_EQUAL(2, Helpers::nextPowerOf2(2));
	ASSERT_EQUAL(4, Helpers::nextPowerOf2(4));
	ASSERT_EQUAL(8, Helpers::nextPowerOf2(5));
	ASSERT_EQUAL(128, Helpers::nextPowerOf2(100));
	ASSERT_EQUAL(1024, Helpers::nextPowerOf2(1000));
	ASSERT_EQUAL(2147483648, Helpers::nextPowerOf2(1770165767));
}

void TestStdRound::__test()
{
    ASSERT_EQUAL(7.1234, Helpers::stdRound(7.1234123, 4));
}
