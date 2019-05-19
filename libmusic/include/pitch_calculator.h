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
 * @file        pitch_calculator.h
 * @brief       PitchCalculator APIs
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <map>
#include <vector>

#include "lmtypes.h"

#define FREQ_INVALID            ((freq_hz_t) -1)
#define IS_FREQ_VALID(f)        ((f) > 0)
#define SEMITONES_A0_TO_A4      -48 /** @TODO define proper values and notes */
#define SEMITONES_A4_TO_C8      200 /** @TODO define proper values and notes */
#define SEMITONES_TOTAL         (SEMITONES_A4_TO_C8 - SEMITONES_A0_TO_A4)
#define FREQ_A4                 ((freq_hz_t) 440)

#ifndef PITCH_CALCULATOR_TEST_FRIENDS
#define PITCH_CALCULATOR_TEST_FRIENDS
#endif


class PitchCalculator {

PITCH_CALCULATOR_TEST_FRIENDS;

private:
    freq_hz_t                   *__mPitches;
    int16_t                     __mPitchIdxA4;
    std::map<int8_t, note_t>    __mNotesFromA4;
    std::map<note_t, int8_t>    __mSemitonesFromA4;

    /**
     * Constructor
     */
    PitchCalculator();

    /* We want to make sure they are unacceptable otherwise we may accidentally
     * get copies of singleton appearing */
    PitchCalculator(PitchCalculator const&);    // Don't Implement
    void operator=(PitchCalculator const&);     // Don't implement

    void __initPitches();

    /**
     * @return true if freq matches one of the pitch frequencies
     */
    bool __isPitch(freq_hz_t freq);

    /**
     * Find frequency with the highest amplitude
     *
     * @param   freqDomain  frequency domain magnitudes
     * @param   len         number of points in freqDomain
     * @param   fftSize     taken FFT length
     * @param   sampleRate  sample rate of the analyzed signal
     * @return  frequency value
     */
    freq_hz_t __getTonic(amplitude_t *freqDomain, uint32_t len, uint32_t fftSize,
                         uint32_t sampleRate);

    /**
     * Find sequential number of the pitch in __mPitches
     *
     * @param   freq    pitch frequency
     * @return  index in __mPitches array
     */
    int16_t __getPitchIdx(freq_hz_t freq);

public:
    static PitchCalculator& getInstance()
    {
        static PitchCalculator instance;

        return instance;
    }

    /**
     * Destructor
     */
    ~PitchCalculator();

    /**
     * Detect pitch from the corresponding frequency domain points
     *
     * @param   freqDomain  frequency domain magnitudes
     * @param   len         number of points in freqDomain
     * @param   fftSize     taken FFT length
     * @param   sampleRate  sample rate of the analyzed signal
     * @return  detected pitch frequency
     */
    freq_hz_t getPitch(freq_hz_t freq);

    /**
     * Find note corresponding to the given pitch
     *
     * @param   pitch frequency in Hz
     * @return  note corresponding to the given pitch
     */
    note_t pitchToNote(freq_hz_t freq);

    /**
     * Find pitch corresponding to the note
     *
     * @param   note    note to get pitch for
     * @param   octave  octave to get pitch for
     * @return  pitch frequency
     */
    freq_hz_t noteToPitch(note_t note, octave_t octave);

    /**
     * Find octave corresponding to the given pitch
     *
     * @param   pitch frequency in Hz
     * @return  octave number corresponding to the given pitch
     */
    note_t pitchToOctave(freq_hz_t freq);

    /**
     * Calculate pitch frequency at n semitones interval from given pitch
     *
     * @param   pitch   base frequency to calculate from
     * @param   n       number of semitones from pitch
     * @return corresponding calculated pitch frequency
     */
    freq_hz_t getPitchByInterval(freq_hz_t pitch, int16_t n);

    /**
     * Calculate pitch frequency at i bins interval from given frequency
     */
    freq_hz_t getFreqByInterval(freq_hz_t f, float i);

    /**
     * Interval between two frequencies measured in octaves
     *
     * @param   f1, f2  frequencies between which measurement is taken
     * @return   distance in octaves
     */
    static double octavesDistance(freq_hz_t f1, freq_hz_t f2);

    /**
     * Interval between two frequencies measured in semitones
     *
     * @param   f1, f2  frequencies between which measurement is taken
     * @return   distance in semitones
     */
    static int32_t semitonesDistance(freq_hz_t f1, freq_hz_t f2);
};

/** @} */
