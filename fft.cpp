/*
 * fft.cpp
 *
 *  Fast Fourier Transform implementation
 */

#include <stdexcept>
#include <stdint.h>

#include "fft.h"

using namespace std;


uint32_t FFT::__bitReverse(uint32_t n) {
    static uint8_t lookup[] = {
        0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
        0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
    };

    uint32_t ret = 0;
    int i = 1;

    while (n != 0) {
        ret |= (lookup[n & 0xf] << (8 * sizeof(n) - i * 4));
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
    for (uint32_t i = start; i <= end; i++) {
        __exch(input, i, __bitReverse(i));
    }
}

void FFT::__butterfly(vector<complex_t> &input, uint32_t start, uint32_t end)
{
    uint32_t N = end - start;

    // ??? is frequency correct
    for (uint32_t k = 0; k <= N / 2; k++) {
        complex_t S = polar(1.0, -2 * M_PI * k / N);

        input[k] =  input[k] * S;
        input[k] += input[k + N/2];
        input[k + N/2] -= input[k];
    }
}

void FFT::__forward(vector<complex_t> &input, uint32_t start, uint32_t end)
{
    if (start == end)
        return;

    __sortBitReversal(input, start, end);

    // ??? which part is odd and which is even
    __forward(input, start, end / 2);
    __forward(input, end / 2 + 1, end);

    __butterfly(input, start, end);
}

void FFT::forward(vector<complex_t> &input)
{
	if (input.size() == 0) {
		throw invalid_argument("Empty input");
	}
	__forward(input, 0, input.size() - 1);
}
