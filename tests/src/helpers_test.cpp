#include "cute.h"
#include "helpers_test.h"
#include "lmhelpers.h"

void TestNextPowerOf2::__test()
{
	ASSERT_EQUAL(1, Helpers::nextPowerOf2(0));
	// TODO: check why nextPowerOf2 for 1 is 2
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
