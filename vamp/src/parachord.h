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

#pragma once

#include <string>

#include "vamp-sdk/Plugin.h"

class Parachord : public Vamp::Plugin
{
protected:
    size_t m_stepSize;
    size_t m_blockSize;

    float  m_threshold;
    float  m_sensitivity;
    float *m_priorMagnitudes;
    float  m_dfMinus1;
    float  m_dfMinus2;

public:
    Parachord(float inputSampleRate);
    virtual ~Parachord();

    bool initialise(size_t channels, size_t stepSize, size_t blockSize);
    void reset();

    std::string getIdentifier() const;
    std::string getName() const;
    std::string getDescription() const;
    std::string getMaker() const;
    std::string getCopyright() const;
    int getPluginVersion() const;

    size_t getMinChannelCount() const;
    size_t getMaxChannelCount() const;
    InputDomain getInputDomain() const;
    size_t getPreferredStepSize() const;
    size_t getPreferredBlockSize() const;

    ParameterList getParameterDescriptors() const;
    float getParameter(std::string id) const;
    void setParameter(std::string id, float value);

    OutputList getOutputDescriptors() const;

    FeatureSet process(const float *const *inputBuffers, Vamp::RealTime timestamp);

    FeatureSet getRemainingFeatures();
};
