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

/**
 * @file    lmpriority_queue.cpp
 * @brief   Max priority queue implementation
 *
 * Current is implementation is just a wrapper around std::priority_queue.
 * Wrapper is used for API unification across the library with further
 * possibility of optimization.
 */

#include "lmpriority_queue.h"

using namespace std;

PriorityQueue::PriorityQueue()
{
    __mQueue = new priority_queue<FftPoint>();
}

PriorityQueue::~PriorityQueue()
{
    delete __mQueue;
}

void PriorityQueue::insert(FftPoint item)
{
    __mQueue->push(item);
}

FftPoint PriorityQueue::getMax()
{
    return __mQueue->top();
}

FftPoint PriorityQueue::delMax()
{
    FftPoint max = __mQueue->top();

    __mQueue->pop();

    return max;
}

bool PriorityQueue::isEmpty()
{
    return __mQueue->empty();
}

uint32_t PriorityQueue::size()
{
    return __mQueue->size();
}
