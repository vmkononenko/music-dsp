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

/*
 * pitch_calculator.cpp
 *
 * Implementation of various pitch calculation functions
 */

#include "config.h"
#include "pitch_calculator.h"

#include <cmath>
#include <stdexcept>

#include "lmtypes.h"
#include "lmhelpers.h"

#define FREQ_PRECISION          4
#define OCTAVES_CNT             9   /* from 0 to 8 */
#define SEMITONES_PER_OCTAVE    ((int32_t)12)
#define IS_PITCH_IDX_VALID(idx) (((idx) >= 0) && ((idx) < SEMITONES_TOTAL))


PitchCalculator::PitchCalculator()
{
    __mNotesFromA4[0] = note_A;
    __mNotesFromA4[1] = note_A_sharp;
    __mNotesFromA4[2] = note_B;
    __mNotesFromA4[3] = note_C;
    __mNotesFromA4[4] = note_C_sharp;
    __mNotesFromA4[5] = note_D;
    __mNotesFromA4[6] = note_D_sharp;
    __mNotesFromA4[7] = note_E;
    __mNotesFromA4[8] = note_F;
    __mNotesFromA4[9] = note_F_sharp;
    __mNotesFromA4[10] = note_G;
    __mNotesFromA4[11] = note_G_sharp;

    __mSemitonesFromA4[note_C]       = -9;
    __mSemitonesFromA4[note_C_sharp] = -8;
    __mSemitonesFromA4[note_D]       = -7;
    __mSemitonesFromA4[note_D_sharp] = -6;
    __mSemitonesFromA4[note_E]       = -5;
    __mSemitonesFromA4[note_F]       = -4;
    __mSemitonesFromA4[note_F_sharp] = -3;
    __mSemitonesFromA4[note_G]       = -2;
    __mSemitonesFromA4[note_G_sharp] = -1;
    __mSemitonesFromA4[note_A]       = -0;
    __mSemitonesFromA4[note_A_sharp] = 1;
    __mSemitonesFromA4[note_B]       = 2;

    __initPitches();
}

PitchCalculator::~PitchCalculator()
{
    delete[] __mPitches;
}

void PitchCalculator::__initPitches()
{
    __mPitches = new freq_hz_t[SEMITONES_TOTAL];

    for (int32_t i = 0; i < SEMITONES_TOTAL; i++) {
        double n = i + SEMITONES_A0_TO_A4;
        __mPitches[i] = pow(2, (n / SEMITONES_PER_OCTAVE)) * FREQ_A4;
        __mPitches[i] = Helpers::stdRound<freq_hz_t>(__mPitches[i], FREQ_PRECISION);
        if(__mPitches[i] == FREQ_A4) {
            __mPitchIdxA4 = i;
        }
    }

    /** @TODO add assert if __mPitchIdxA4 == 0 */
}

freq_hz_t PitchCalculator::getPitchByInterval(freq_hz_t pitch, int16_t n)
{
    int16_t pitchIdx = __getPitchIdx(pitch);
    int16_t retIdx;

    if (pitchIdx < 0) {
        return FREQ_INVALID;
    }

    retIdx = pitchIdx + n;

    return (IS_PITCH_IDX_VALID(retIdx) ? __mPitches[retIdx]  : FREQ_INVALID);
}

freq_hz_t PitchCalculator::getFreqByInterval(freq_hz_t f, float i)
{
    return pow(2, i) * f;
}

int16_t PitchCalculator::__getPitchIdx(freq_hz_t freq)
{
    int16_t start = 0, end = SEMITONES_TOTAL - 1, mid;
    int16_t idx = -1;

    freq = Helpers::stdRound(freq, FREQ_PRECISION);

    while (start <= end) {
        mid = start + (end - start) / 2;

        if (__mPitches[mid] < freq) {
            start = mid + 1;
        } else if (__mPitches[mid] > freq) {
            end = mid - 1;
        } else {
            idx = mid;
            break;
        }
    }

    return idx;
}

bool PitchCalculator::__isPitch(freq_hz_t freq)
{
    return (__getPitchIdx(freq) >= 0);
}

/** @TODO reuse delta mechanism used in getPitch */
freq_hz_t PitchCalculator::__getTonic(amplitude_t *freqDomain, uint32_t len,
                                    uint32_t fftSize, uint32_t sampleRate)
{
    if (freqDomain == NULL || sampleRate == 0) {
        throw std::invalid_argument("Invalid argument");
    }

    amplitude_t max = freqDomain[0];
    amplitude_t mag;
    uint32_t i = 0, maxIndex = 0;

    /** @TODO replace with faster algorithm than the one with linear complexity */
    while (i < len) {
        mag = freqDomain[i];
        if (mag > max) {
            max = mag;
            maxIndex = i;
        }
        i++;
    }

    return (maxIndex * sampleRate / fftSize);
}

freq_hz_t PitchCalculator::getPitch(freq_hz_t freq)
{
    freq_hz_t freqPitch = FREQ_INVALID; // closest pitch matching freq
    double deltaRight, deltaLeft, deltaMid;
    int16_t start = 0, end = SEMITONES_TOTAL - 1, mid;

    if (__isPitch(freq)) {
        freqPitch = freq;
        goto ret;
    }

    while (start <= end) {
        mid = start + (end - start) / 2;

        if (!IS_PITCH_IDX_VALID(mid) || !IS_PITCH_IDX_VALID(start) ||
            !IS_PITCH_IDX_VALID(end))
        {
            goto ret;
        }

        deltaMid = fabs(octavesDistance(__mPitches[mid], freq));
        deltaLeft = fabs(octavesDistance(__mPitches[mid - 1], freq));
        deltaRight = fabs(octavesDistance(__mPitches[mid + 1], freq));

        /** @TODO check the case - when all values in __mPitches are 0 this
                  loop becomes infinite */

        if (deltaLeft < deltaMid) {
            end = mid - 1;
            freqPitch = __mPitches[end];
        } else if (deltaRight < deltaMid) {
            start = mid + 1;
            freqPitch = __mPitches[start];
        } else if ((deltaLeft == deltaMid) || (deltaRight == deltaMid)) {
            freqPitch = FREQ_INVALID;
            break;
        } else if ((deltaMid < deltaLeft) && (deltaMid < deltaRight)) {
            if (deltaMid <= CFG_PITCH_PRECISION_THRESHOLD) {
                freqPitch = __mPitches[mid];
            } else {
                freqPitch = FREQ_INVALID;
            }
            break;
        }
    }

 ret:
    return freqPitch;
}

note_t PitchCalculator::pitchToNote(freq_hz_t freq)
{
    if (!__isPitch(freq)) {
        throw std::invalid_argument("Invalid frequency - pitch is expected");
    }

    int32_t semitonesFromA4 = (semitonesDistance(freq, FREQ_A4) % SEMITONES_PER_OCTAVE +
            SEMITONES_PER_OCTAVE) % SEMITONES_PER_OCTAVE;

    return (__mNotesFromA4.find(semitonesFromA4) != __mNotesFromA4.end() ?
            __mNotesFromA4[semitonesFromA4] : note_Unknown);
}

freq_hz_t PitchCalculator::noteToPitch(note_t note, octave_t octave)
{
    if ((note < note_Min) || (note > note_Max)) {
        throw std::invalid_argument("Invalid note");
    }
    if ((octave < OCTAVE_MIN) || (octave > OCTAVE_MAX)) {
        throw std::invalid_argument("Invalid octave");
    }

    int16_t semitonesFromA4 = (__mSemitonesFromA4[note] +
                               (octave - OCTAVE_4) * SEMITONES_PER_OCTAVE);
    int16_t idx = __mPitchIdxA4 + semitonesFromA4;
    freq_hz_t ret;

    if (!IS_PITCH_IDX_VALID(idx)) {
        ret = FREQ_INVALID;
    } else {
        ret = __mPitches[idx];
    }

    return ret;
}

double PitchCalculator::octavesDistance(freq_hz_t f1, freq_hz_t f2)
{
    if (!IS_FREQ_VALID(f1) || !IS_FREQ_VALID(f2)) {
        throw std::invalid_argument("Invalid frequency");
    }

    return log2(f1 / f2);
}

int32_t PitchCalculator::semitonesDistance(freq_hz_t f1, freq_hz_t f2)
{
    return (int32_t)Helpers::stdRound(SEMITONES_PER_OCTAVE * octavesDistance(f1, f2), 0);
}
