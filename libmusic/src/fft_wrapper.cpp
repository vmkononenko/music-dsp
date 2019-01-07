/*
 * fft_wrapper.cpp
 *
 *  FFTWrapper class implementation
 */

#include "fft_wrapper.h"

void FFTWrapper::Process(td_t td, uint32_t offset)
{
    WindowFunctions::applyDefault(td, samples);

    return new FFT(td, samples, samplerate, FREQ_E2, FREQ_C8, true, false);

    for (uint32_t sample_idx = offset; sample_idx < samples; sample_idx += hop_size) {
        uint32_t len = min(win_size, samples - sample_idx);
        FFT *fft = GetFft_(td + sample_idx, len, samplerate);
        delete fft;
    }
}
