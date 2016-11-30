/*
 * chord_detector.h
 *
 *  Chord recognition API
 */

#pragma once

#include <stdint.h>
#include <vector>

#include "fft.h"
#include "lmtypes.h"
#include "music_scale.h"
#include "pitch_calculator.h"

#ifndef TEST_FRIENDS
#define TEST_FRIENDS
#endif


typedef struct Chord {
    note_t      mainNote;
    octave_t    octave;
    bool        isMinor;

    Chord() : mainNote(note_Unknown) {}
} chord_t;


class ChordDetector {

TEST_FRIENDS;

private:
    FFT *__mFft;
    PitchCalculator *__mPitchCalculator;
    std::vector<MusicScale *> __mScales;

    /**
     *  Attenuate frequencies lower than freq to 0
     *
     *  Attenuate instead of cutting of the range to be able to do conversion
     *  between frequencies and indexes of FFT points
     *
     *  @param freqDomain   input frequency domain
     *  @param len          number of points in freqDomain
     *  @param freq         frequency to cut from
     *  @param fftSize      taken FFT length
     *  @param sampleRate   sample rate of the analyzed signal
     */
    void __attLowFreq(amplitude_t *, uint32_t, freq_hz_t, uint32_t, uint32_t);

    /**
     * Convert frequency to point index in frequency domain to cut off from
     *
     * Calculated index is then used to cut off unneeded high frequency range
     * from the input frequency domain
     *
     * @param freq         frequency to cut from
     * @param sampleRate   sample rate of the analyzed signal
     * @param fftSize      taken FFT length
     */
    uint32_t __cutoffHighIdx(freq_hz_t freq, uint32_t sampleRate, uint32_t fftSize);

    /**
     * Initialize __mScales
     */
    void __initScales();

public:
    /**
     * Constructor
     */
    ChordDetector();

    /**
     * Destructor
     */
    ~ChordDetector();

    /**
     * Detect chord corresponding to the given time domain
     *
     * @param   x           time domain data
     * @param   samples     number of samples in x
     * @param   sampleRate  sample rate of x
     * @return  detected chord
     */
    chord_t getChord(amplitude_t *, uint32_t, uint32_t);

    /**
     * Build major or minor scale from the main note
     *
     * @param   mainNote    main note to build the scale from
     * @param   isMinor     specifies major or minor scale
     * @return  requested scale
     */
    std::vector<note_t> getScale(note_t, bool);
};
