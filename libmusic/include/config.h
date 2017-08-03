/*
 * config.h
 *
 * Configuration parameters for the library
 */

#pragma once

#ifndef CFG_WINDOW_SIZE
#define CFG_WINDOW_SIZE     ((uint32_t)4096)
#endif /* CFG_WINDOW_SIZE */

#ifndef CFG_FFT_SIZE
#define CFG_FFT_SIZE        ((uint32_t)16384)
#endif /* CFG_FFT_SIZE */

#ifndef CFG_WINDOW_FUNC
#define CFG_WINDOW_FUNC WINDOW_FUNC_RECTANGULAR
#endif /* CFG_WINDOW_FUNC */

#ifndef CFG_PITCH_PRECISION_THRESHOLD
/* Determines pitch detection precision measured in octaves. If validated
 * frequency does not fulfill precision requirements, it is treated as
 * invalid (not reliable value) */
#define CFG_PITCH_PRECISION_THRESHOLD   ((double)0.35)
#endif /* CFG_PITCH_PRECISION_THRESHOLD */
