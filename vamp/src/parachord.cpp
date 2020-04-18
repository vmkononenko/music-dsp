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

using namespace std;

Parachord::Parachord(float inputSampleRate) :
    Plugin(inputSampleRate),
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
    return 0; // 0 means "I can handle any block size"
}

size_t
Parachord::getPreferredStepSize() const
{
    return 0; // 0 means "anything sensible"; in practice this
              // means the same as the block size for TimeDomain
              // plugins, or half of it for FrequencyDomain plugins
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
    //d.sampleRate = featureRate;
    list.push_back(d);

    return list;
}

bool
Parachord::initialise(size_t channels, size_t stepSize, size_t blockSize)
{
    if (channels < getMinChannelCount() || channels > getMaxChannelCount()) {
        return false;
    }

    (void) stepSize;
    (void) blockSize;

    // Real initialisation work goes here!

    return true;
}

void
Parachord::reset()
{
    // Clear buffers, reset stored values, etc
}

Parachord::FeatureSet
Parachord::process(const float *const *inputBuffers, Vamp::RealTime timestamp)
{
    (void)inputBuffers;
    (void)timestamp;
    // Do actual work!
    return FeatureSet();
}

Parachord::FeatureSet
Parachord::getRemainingFeatures()
{
    return FeatureSet();
}
