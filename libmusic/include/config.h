/*
 * Copyright 2019 Volodymyr Kononenko
 *
 * This file is part of Music-DSP.
 *
 * Music-DSP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Music-DSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Music-DSP. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file        config.h
 * @brief       Configuration parameters for the library
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#ifndef CFG_DYNAMIC_WINDOW
//#define CFG_DYNAMIC_WINDOW
#endif

#ifndef CFG_TFT_TYPE
#define CFG_TFT_TYPE TFT_TYPE_CONSTANTQ
#endif /* CFG_TFT_TYPE */

#ifndef CFG_USE_HMM_TPLS
#define CFG_USE_HMM_TPLS 0
#endif /* CFG_USE_HMM_TPLS */

#ifndef CFG_WINDOW_SIZE
#define CFG_WINDOW_SIZE     ((uint32_t)4096)
#endif /* CFG_WINDOW_SIZE */

#ifndef CFG_HOPS_PER_WINDOW
#define CFG_HOPS_PER_WINDOW 1
#endif /* CFG_HOPS_PER_WINDOW */

#ifndef CFG_FFT_SIZE
#define CFG_FFT_SIZE        ((uint32_t)8192)
#endif /* CFG_FFT_SIZE */

/**
 * @brief set to 1 to avoid averaging
 */
#ifndef CFG_FFT_AVG_WINDOW
#define CFG_FFT_AVG_WINDOW 2
#endif /* CFG_FFT_AVG_WINDOW */

#ifndef CFG_FFT_HPS_HARMONICS
#define CFG_FFT_HPS_HARMONICS   3
#endif /* CFG_FFT_HPS_HARMONICS */

#ifndef CFG_WINDOW_FUNC
#define CFG_WINDOW_FUNC WINDOW_FUNC_RECTANGULAR
#endif /* CFG_WINDOW_FUNC */

#ifndef CFG_PITCH_PRECISION_THRESHOLD
/** @brief Determines pitch detection precision measured in octaves
 * If validated frequency does not fulfill precision requirements
 * it is treated as invalid (not reliable value)
 */
#define CFG_PITCH_PRECISION_THRESHOLD   ((double)0.35)
#endif /* CFG_PITCH_PRECISION_THRESHOLD */

#ifndef CFG_MA_FILT_DF_DEFAULT
/**
 * @brief Default downsampling factor for the MAFilter
 */
#define CFG_MA_FILT_DF_DEFAULT    15
#endif /* CFG_MA_FILT_DF_DEFAULT */

#ifndef CFG_BEAT_INTERVAL_MIN
#define CFG_BEAT_INTERVAL_MIN 4096
#endif /* CFG_BEAT_INTERVAL_MIN */

#ifndef CFG_BEAT_INTERVAL_MAX
#define CFG_BEAT_INTERVAL_MAX 8192
#endif /* CFG_BEAT_INTERVAL_MAX */

#ifndef CFG_SILENCE_THRESHOLD_DB
#define CFG_SILENCE_THRESHOLD_DB -34
#endif /* CFG_SILENCE_THRESHOLD_DB */

#ifndef CFG_PCP_CHANGE_THRESHOLD
#define CFG_PCP_CHANGE_THRESHOLD    1.4f
#endif

/**
 * Special value 0 means that self-transition probability (e.g. C -> C) is equal
 * to transition probability to any other chord
 */
#ifndef CFG_CHORD_SELF_TRANSITION_P
#define CFG_CHORD_SELF_TRANSITION_P 0.1f
#endif /* CFG_CHORD_SELF_TRANSITION_P */

#ifndef CFG_HARTE_SYNTAX
#define CFG_HARTE_SYNTAX 1
#endif /* CFG_HARTE_SYNTAX */

/** @} */
