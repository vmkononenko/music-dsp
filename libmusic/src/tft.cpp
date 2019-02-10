/*
 * fft.cpp
 *
 *  TFT class implementation
 */

#include <algorithm>

#include "lmhelpers.h"
#include "tft.h"

namespace anatomist {

TFT::TFT(freq_hz_t f_min, freq_hz_t f_max, uint16_t bpo, uint32_t sample_rate,
         uint16_t win_size, uint16_t hop_size) :
                                            f_min_(f_min),
                                            f_max_(f_max),
                                            bpo_(bpo),
                                            sample_rate_(sample_rate),
                                            win_size_(win_size),
                                            hop_size_(hop_size)
{
    spectrogram_ = log_spectrogram_t(0, fd_t(0));
    interval_ = win_size;
}

log_spectrogram_t TFT::GetSpectrogram()
{
    return spectrogram_;
}

uint32_t TFT::SpectrogramInterval()
{
    return interval_;
}

void TFT::Denoise_(log_spectrogram_t &block)
{
    for (auto & col : block) {
        std::vector<amplitude_t> devs;
        amplitude_t thr_uni, sigma, mad;
        amplitude_t median = Helpers::median(col);

        for (auto val : col) {
            devs.push_back(abs(val - median));
        }

        mad = Helpers::median(devs);

        sigma = mad / 0.6745;

        thr_uni = sigma * sqrt(2 * log10(interval_));

        for (auto & bin : col) {
            if (abs(bin) < thr_uni) {
                bin = 0;
            }
        }
    }
}

}
