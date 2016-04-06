/*
 * fft.h
 *
 *  Fast Fourier Transform API
 */

#pragma once

#include <complex>

class FFT {
public:
	FFT();
	std::complex<double *> forward(std::complex<double *>, int);
	std::complex<double *> inverse(std::complex<double *>, int);
	virtual ~FFT();
};
