/**
 * ma_filter.cpp
 *
 * Moving average filter implementation
 */

#include "config.h"
#include "ma_filter.h"

using namespace std;

MAFilter::MAFilter() : MAFilter(CFG_MA_FILT_DF_DEFAULT) {}

MAFilter::MAFilter(uint16_t df)
{
    __mDF = df;
}

vector<amplitude_t> MAFilter::process(amplitude_t *td, uint32_t samples)
{
    vector<amplitude_t> ret;
    amplitude_t tmp;
    uint32_t i, j;

    for(i = 0; i < samples; i += __mDF) {
        tmp = 0;
        for (j = 0; ((j < __mDF) && (i + j < samples)); j++) {
            tmp += td[i + j];
        }
        ret.push_back(tmp / (j + 1));
    }

    return ret;
}

uint16_t MAFilter::getDownsampleFactor()
{
    return __mDF;
}

MAFilter::~MAFilter() {}
