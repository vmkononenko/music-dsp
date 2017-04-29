#include "cute.h"

#include "fft_test.h"
#include "lmhelpers.h"

void TestBitReverse::__test()
{
    FFT *fft = new FFT();
    uint32_t x;

    x = 0x2;
    ASSERT_EQUAL(0x4, fft->__bitReverse(x, 4));

    x = 0x4;
    ASSERT_EQUAL(0x2, fft->__bitReverse(x, 4));

    x = 0;
    ASSERT_EQUAL(0, fft->__bitReverse(x, 4));

    x = 0b011;
    ASSERT_EQUAL(0b110, fft->__bitReverse(x, 3));

    x = 0xa;
    ASSERT_EQUAL(0x50000000, fft->__bitReverse(x, 32));

    x = 0x83;
    ASSERT_EQUAL(0xc1000000, fft->__bitReverse(x, 32));

    x = 0xe42;
    ASSERT_EQUAL(0x42700000, fft->__bitReverse(x, 32));

    x = 0xcb035000;
    ASSERT_EQUAL(0xac0d3, fft->__bitReverse(x, 32));

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
    fft->__sortBitReversal(v2, 0, v2.size() - 1);

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

    fft->__sortBitReversal(v2, 8, v2.size() - 1);

    ASSERT_EQUAL(v1, v2);

    delete fft;
}

void TestForwardTransform::__test()
{
    FFT *fft = new FFT();
    std::vector<complex_t> src, expected;

    src.push_back(complex_t (1, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));

    expected.push_back(complex_t (1, 0));
    expected.push_back(complex_t (1, 0));
    expected.push_back(complex_t (1, 0));
    expected.push_back(complex_t (1, 0));
    expected.push_back(complex_t (1, 0));
    expected.push_back(complex_t (1, 0));
    expected.push_back(complex_t (1, 0));
    expected.push_back(complex_t (1, 0));

    fft->forward(src);
    ASSERT_EQUAL(expected, src);

    src.clear();
    expected.clear();

    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (1, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));

    expected.push_back(complex_t (1, 0));
    expected.push_back(complex_t (0.7071, -0.7071));
    expected.push_back(complex_t (0, -1));
    expected.push_back(complex_t (-0.7071, -0.7071));
    expected.push_back(complex_t (-1, 0));
    expected.push_back(complex_t (-0.7071, 0.7071));
    expected.push_back(complex_t (0, 1));
    expected.push_back(complex_t (0.7071, 0.7071));

    fft->forward(src);

    for (uint32_t i = 0; i < src.size(); i++) {
        src[i] = complex_t (Helpers::stdRound(real(src[i]), 4),
                            Helpers::stdRound(imag(src[i]), 4));
    }
    ASSERT_EQUAL(expected, src);

    delete fft;
}

void TestInverseTransform::__test() {
    FFT *fft = new FFT();
    std::vector<complex_t> src, inverse;

    src.push_back(complex_t (1, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));
    src.push_back(complex_t (0, 0));

    inverse = src;

    fft->forward(inverse);
    fft->inverse(inverse);

    ASSERT_EQUAL(src, inverse);

    delete fft;
}
