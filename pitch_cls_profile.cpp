/*
 * pitch_cls_profile.cpp
 *
 * Pitch Class Profile implementation
 */

#include "lmhelpers.h"
#include "pitch_calculator.h"
#include "pitch_cls_profile.h"


using namespace std;

PitchClsProfile::PitchClsProfile(amplitude_t *freqDomainMagnitudes,
        uint32_t fftSize, uint32_t sampleRate, uint32_t pointsCnt)
{
    PitchCalculator& pc = PitchCalculator::getInstance();
    amplitude_t magMax = 0;

    __mPCP.reserve(notes_Total);
    __mPCP.assign(notes_Total, 0);

    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;

        freq_hz_t pitchFreq;
        uint32_t fftIdx;
        amplitude_t pitchCls = 0;
        amplitude_t magCur = 0;
        for (int o = OCTAVE_MIN; o <= OCTAVE_MAX; o++) {
            octave_t oct = (octave_t)o;

            pitchFreq = pc.noteToPitch(note, oct);
            // TODO: replace ceil with the proper round function
            fftIdx = Helpers::freqToFftIdx(pitchFreq, fftSize, sampleRate, floor);

            if (fftIdx > pointsCnt) {
                throw runtime_error("fftIdx > pointsCnt");
            }

            magCur = freqDomainMagnitudes[fftIdx];
            pitchCls += magCur;
            magMax = max(magCur, magMax);
        }

        __mPCP[note - note_Min] = pitchCls;
    }

    __normalize(magMax);
}

void PitchClsProfile::__normalize(amplitude_t magMax)
{
    for (uint8_t i = 0; i < __mPCP.size(); i++) {
        __mPCP[i] = __mPCP[i] / magMax;
    }
}

amplitude_t PitchClsProfile::getPitchCls(note_t note)
{
    if ((note < note_Min) || (note > note_Max)) {
        throw std::invalid_argument("Invalid note");
    }

    return __mPCP[note - note_Min];
}
