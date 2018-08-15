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
