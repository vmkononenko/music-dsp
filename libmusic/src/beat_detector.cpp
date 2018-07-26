/**
 * @file    beat_detector.cpp
 * @brief   Implementation of the beat detection mechanism
 */

#include <algorithm>

#include "beat_detector.h"
#include "config.h"
#include "envelope.h"
#include "fft.h"
#include "lmhelpers.h"

#define BEAT_FREQ_HZ_MIN    1
#define BEAT_FREQ_HZ_MAX    5

using namespace std;


BeatDetector::BeatDetector(amplitude_t *td, uint32_t samples, uint32_t sampleRate)
{
    detectBeat(td, samples, sampleRate);
}

void BeatDetector::detectBeat(amplitude_t *td, uint32_t samples, uint32_t sampleRate)
{
    Envelope *env = new Envelope(td, samples);
    vector<amplitude_t> envDiff = env->diff();
    uint32_t envSampleRate = sampleRate / env->getDownsampleFactor();
    FFT *fft = new FFT();
    uint32_t fftSize = Helpers::nextPowerOf2(envDiff.size());
    vector<complex_t> envDiffComplex = Helpers::timeDomain2ComplexVector(
            envDiff.data(), envDiff.size(), fftSize);
    amplitude_t *envFreqDomain = (amplitude_t*) malloc(fftSize / 2 * sizeof(amplitude_t));
    uint32_t maxFFTAmpIdx, maxEnvAmpIdx, closestLeftLocalMinIdx;
    freq_hz_t beatHz;

    fft->forward(envDiffComplex);
    fft->toPolar(envDiffComplex, envFreqDomain, fftSize / 2);
    fft->toHPS(envFreqDomain, fftSize / 2);

    maxFFTAmpIdx = max_element(envFreqDomain, envFreqDomain + fftSize / 2) - envFreqDomain;

    beatHz = Helpers::fftIdxToFreq(maxFFTAmpIdx, fftSize / 2, envSampleRate);

    __mBeatIdxInterval = sampleRate / beatHz;
    normalizeInterval();

    hz2BPM(beatHz);

    maxEnvAmpIdx = max_element(envDiff.begin(), envDiff.begin() + envDiff.size()) - envDiff.begin();

    // closest local minimum of the envelope to the left of highest peak
    closestLeftLocalMinIdx = maxEnvAmpIdx - 1;
    while (envDiff[closestLeftLocalMinIdx] > 0) {
      closestLeftLocalMinIdx -= 1;
    }
    closestLeftLocalMinIdx++;

    __mOffset = ((maxEnvAmpIdx - closestLeftLocalMinIdx) >= 2 ?
            (maxEnvAmpIdx - closestLeftLocalMinIdx) / 2 : maxEnvAmpIdx) % __mBeatIdxInterval;

    delete fft;
    delete env;
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
