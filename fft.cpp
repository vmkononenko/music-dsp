/*
 * fft.cpp
 *
 *  Fast Fourier Transform implementation
 */

#include <stdexcept>
#include <math.h>

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
    complex<amplitude_t> tmp = input[i];

    input[i] = input[j];
    input[j] = tmp;
}

void FFT::__sortBitReversal(vector<complex_t> &input,
                            uint32_t start, uint32_t end)
{
    uint32_t N = end - start + 1;
    uint8_t maxBits = log2(N);
    uint32_t reversed;


    for (uint32_t i = 0; i < N; i++) {
        reversed = __bitReverse(i, maxBits);
        if (reversed >  i) {
            __exch(input, start + i, start + reversed);
        }
    }
}

void FFT::__butterfly(vector<complex_t> &input, uint32_t start, uint32_t end)
{
    double N = end - start + 1;

    for (uint32_t k = 0; k < N / 2; k++) {
        complex_t twiddle = complex_t(cos(2 * M_PI * k / N), -sin(2 * M_PI * k / N));
        complex_t *even = &input[start + k];
        complex_t *odd  = &input[start + k + N/2];
        complex_t tmp = *odd * twiddle;

        *odd = *even - tmp;
        *even = *even + tmp;
    }
}

void FFT::__forward(vector<complex_t> &input, uint32_t start, uint32_t end)
{
    if (start == end)
        return;

    __forward(input, start, start + (end - start) / 2);   // even
    __forward(input, start + (end - start) / 2 + 1, end); // odd

    __butterfly(input, start, end);
}

void FFT::__forward(vector<complex_t> &input)
{
    uint32_t stages = log2(input.size());

    for (uint32_t stage = 1; stage <= stages; stage++) {
        double subFftPoints = 1 << stage;

        for (uint32_t j = 0; j < input.size(); j += subFftPoints) {
            for (uint32_t i = 0; i < subFftPoints / 2; i++) {
                complex_t twiddle = complex_t(cos(2 * M_PI * i / subFftPoints),
                                             -sin(2 * M_PI * i / subFftPoints));
                complex_t *even = &input[j + i];
                complex_t *odd  = &input[j + i + subFftPoints / 2];
                complex_t tmp = *odd * twiddle;

                *odd = *even - tmp;
                *even = *even + tmp;
            }
        }
    }
}

void FFT::forward(vector<complex_t> &input)
{
    if (input.size() == 0) {
        throw invalid_argument("Empty input");
    }

    __sortBitReversal(input, 0, input.size() - 1);
    //__forward(input, 0, input.size() - 1);
    __forward(input);
}

/**
 * Converts frequency domain from rectangular to polar notation
 *
 * @param input frequency domain in rectangular notation
 */
void FFT::toPolar(std::vector<complex_t> &input)
{
    if (input.size() == 0) {
        throw invalid_argument("Empty input");
    }

    for (unsigned int i = 0; i < input.size() / 2; i++) {
        double re = real(input[i]);
        double im = imag(input[i]);
        double mag = sqrt(im * im + re * re);
        double theta;

        if (re == 0) {
            // if the real part is zero, change it to negligibly
            // small number to avoid division by 0
            re = 1e-20;
        }

        theta = atan(im/re);

        input[i] = complex_t (mag, theta);
    }
}

/**
 * Inverse DFT calculation
 *
 * @param input frequency domain in rectangular notation
 */
void FFT::inverse(vector<complex_t> &input)
{
    if (input.size() == 0) {
        throw invalid_argument("Empty input");
    }

    unsigned int N = input.size();

    /* change the sign of the imaginary part */
    for (unsigned int k = 1; k < N; k++) {
        input[k] = complex_t (real(input[k]), -imag(input[k]));
    }

    forward(input);

    // divide the time domain by N and change the sign of imaginary part
    for (unsigned int i = 1; i < N; i++) {
        input[i] = complex_t (real(input[i]) / N, -imag(input[i]) / N);
    }
}
