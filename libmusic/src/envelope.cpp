/**
 * @file    envelope.cpp
 * @brief   Envelope implementation
 */

#include <algorithm>
#include <iomanip>
#include <math.h>
#include <string.h>

#include "butterworth_filter.h"
#include "cheby1_filter.h"
#include "config.h"
#include "envelope.h"
#include "ma_filter.h"


using namespace std;

Envelope::Envelope(const amplitude_t *td, uint32_t samples)
{
    Filter *f_ma = new MAFilter();
    Filter *f_bw = new ButterworthFilter();
    amplitude_t *tdCopy = (amplitude_t *) malloc(samples * sizeof(amplitude_t));

    memcpy(tdCopy, td, samples * sizeof(amplitude_t));

    squareAndDouble(tdCopy, samples);

    __mEnvelope = f_ma->process(tdCopy, samples);
    __mDF *= f_ma->getDownsampleFactor();

    __mEnvelope = f_bw->process(__mEnvelope.data(), __mEnvelope.size());
    __mDF *= f_bw->getDownsampleFactor();

    squareRoot(__mEnvelope);

    __mMaxAmplitude = max();

    free(tdCopy);
    delete f_ma;
    delete f_bw;
}

void Envelope::squareAndDouble(amplitude_t *td, uint32_t samples)
{
    for (uint32_t i = 0; i < samples; i++) {
        td[i] = td[i] * td[i] * 2;
    }
}

void Envelope::squareRoot(vector<amplitude_t> &input)
{
    for (uint32_t i = 0; i < input.size(); i++) {
        complex_t val(input[i], 0);

        val = sqrt(val);

        input[i] = sqrt(pow(real(val) ,2) + pow(imag(val), 2));
    }
}

amplitude_t Envelope::max()
{
    return *max_element(__mEnvelope.begin(), __mEnvelope.end());
}

amplitude_t Envelope::mean(uint32_t startIdx, uint32_t endIdx)
{
    amplitude_t res = 0;

    for (uint32_t i = startIdx; i <= endIdx; i++) {
        res += __mEnvelope[i];
    }

    return res / (endIdx - startIdx + 1);
}

vector<amplitude_t> Envelope::diff()
{
    vector<amplitude_t> res(__mEnvelope.size());

    res[0] = 0;

    for (uint32_t i = 1; i < __mEnvelope.size(); i++) {
        res[i] = __mEnvelope[i] - __mEnvelope[i-1];
    }

    return res;
}

uint16_t Envelope::getDownsampleFactor()
{
    return __mDF;
}

bool Envelope::isSilence(uint32_t startIdx, uint32_t endIdx)
{
    uint32_t start = round(startIdx / __mDF);
    uint32_t end = floor(endIdx / __mDF);

    if ((start > end) || (end > __mEnvelope.size() - 1)) {
        throw invalid_argument("Envelope::mean() invalid argument");
    }

    for (uint32_t i = start; i <= end; i++) {
        if (20 * log10(__mEnvelope[i] / __mMaxAmplitude) < CFG_SILENCE_THRESHOLD_DB) {
            return true;
        }
    }

    return false;
}

ostream& operator<<(ostream& os, const Envelope& e)
{
    for(uint32_t i = 0; i < e.__mEnvelope.size(); i++) {
        os << i << "," << e.__mEnvelope[i] << endl;
    }

    return os;
}

Envelope::~Envelope() {}
