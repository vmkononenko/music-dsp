/*
 * fft.cpp
 *
 *  TFT class implementation
 */


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

}
