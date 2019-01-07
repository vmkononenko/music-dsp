/*
 * fft.cpp
 *
 *  Fast Fourier Transform implementation
 */

#include <algorithm>
#include <math.h>
#include <stdexcept>
#include <string.h>

#include "config.h"
#include "fft.h"
#include "lmhelpers.h"

using namespace std;

namespace anatomist {

FFT::FFT(amplitude_t *td, uint32_t td_len, uint32_t samplerate, freq_hz_t f_low,
        freq_hz_t f_high, bool polar, bool hps)
{
    if ((td == nullptr) || (td_len == 0) || (f_low >= f_high) || (f_high > samplerate / 2))
    {
        throw invalid_argument("FFT(): invalid argument");
    }

    size_ = (td_len <= CFG_WINDOW_SIZE) ? CFG_WINDOW_SIZE : Helpers::nextPowerOf2(td_len);
    samplerate_ = samplerate;
    polar_ = polar;
    fd_len_ = FreqToIdx(f_high, ceil) + 1;

    vector<complex_t> x = Helpers::timeDomain2ComplexVector(td, td_len, size_);
    uint32_t f_low_idx = (f_low == 0) ? 0 : FreqToIdx(f_low, floor);

    Forward_(x);

    if (!polar) {
        fd_.set_r(new vector<complex_t>(x));
        if (f_low_idx > 0) {
            AttLowFreqs(f_low_idx);
        }
        return;
    }

    fd_.p = new amplitude_t[fd_len_ + CFG_FFT_AVG_WINDOW - 1];
    memset(fd_.p, 0, sizeof(fd_.p[0]) * fd_len_);

    fd_len_ = ToPolar_(x, fd_.p, fd_len_ + CFG_FFT_AVG_WINDOW - 1, f_low_idx);

#if (defined(CFG_FFT_AVG_WINDOW) && (CFG_FFT_AVG_WINDOW > 1))
    Avg_(fd_.p, fd_len_, CFG_FFT_AVG_WINDOW);
#endif

#if (defined(CFG_FFT_HPS_HARMONICS) && (CFG_FFT_HPS_HARMONICS > 1))
    if (hps) {
        ToHPS_(fd_.p, fd_len_);
    }
#else
    UNUSED(hps);
#endif

}

FFT::FFT(amplitude_t *td, uint32_t td_len, uint32_t samplerate, bool polar) :
        FFT(td, td_len, samplerate, 0, samplerate / 2, polar, false) {}

FFT::FFT(td_t td, uint32_t samplerate, freq_hz_t f_low, freq_hz_t f_high) :
     FFT(td.data(), static_cast<uint32_t>(td.size()), samplerate, f_low,
         f_high, true, false) {}

FFT::FFT()
{
    fd_.p = nullptr;
    fd_len_ = 0;
    size_ = 0;
    samplerate_ = 0;
    polar_ = false;
}

FFT::~FFT()
{
    if (polar_) {
        free(fd_.p);
    } else {
        delete fd_.r();
    }
}

void FFT::AttLowFreqs(uint32_t f_low_idx)
{
    for (uint32_t i = 0; i < f_low_idx; i++) {
        (*fd_.r())[i] = complex_t(0, 0);
    }
}

uint32_t FFT::BitReverse_(uint32_t n, uint8_t bitCnt) {
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

void FFT::Exch_(vector<complex_t> &input, uint32_t i, uint32_t j)
{
    complex<amplitude_t> tmp = input[i];

    input[i] = input[j];
    input[j] = tmp;
}

void FFT::SortBitReversal_(vector<complex_t> &input,
                            uint32_t start, uint32_t end)
{
    uint32_t N = end - start + 1;
    uint8_t maxBits = log2(N);
    uint32_t reversed;


    for (uint32_t i = 0; i < N; i++) {
        reversed = BitReverse_(i, maxBits);
        if (reversed >  i) {
            Exch_(input, start + i, start + reversed);
        }
    }
}

void FFT::Butterfly_(vector<complex_t> &input, uint32_t start, uint32_t end)
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

void FFT::Forward_(vector<complex_t> &input, uint32_t start, uint32_t end)
{
    if (start == end)
        return;

    Forward_(input, start, start + (end - start) / 2);   // even
    Forward_(input, start + (end - start) / 2 + 1, end); // odd

    Butterfly_(input, start, end);
}

void FFT::Forward_(vector<complex_t> &input)
{
    if (input.size() == 0) {
        throw invalid_argument("Empty input");
    }

    uint32_t stages = log2(input.size());

    SortBitReversal_(input, 0, input.size() - 1);

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

uint32_t FFT::ToPolar_(vector<complex_t> &fd, amplitude_t *fd_magnitudes,
                       PriorityQueue *pq, uint32_t req_len, uint32_t start_idx)
{
    if (fd.size() == 0) {
        throw invalid_argument("Empty input");
    }

    uint32_t len = min(req_len, static_cast<uint32_t>(fd.size()) / 2);

    for (unsigned int i = start_idx; i < len; i++) {
        double re = real(fd[i]);
        double im = imag(fd[i]);
        double mag = sqrt(im * im + re * re);
        double theta;

        if (pq != NULL) {
            pq->insert(FftPoint(i, mag));
        } else if (fd_magnitudes != NULL) {
            fd_magnitudes[i] = mag;
        } else {
            if (re == 0) {
                // if the real part is zero, change it to negligibly
                // small number to avoid division by 0
                re = 1e-20;
            }

            theta = atan(im/re);
            fd[i] = complex_t (mag, theta);
        }
    }

    return len;
}

uint32_t FFT::ToPolar_(vector<complex_t> &fd, amplitude_t *fd_magnitudes,
                       uint32_t req_len, uint32_t start_idx)
{
    return ToPolar_(fd, fd_magnitudes, nullptr, req_len, start_idx);
}

void FFT::ToPolar_(vector<complex_t> &input)
{
    ToPolar_(input, nullptr, nullptr, input.size(), 0);
}

void FFT::ToHPS_(amplitude_t *fd_magnitudes, uint32_t fd_len)
{
    if (!polar_) {
        throw runtime_error("ToHPS_(): only implemented for polar notation");
    }

    for (uint8_t m = 1; m < CFG_FFT_HPS_HARMONICS; m++) {
        for (uint32_t i = 0; i < fd_len / pow(2, m); i++) {
            fd_magnitudes[i] *= fd_magnitudes[i * (uint32_t)pow(2, m)];
        }
    }
}

void FFT::Avg_(amplitude_t *fd_magnitudes, uint32_t fd_len, uint32_t window) {
    if (window == 1) {
        return;
    }

    for (uint32_t i = 0; i < fd_len; i++) {
        for (uint32_t j = 1; j < window; j++) {
            fd_magnitudes[i] += fd_magnitudes[i+j];
        }
        fd_magnitudes[i] /= window;
    }
}

void FFT::Inverse(std::vector<complex_t> & input)
{
    if (polar_) {
        throw runtime_error("Inverse_(): only implemented for rectangular notation");
    }

    unsigned int N = input.size();

    /* change the sign of the imaginary part */
    for (uint32_t k = 0; k < N; k++) {
        input[k] = complex_t (real(input[k]), -imag(input[k]));
    }

    Forward_(input);

    // divide the time domain by N and change the sign of imaginary part
    for (uint32_t i = 0; i < N; i++) {
       input[i] = complex_t (real(input[i]) / N, -imag(input[i]) / N);
    }
}

void FFT::Inverse()
{
    Inverse(*fd_.r());
}

uint32_t FFT::FreqToIdx(freq_hz_t freq, double (*roundFunc)(double))
{
    return (*roundFunc)(freq * size_ / samplerate_);
}

freq_hz_t FFT::IdxToFreq(uint32_t idx)
{
    return (idx * (freq_hz_t)samplerate_ / size_);
}

}
