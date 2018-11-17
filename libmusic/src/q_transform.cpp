#include <algorithm>
#include <cmath>

#include "CQParameters.h"

#include "lmhelpers.h"
#include "q_transform.h"


using namespace std;

QTransform::QTransform(uint8_t bpo, uint32_t samplerate, freq_hz_t f_low,
                       freq_hz_t f_high, uint32_t win_size)
{
    CQParameters p(samplerate, f_low, f_high, bpo);

    /*
     * From the paper:
     * values q < 1 can be seen to implement oversampling of the frequency axis,
     * analogously to the use of zero padding when calculating the DFT.
     * For example q = 0.5 corresponds to oversampling factor of 2.
     */
    p.q = 0.5;

    cq_spectrogram_ = new CQSpectrogram(p, CQSpectrogram::InterpolateLinear);

    if (!cq_spectrogram_->isValid()) {
        throw new runtime_error("Failed to construct a Q-Transform");
    }

    f_min_ = cq_spectrogram_->getMinFrequency();
    f_max_ = cq_spectrogram_->getMaxFrequency();
    interval_ = round(1.0 * win_size / cq_spectrogram_->getColumnHop())
                * cq_spectrogram_->getColumnHop();

}

QTransform::QTransform(uint32_t samplerate, freq_hz_t f_low, freq_hz_t f_high,
                       uint32_t win_size) : QTransform(BINS_PER_OCTAVE_DEFAULT,
                               samplerate, f_low, f_high, win_size) {}

QTransform::~QTransform()
{
    delete cq_spectrogram_;
}

log_spectrogram_t QTransform::GetSpectrogram(amplitude_t *td, uint32_t td_len,
                                             uint32_t offset, uint32_t hop_size)
{
    CQBase::RealBlock output_block, output;

#if 0
    for (uint32_t sample = offset; sample < td_len; sample += hop_size) {
        uint32_t len = min(win_size, td_len - sample);
        CQBase::RealSequence input(td + sample, td + sample + len);
        output_block = cq_spectrogram_->process(input);

        if (!output_block.empty()) {
            output.insert(output.end(), output_block.begin(), output_block.end());
            output_block.clear();
        }
    }
#else
    UNUSED(offset);
    CQBase::RealSequence input(td, td + td_len);
    output_block = cq_spectrogram_->process(input);
    output.insert(output.end(), output_block.begin(), output_block.end());

    output_block = cq_spectrogram_->getRemainingOutput();
    output.insert(output.end(), output_block.begin(), output_block.end());
#endif /* 0 */

    output.erase(output.begin(), output.begin() + cq_spectrogram_->getLatency() /
                                                  cq_spectrogram_->getColumnHop());

    for (auto &col : output) {
        reverse(col.begin(), col.end());
    }

    return ConvertRealBlock_(output, hop_size);
}

log_spectrogram_t QTransform::ConvertRealBlock_(CQBase::RealBlock &block,
                                                uint32_t hop_size)
{
    if (block.empty()) {
        throw invalid_argument("Empty input block");
    }

    UNUSED(hop_size);

    log_spectrogram_t lsg;
    uint32_t cols_per_window = interval_ / cq_spectrogram_->getColumnHop();

    if (cols_per_window <= 1) {
        return block;
    }

    for (uint32_t i = 0; i < block.size(); i += cols_per_window) {
        uint32_t columns = min(cols_per_window, static_cast<uint32_t>(block.size()) - i);
        vector<amplitude_t> col;

        for (uint32_t j = 0; j < block[i].size(); j++) {
            amplitude_t row_value = 0;
            for (uint32_t c = 0; c < columns; c++) {
                row_value += block[i + c][j];
            }
            col.push_back(row_value);
        }

        lsg.push_back(col);
    }

    return lsg;
}

uint32_t QTransform::SpectrogramInterval()
{
    return interval_;
}

uint8_t QTransform::BinsPerSemitone()
{
    return cq_spectrogram_->getBinsPerOctave() / notes_Total;
}

uint32_t QTransform::FreqToBin(freq_hz_t f)
{
    if ((f < f_min_) || (f > f_max_))
    {
        throw invalid_argument("QTransform::FreqToBin(): f is out of range");
    }

    return (log2(f / f_min_) * cq_spectrogram_->getBinsPerOctave());
 }

freq_hz_t QTransform::BinToFreq(uint32_t idx)
{
    return cq_spectrogram_->getBinFrequency(cq_spectrogram_->getTotalBins() - idx - 1);
}

