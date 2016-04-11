/*
 * fft.h
 *
 *  Fast Fourier Transform API
 */

#pragma once

#include <complex>
#include <vector>
#include <stdint.h>

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif

typedef std::complex<double> complex_t;

class FFT {

TEST_FRIENDS;

private:
    uint32_t __bitReverse(uint32_t, uint8_t);
    void __sortBitReversal(std::vector<complex_t> &, uint32_t, uint32_t);
    void __forward(std::vector<complex_t> &, uint32_t, uint32_t);
    void __exch(std::vector<complex_t> &, uint32_t, uint32_t);
    void __butterfly(std::vector<complex_t> &input, uint32_t, uint32_t);

public:
	void forward(std::vector<complex_t> &);
};
