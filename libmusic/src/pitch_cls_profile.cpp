/*
 * pitch_cls_profile.cpp
 *
 * Pitch Class Profile implementation
 */

#include <iomanip>

#include "lmhelpers.h"
#include "pitch_calculator.h"
#include "pitch_cls_profile.h"


using namespace std;

PitchClsProfile::PitchClsProfile(amplitude_t *freqDomainMagnitudes,
        uint32_t fftSize, uint32_t sampleRate, uint32_t pointsCnt)
{
    PitchCalculator& pc = PitchCalculator::getInstance();

    __mPCP.reserve(notes_Total);
    __mPCP.assign(notes_Total, 0);

    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;

        freq_hz_t pitchFreq;
        uint32_t fftIdx;
        amplitude_t pitchCls = 0;
        amplitude_t mag = 0;
        for (int o = OCTAVE_MIN; o <= OCTAVE_MAX; o++) {
            octave_t oct = (octave_t)o;

            pitchFreq = pc.noteToPitch(note, oct);
            fftIdx = Helpers::freqToFftIdx(pitchFreq, fftSize, sampleRate, round);

            if (fftIdx > pointsCnt) {
                throw runtime_error("fftIdx > pointsCnt");
            }

            mag = freqDomainMagnitudes[fftIdx];
            pitchCls += mag * mag;
            __mPitchClsMax = max(pitchCls, __mPitchClsMax);
        }

        __mPCP[note - note_Min] = pitchCls;
    }
    __normalize();
}

void PitchClsProfile::__normalize()
{
    for (uint8_t i = 0; i < __mPCP.size(); i++) {
        __mPCP[i] = __mPCP[i] / __mPitchClsMax;
    }
}

amplitude_t PitchClsProfile::getPitchCls(note_t note) const
{
    if ((note < note_Min) || (note > note_Max)) {
        throw std::invalid_argument("Invalid note");
    }

    return __mPCP[note - note_Min];
}

#define PCP_ROWS            10
#define PCP_SYM_PER_COL     3

ostream& operator<<(ostream& os, const PitchClsProfile& pcp)
{
    int percentPerRow = 100 / PCP_ROWS;

    for (int row = 0; row < PCP_ROWS; row++) {
        for (int n = note_Min; n <= note_Max; n++) {
            note_t note = static_cast<note_t>(n);
            uint8_t progress = pcp.getPitchCls(note) * 100;
            os << std::setw(PCP_SYM_PER_COL)
               << ((progress >= round(100 - percentPerRow * row)) ? '|' : ' ');
        }
        os << endl;
    }

    for (int n = note_Min; n <= note_Max; n++) {
        os << std::setw(PCP_SYM_PER_COL) << static_cast<note_t>(n);
    }

    return os;
}
