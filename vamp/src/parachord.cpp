/*
 * Copyright 2020 Volodymyr Kononenko
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

#include "parachord.h"
#include "config.h"

using namespace std;
using Vamp::RealTime;

Parachord::Parachord(float inputSampleRate) :
    Plugin(inputSampleRate),
    m_stepSize(0),
    m_blockSize(0)
{
}

Parachord::~Parachord()
{
}

string
Parachord::getIdentifier() const
{
    return "parachord";
}

string
Parachord::getName() const
{
    return "Parachord";
}

string
Parachord::getDescription() const
{
    return "VAMP interface for the Music-DSP library";
}

string
Parachord::getMaker() const
{
    return "Volodymyr Kononenko";
}

int
Parachord::getPluginVersion() const
{
    return 1;
}

string
Parachord::getCopyright() const
{
    return "GNU Lesser General Public License version 3 (LGPLv3)";
}

Parachord::InputDomain
Parachord::getInputDomain() const
{
    return TimeDomain;
}

size_t
Parachord::getPreferredBlockSize() const
{
    return CFG_WINDOW_SIZE;
}

size_t
Parachord::getPreferredStepSize() const
{
    return CFG_WINDOW_SIZE;
}

size_t
Parachord::getMinChannelCount() const
{
    return 1;
}

size_t
Parachord::getMaxChannelCount() const
{
    return 1;
}

Parachord::ParameterList
Parachord::getParameterDescriptors() const
{
    return ParameterList();
}

float
Parachord::getParameter(string identifier) const
{
    (void) identifier;
    return 0;
}

void
Parachord::setParameter(string identifier, float value)
{
    (void) identifier;
    (void) value;
}

Parachord::OutputList
Parachord::getOutputDescriptors() const
{
    OutputList list;

    OutputDescriptor d;
    d.identifier = "chordinfo";
    d.name = "Chord Information";
    d.description = "Estimated chord times and labels.";
    d.unit = "";
    d.hasFixedBinCount = true;
    d.binCount = 0;
    d.hasKnownExtents = false;
    d.isQuantized = false;
    d.sampleType = OutputDescriptor::VariableSampleRate;
    d.hasDuration = false;
    d.sampleRate = 0;
    list.push_back(d);

    d.identifier = "chromagram";
    d.name = "Chroma Information";
    d.description = "Pitch Class Profile Visualization";
    d.unit = "pitch class";
    d.binCount = notes_Total;
    d.binNames = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    d.hasKnownExtents = true;
    d.minValue = 0;
    d.maxValue = 1;
    d.sampleType = OutputDescriptor::FixedSampleRate;
    d.sampleRate = m_inputSampleRate / m_blockSize;
    list.push_back(d);

    return list;
}

bool
Parachord::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() || channels > getMaxChannelCount()) {
        return false;
    }

    m_stepSize = stepSize;
    m_blockSize = blockSize;

    return true;
}

void
Parachord::reset()
{
    m_channelInput.clear();
}

Parachord::FeatureSet
Parachord::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    (void)inputBuffers;
    (void)timestamp;

    if (m_stepSize == 0) {
        cerr << "ERROR: Parachord::process(): not initialized";
        return FeatureSet();
    }

    m_channelInput.insert(m_channelInput.end(), inputBuffers[0],
                          inputBuffers[0] + m_blockSize);

    return FeatureSet();
}

Parachord::Feature
Parachord::segmentToFeature(segment_t *s)
{
    Parachord::Feature f;

    f.label = s->silence ? "S" : s->chord.toString();
    f.hasTimestamp = true;
    f.timestamp = RealTime::frame2RealTime(s->startIdx, m_inputSampleRate);
    f.hasDuration = true;
    f.duration = RealTime::frame2RealTime(s->endIdx, m_inputSampleRate) - f.timestamp;

    return f;
}

void
Parachord::trimInput()
{
    auto it = m_channelInput.end();
    while (it != m_channelInput.begin() && *it == 0 &&
           static_cast<size_t>(std::distance(it, m_channelInput.end())) < m_blockSize)
    {
        it--;
    }
    m_channelInput.erase(it, m_channelInput.end());
}

Parachord::FeatureSet
Parachord::getChordFeatures()
{
    anatomist::ChordDetector *cd = new anatomist::ChordDetector();
    vector<segment_t> segments;
    Parachord::FeatureSet retFeatures;

    cd->getSegments(segments, m_channelInput.data(), m_channelInput.size(), m_inputSampleRate);

    for (uint32_t i = 0; i < segments.size(); i++) {
        segment_t *s = &segments[i];
        cout << s->chord << endl;
        retFeatures[0].push_back(segmentToFeature(s));
    }

    anatomist::chromagram_t chromagram = cd->GetChromagram(m_channelInput.data(), m_channelInput.size(), m_inputSampleRate);
    for (uint32_t i = 0; i < chromagram.size(); i++) {
        Parachord::Feature f;
        f.hasTimestamp = false;
        for (int N = note_Min; N <= note_Max; N++)
            f.values.push_back(chromagram[i].getPitchCls(static_cast<note_t>(N)));
        retFeatures[1].push_back(f);
    }
    delete cd;

    return retFeatures;
}

Parachord::FeatureSet
Parachord::getRemainingFeatures()
{
    Parachord::FeatureSet retFeatures;

    if (m_stepSize == 0) {
        cerr << "ERROR: Parachord::getRemainingFeatures(): not initialised";
        return FeatureSet();
    }

    trimInput();

    retFeatures = getChordFeatures();

    return retFeatures;
}
