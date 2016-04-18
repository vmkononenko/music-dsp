/*
 * fft.cpp
 *
 *  Fast Fourier Transform implementation
 */

#include <stdexcept>
#include <stdint.h>

#include "fft.h"

using namespace std;


uint32_t FFT::__bitReverse(uint32_t n, uint8_t bitCnt) {
    static uint8_t lookup4[] = {
        0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
        0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
    };

    static uint8_t lookup3[] = {
        0x0, 0x4, 0x2, 0x6, 0x1, 0x5, 0x3, 0x7
    };

    static uint8_t lookup2[] = {
        0x0, 0x2, 0x1, 0x3
    };

    if (bitCnt == 1)
        return n;
    else if (bitCnt == 2)
        return lookup2[n];
    else if (bitCnt == 3)
        return lookup3[n];
    else if (bitCnt == 4)
        return lookup4[n];

    uint32_t ret = 0;
    int i = 1;

    while (n != 0) {
        ret |= (lookup4[n & 0b1111] << (bitCnt - i * 4));
        n >>= 4;
        i++;
    }

    return ret;
}

void FFT::__exch(vector<complex_t> &input, uint32_t i, uint32_t j)
{
    complex<double> tmp = input[i];

    input[i] = input[j];
    input[j] = tmp;
}

void FFT::__sortBitReversal(vector<complex_t> &input,
                            uint32_t start, uint32_t end)
{
    uint8_t maxBits = sqrt(input.size());
    uint32_t reversed;

    for (uint32_t i = start; i <= end; i++) {
        reversed = __bitReverse(i, maxBits);
        if (reversed > i) {
            __exch(input, i, reversed);
        }
    }
}

void FFT::__butterfly(vector<complex_t> &input, uint32_t start, uint32_t end)
{
    uint32_t N = end - start;

    // ??? is frequency correct
    // ??? double for N
    for (uint32_t k = 0; k <= N / 2; k++) {
        complex_t sine  = polar(1.0, -2 * M_PI * k / N);
        complex_t *even = &input[k];
        complex_t *odd  = &input[k + N/2];

        *odd *= sine;
        *even += odd;
        *odd -= *even;
    }
}

void FFT::__forward(vector<complex_t> &input, uint32_t start, uint32_t end)
{
    if (start == end)
        return;

    __sortBitReversal(input, start, end);

    __forward(input, start, end / 2);   // even
    __forward(input, end / 2 + 1, end); // odd

    __butterfly(input, start, end);
}

void FFT::forward(vector<complex_t> &input)
{
	if (input.size() == 0) {
		throw invalid_argument("Empty input");
	}
	__forward(input, 0, input.size() - 1);
}
