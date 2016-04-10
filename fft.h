/*
 * fft.h
 *
 *  Fast Fourier Transform API
 */

#pragma once

#include <complex>
#include <vector>
#include <stdint.h>

class FFT {
private:
    uint32_t __bitReverse(uint32_t);
    void __sortBitReversal(std::vector< std::complex<double> > &, uint32_t, uint32_t);
    void __forward(std::vector< std::complex<double> > &, uint32_t, uint32_t);
    void __exch(std::vector< std::complex<double> > &, uint32_t, uint32_t);
    void __butterfly(std::vector< std::complex<double> > &input, uint32_t, uint32_t);

public:
	void forward(std::vector< std::complex<double> > &);
};
