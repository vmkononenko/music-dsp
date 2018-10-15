/**
 * @file    beat_detector.cpp
 * @brief   Implementation of the beat detection mechanism
 */

#include <algorithm>

#include "beat_detector.h"
#include "config.h"
#include "fft.h"
#include "lmhelpers.h"

#define BEAT_FREQ_HZ_MIN    1
#define BEAT_FREQ_HZ_MAX    5

using namespace std;


namespace anatomist {

BeatDetector::BeatDetector(amplitude_t *td, uint32_t samples, uint32_t sampleRate)
{
    Envelope *e = new Envelope(td, samples);

    detectBeat(e, sampleRate);

    delete e;
}

BeatDetector::BeatDetector(Envelope *e, uint32_t sampleRate)
{
    detectBeat(e, sampleRate);
}

void BeatDetector::detectBeat(Envelope *env, uint32_t sampleRate)
{
    vector<amplitude_t> env_diff = env->diff();
    uint32_t env_samplerate = sampleRate / env->getDownsampleFactor();
    FFT *fft = new FFT(env_diff.data(), env_diff.size(), env_samplerate, 0,
                       env_samplerate / 2, true, true);
    amplitude_t *env_fd = fft->GetFreqDomain().p;
    uint32_t maxFFTAmpIdx, maxEnvAmpIdx, closestLeftLocalMinIdx;
    freq_hz_t beat_hz;

    maxFFTAmpIdx = max_element(env_fd, env_fd + fft->GetSize() / 2) - env_fd;

    beat_hz = fft->IdxToFreq(maxFFTAmpIdx);

    __mBeatIdxInterval = sampleRate / beat_hz;
    normalizeInterval();

    hz2BPM(beat_hz);

    maxEnvAmpIdx = max_element(env_diff.begin(), env_diff.begin() + env_diff.size()) - env_diff.begin();

    // closest local minimum of the envelope to the left of highest peak
    closestLeftLocalMinIdx = maxEnvAmpIdx - 1;
    while (env_diff[closestLeftLocalMinIdx] > 0) {
      closestLeftLocalMinIdx -= 1;
    }
    closestLeftLocalMinIdx++;

    __mOffset = ((maxEnvAmpIdx - closestLeftLocalMinIdx) >= 2 ?
            (maxEnvAmpIdx - closestLeftLocalMinIdx) / 2 : maxEnvAmpIdx) % __mBeatIdxInterval;

    delete fft;
}

void BeatDetector::normalizeInterval()
{
    while ((__mBeatIdxInterval < CFG_BEAT_INTERVAL_MIN) ||
           (__mBeatIdxInterval > CFG_BEAT_INTERVAL_MAX))
    {
        __mBeatIdxInterval = (__mBeatIdxInterval < CFG_BEAT_INTERVAL_MIN) ?
                             __mBeatIdxInterval * 2 : __mBeatIdxInterval / 2;
    }
}

void BeatDetector::hz2BPM(freq_hz_t beatHz)
{
    while ((beatHz < BEAT_FREQ_HZ_MIN) || (beatHz > BEAT_FREQ_HZ_MAX)) {
        beatHz = (beatHz < BEAT_FREQ_HZ_MIN) ? beatHz * 2 : beatHz / 2;
    }
    __mBPM = beatHz * 60;
}

uint32_t BeatDetector::getOffset()
{
    return __mOffset;
}

uint32_t BeatDetector::getIdxInterval()
{
    return __mBeatIdxInterval;
}

float BeatDetector::getBPM()
{
    return __mBPM;
}

BeatDetector::~BeatDetector() {}

}
