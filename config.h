/*
 * config.h
 *
 * Configuration parameters for the library
 */

#pragma once

#define CFG_WINDOW_SIZE     ((uint32_t)4096)
#define CFG_FFT_SIZE        ((uint32_t)16384)

#define CFG_WINDOW_FUNC_BLACKMAN        0
#define CFG_WINDOW_FUNC_HAMMING         1
#define CFG_WINDOW_FUNC_HANN            2
#define CFG_WINDOW_FUNC_RECTANGULAR     3
#define CFG_WINDOW_FUNC CFG_WINDOW_FUNC_RECTANGULAR

/* Determines pitch detection precision measured in octaves. If validated
 * frequency does not fulfill precision requirements, it is treated as
 * invalid (not reliable value) */
#define CFG_PITCH_PRECISION_THRESHOLD   ((double)0.35)
