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

/*
 * transform.cpp
 */

#include "transform.h"


Transform::Transform() {}

Transform::~Transform() {}

bool Transform::IsPolar()
{
    return polar_;
}

freq_domain_t & Transform::GetFreqDomain()
{
    return fd_;
}

uint32_t Transform::GetFreqDomainLen()
{
    return fd_len_;
}

uint32_t Transform::GetSize()
{
    return size_;
}

uint32_t Transform::GetSampleRate()
{
    return samplerate_;
}
