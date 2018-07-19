/**
 * cheby1_filter.cpp
 *
 * Chebyshev type 1 filter implementation
 */

#include "cheby1_filter.h"


using namespace std;

Cheby1Filter::Cheby1Filter() {}

std::vector<amplitude_t> Cheby1Filter::leftToRight(amplitude_t *x, uint32_t samples)
{
    vector<amplitude_t> y(samples);

    y[0] = b0 * x[0];
    y[1] = b0 * x[1] + b1 * x[0] - a1 * y[0];
    y[2] = b0 * x[2] + b1 * x[1] + b2 * x[0] - a1 * y[1] - a2 * y[0];
    y[3] = b0 * x[3] + b1 * x[2] + b2 * x[1] + b3 * x[0] - a1 * y[2] - a2 * y[1] - a3 * y[0];

    for (uint32_t n = CHEBYSHEV_POLES; n < samples; n++) {
        y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] + b3 * x[n-3] + b4 * x[n-4]
             - a1 * y[n-1] - a2 * y[n-2] - a3 * y[n-3] - a4 * y[n-4];
    }

    return y;
}

std::vector<amplitude_t> Cheby1Filter::rightToLeft(amplitude_t *x, uint32_t c)
{
    vector<amplitude_t> y(c);

    y[c-1] = b0 * x[c-1];
    y[c-2] = b0 * x[c-2] + b1 * x[c-1] - a1 * y[c-1];
    y[c-3] = b0 * x[c-3] + b1 * x[c-2] + b2 * x[c-1] - a1 * y[c-2] - a2 * y[c-1];
    y[c-4] = b0 * x[c-4] + b1 * x[c-3] + b2 * x[c-2] + b3 * x[c-1] - a1 * y[c-3] - a2 * y[c-2] - a3 * y[c-1];

    for (int32_t n = (int32_t)(c - CHEBYSHEV_POLES - 1); n >= 0; n--) {
        y[n] = b0 * x[n] + b1 * x[n+1] + b2 * x[n+2] + b3 * x[n+3] + b4 * x[n+4]
             - a1 * y[n+1] - a2 * y[n+2] - a3 * y[n+3] - a4 * y[n+4];
    }

    return y;
}

vector<amplitude_t> Cheby1Filter::process(amplitude_t *td, uint32_t samples)
{
    vector<amplitude_t> l2r = leftToRight(td, samples);
    vector<amplitude_t> r2l = rightToLeft(l2r.data(), l2r.size());

    return r2l;
}

Cheby1Filter::~Cheby1Filter() {}
