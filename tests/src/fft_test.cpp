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

#include <string.h>

#include "cute.h"

#include "fft_test.h"
#include "lmhelpers.h"


namespace anatomist {

void FftTestHelper::TestTransform(std::vector<complex_t> &exp,
                                std::vector<complex_t> &src)
{
    FFT *fft = new FFT();
    std::vector<complex_t> src_orig = src;

    fft->Forward_(src);

    for (uint32_t i = 0; i < src.size(); i++) {
        src[i] = complex_t (Helpers::stdRound(real(src[i]), 4),
                            Helpers::stdRound(imag(src[i]), 4));
    }

    ASSERT_EQUAL(exp, src);

    fft->Inverse(src);

    for (uint32_t i = 0; i < src.size(); i++) {
        src[i] = complex_t (Helpers::stdRound(real(src[i]), 0),
                            Helpers::stdRound(imag(src[i]), 0));
    }

    ASSERT_EQUAL(src_orig, src);

    delete fft;
}

void TestBitReverse::__test()
{
    FFT *fft = new FFT();
    uint32_t x;

    x = 0x2;
    ASSERT_EQUAL(0x4, fft->BitReverse_(x, 4));

    x = 0x4;
    ASSERT_EQUAL(0x2, fft->BitReverse_(x, 4));

    x = 0;
    ASSERT_EQUAL(0, fft->BitReverse_(x, 4));

    x = 0b011;
    ASSERT_EQUAL(0b110, fft->BitReverse_(x, 3));

    x = 0xa;
    ASSERT_EQUAL(0x50000000, fft->BitReverse_(x, 32));

    x = 0x83;
    ASSERT_EQUAL(0xc1000000, fft->BitReverse_(x, 32));

    x = 0xe42;
    ASSERT_EQUAL(0x42700000, fft->BitReverse_(x, 32));

    x = 0xcb035000;
    ASSERT_EQUAL(0xac0d3, fft->BitReverse_(x, 32));

    delete fft;
}

void TestSortBitReversal::__test()
{
    FFT *fft = new FFT();
    std::vector<complex_t> v1, v2;

    v1.push_back(complex_t (0, 0));
    v1.push_back(complex_t (8, 8));
    v1.push_back(complex_t (4, 4));
    v1.push_back(complex_t (12, 12));
    v1.push_back(complex_t (2, 2));
    v1.push_back(complex_t (10, 10));
    v1.push_back(complex_t (6, 6));
    v1.push_back(complex_t (14, 14));
    v1.push_back(complex_t (1, 1));
    v1.push_back(complex_t (9, 9));
    v1.push_back(complex_t (5, 5));
    v1.push_back(complex_t (13, 13));
    v1.push_back(complex_t (3, 3));
    v1.push_back(complex_t (11, 11));
    v1.push_back(complex_t (7, 7));
    v1.push_back(complex_t (15, 15));

    for (int i = 0; i < 16; i++) {
        complex_t c(i, i);
        v2.push_back(c);
    }
    fft->SortBitReversal_(v2, 0, v2.size() - 1);

    ASSERT_EQUAL(v1, v2);

    while (v1.size() != 8) {
        v1.pop_back();
    }

    v1.push_back(complex_t (1, 1));
    v1.push_back(complex_t (3, 3));
    v1.push_back(complex_t (5, 5));
    v1.push_back(complex_t (7, 7));
    v1.push_back(complex_t (9, 9));
    v1.push_back(complex_t (11, 11));
    v1.push_back(complex_t (13, 13));
    v1.push_back(complex_t (15, 15));

    fft->SortBitReversal_(v2, 8, v2.size() - 1);

    ASSERT_EQUAL(v1, v2);

    delete fft;
}

void TestForwardTransform01::__test()
{
    std::vector<complex_t> src = {
            complex_t (1, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
    };

    std::vector<complex_t> exp = {
            complex_t (1, 0),
            complex_t (1, 0),
            complex_t (1, 0),
            complex_t (1, 0),
            complex_t (1, 0),
            complex_t (1, 0),
            complex_t (1, 0),
            complex_t (1, 0),
    };

    FftTestHelper::TestTransform(exp, src);
}

void TestForwardTransform02::__test()
{
    std::vector<complex_t> src = {
            complex_t (0, 0),
            complex_t (1, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
            complex_t (0, 0),
    };

    std::vector<complex_t> exp = {
            complex_t (1, 0),
            complex_t (0.7071, -0.7071),
            complex_t (0, -1),
            complex_t (-0.7071, -0.7071),
            complex_t (-1, 0),
            complex_t (-0.7071, 0.7071),
            complex_t (0, 1),
            complex_t (0.7071, 0.7071),
    };

    FftTestHelper::TestTransform(exp, src);
}


void TestForwardTransform03::__test()
{
    std::vector<complex_t> src = {
            complex_t (0, 0),
            complex_t (1, 0),
            complex_t (0, 0),
            complex_t (-1, 0),
    };

    std::vector<complex_t> exp = {
            complex_t (0, 0),
            complex_t (0, -2),
            complex_t (0, 0),
            complex_t (0, 2),
    };

    FftTestHelper::TestTransform(exp, src);
}

void TestAvg::__test() {
    FFT *fft = new FFT();
    amplitude_t orig[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
    uint8_t origLen = sizeof(orig) / sizeof(orig[0]);
    amplitude_t *res = (amplitude_t *) malloc(sizeof(orig[0]) * origLen);

    memcpy(res, orig, origLen * sizeof(orig[0]));

    fft->Avg_(res, origLen, 1);

    for (uint8_t i = 0; i < origLen; i++) {
        ASSERT_EQUAL(orig[i], res[i]);
    }

    fft->Avg_(res, origLen - 3, 4);

    ASSERT_EQUAL(res[0], 25);
    ASSERT_EQUAL(res[1], 35);
    ASSERT_EQUAL(res[2], 45);
    ASSERT_EQUAL(res[3], 55);
    ASSERT_EQUAL(res[4], 65);
    ASSERT_EQUAL(res[5], 75);
    ASSERT_EQUAL(res[6], 85);
    ASSERT_EQUAL(res[7], 80);
    ASSERT_EQUAL(res[8], 90);
    ASSERT_EQUAL(res[9], 100);

    free(res);
    delete fft;
}

}

