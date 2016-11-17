/*
 * window_functions.cpp
 *
 * Implementations of window functions
 */

#include "window_functions.h"


void WindowFunctions::applyHamming(amplitude_t *points, uint32_t pointsCnt)
{
    for (uint32_t i = 0; i < pointsCnt; i++) {
        points[i] = points[i] * (0.54 - 0.46 * cos(2 * M_PI * i / pointsCnt));
    }
}

void WindowFunctions::applyBlackman(amplitude_t *points, uint32_t pointsCnt)
{
    for (uint32_t i = 0; i < pointsCnt; i++) {
        points[i] = points[i] * (0.42 - 0.5 * cos(2 * M_PI * i / pointsCnt) +
                                 0.08 * cos(4 * M_PI * i / pointsCnt));
    }
}
