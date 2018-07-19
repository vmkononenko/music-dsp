/**
 * @file    envelope.cpp
 * @brief   Envelope implementation
 */

#include <iomanip>
#include <math.h>

#include "envelope.h"
#include "cheby1_filter.h"
#include "ma_filter.h"


using namespace std;

Envelope::Envelope(amplitude_t *td, uint32_t samples)
{
    Filter *f_ma = new MAFilter();
    Filter *f_cheby1 = new Cheby1Filter();

    squareAndDouble(td, samples);

    __mEnvelope = f_ma->process(td, samples);
    __mDF *= f_ma->getDownsampleFactor();

    __mEnvelope = f_cheby1->process(__mEnvelope.data(), __mEnvelope.size());
    __mDF *= f_cheby1->getDownsampleFactor();

    //squareRoot(__mEnvelope);

    delete f_ma;
    delete f_cheby1;
}

void Envelope::squareAndDouble(amplitude_t *td, uint32_t samples)
{
    for (uint32_t i = 0; i < samples; i++) {
        td[i] = td[i] * td[i] * 2;
    }
}

void Envelope::squareRoot(vector<amplitude_t> &input)
{
    for(auto & amplitude : input) {
        amplitude = sqrt(amplitude);
    }
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

ostream& operator<<(ostream& os, const Envelope& e)
{
    for(uint32_t i = 0; i < e.__mEnvelope.size(); i++) {
        os << i << "," << e.__mEnvelope[i] << endl;
    }

    return os;
}

Envelope::~Envelope() {}
