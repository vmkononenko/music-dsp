/*
 * pitch_cls_profile.cpp
 *
 * Pitch Class Profile implementation
 */

#include <iomanip>
#include <math.h>

#include "lmhelpers.h"
#include "pitch_calculator.h"
#include "pitch_cls_profile.h"


using namespace std;

namespace anatomist {

PitchClsProfile::PitchClsProfile()
{
    __mPCP.resize(notes_Total);
}

PitchClsProfile::PitchClsProfile(Transform *t)
{
    PitchCalculator& pc = PitchCalculator::getInstance();

    __mPCP.reserve(notes_Total);
    __mPCP.assign(notes_Total, 0);

    for (int n = note_Min; n <= note_Max; n++) {
        note_t note = (note_t)n;

        freq_hz_t pitch_freq;
        uint32_t bin_idx;
        amplitude_t pitch_cls = 0;
        amplitude_t mag = 0;
        for (int o = OCTAVE_MIN; o <= OCTAVE_MAX; o++) {
            octave_t oct = (octave_t)o;

            pitch_freq = pc.noteToPitch(note, oct);
            bin_idx = t->FreqToIdx(pitch_freq, round);

            if (bin_idx >= t->GetFreqDomainLen()) {
                throw runtime_error("fftIdx >= pointsCnt");
            }

            mag = t->GetFreqDomain().p[bin_idx];
            pitch_cls += mag * mag;
            __mPitchClsMax = max(pitch_cls, __mPitchClsMax);
        }

        __mPCP[note - note_Min] = pitch_cls;
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

amplitude_t PitchClsProfile::euclideanDistance(PitchClsProfile &pcp)
{
    amplitude_t d = euclideanDistance<typeof(__mPCP[0])>(pcp.__mPCP);
    return d;
}

amplitude_t PitchClsProfile::divergenceKullbackLeibler(vector<uint8_t> &v)
{
    if (v.size() != __mPCP.size()) {
        throw invalid_argument("divergenceKullbackLeibler(): wrong vector size");
    }

    amplitude_t d = 0;

    for (uint8_t i = 0; i < __mPCP.size(); i++) {
        d += __mPCP[i] * log10(__mPCP[i] / v[i]);
    }

    return d;
}

PitchClsProfile & PitchClsProfile::operator+=(const PitchClsProfile& pcp)
{
    for (uint8_t i = 0; i < __mPCP.size(); i++) {
        __mPCP[i] += pcp.__mPCP[i];
    }

    return *this;
}

PitchClsProfile & PitchClsProfile::operator/=(float denominator)
{
    for (uint8_t i = 0; i < __mPCP.size(); i++) {
        __mPCP[i] /= denominator;
    }

    return *this;
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

}
