/*
 * window_functions.cpp
 *
 * Implementations of window functions
 */

#include "config.h"
#include "lmhelpers.h"
#include "window_functions.h"


using namespace std;

vector<amplitude_t> WindowFunctions::getHamming(uint32_t len, uint32_t offset)
{
    if (offset >= len) {
        throw invalid_argument("getHamming(): offset >= len");
    }

    vector<amplitude_t> win(len);

    for (uint32_t i = offset; i < len; i++) {
        win[i] = (0.54 - 0.46 * cos(2 * M_PI * i / (len - 1)));
    }

    return win;
}

void WindowFunctions::applyHamming(td_t td)
{
    vector<amplitude_t> win = getHamming(td.size(), 0);

    for (uint32_t i = 0; i < td.size(); i++) {
        td[i] *= win[i];
    }
}

void WindowFunctions::applyBlackman(td_t td)
{
    for (uint32_t i = 0; i < td.size(); i++) {
        td[i] *= (0.42 - 0.5 * cos(2 * M_PI * i / (td.size() - 1)) +
                    0.08 * cos(4 * M_PI * i / (td.size() - 1)));
    }
}

void WindowFunctions::applyHann(td_t td)
{
    for (uint32_t i = 0; i < td.size(); i++) {
        td[i] *= (0.5 * (1 - cos(2 * M_PI * i / (td.size() - 1))));
    }
}

void WindowFunctions::applyDefault(td_t td)
{
#if !defined(CFG_WINDOW_FUNC) || (CFG_WINDOW_FUNC == WINDOW_FUNC_RECTANGULAR)
    /* rectangular is a default. Do nothing for it */
    UNUSED(td);
#elif CFG_WINDOW_FUNC == WINDOW_FUNC_BLACKMAN
    applyBlackman(td);
#elif CFG_WINDOW_FUNC == WINDOW_FUNC_HAMMING
    applyHamming(td);
#elif CFG_WINDOW_FUNC == WINDOW_FUNC_HANN
    applyHann(td);
#endif
}

const char * WindowFunctions::toString(uint32_t fcode)
{
    if ((fcode < WINDOW_FUNC_MIN) || (fcode > WINDOW_FUNC_MAX)) {
        throw std::invalid_argument("Invalid window function");
    }

    const char *ret;

    switch(fcode) {
        case WINDOW_FUNC_RECTANGULAR:
            ret = "Rectangular";
            break;
        case WINDOW_FUNC_BLACKMAN:
            ret = "Blackman";
            break;
        case WINDOW_FUNC_HAMMING:
            ret = "Hamming";
            break;
        case WINDOW_FUNC_HANN:
            ret = "Hann";
            break;
        default:
            ret = "unknown";
            break;
    }

    return ret;
}
