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
 * @file        lmpriority_queue.h
 * @brief       Max priority queue API
 * @addtogroup  libmusic
 * @{
 */

#pragma once

#include <queue>

#include "fft_point.h"
#include "lmtypes.h"


class PriorityQueue {

private:
    std::priority_queue<FftPoint> *__mQueue;

public:
    /**
     * Priority queue constructor
     */
    PriorityQueue();

    /**
     * Create a priority queue with specified initial capacity
     *
     * @param   n   initial capacity
     */
    PriorityQueue(uint32_t n);

    /**
     * Destructor
     */
    ~PriorityQueue();

    /**
     * Insert an item into priority queue
     *
     * @param   item    an item to be inserted
     */
    void insert(FftPoint item);

    /**
     * Retrieve the largest item from the queue
     *
     * @return  item with the highest value
     */
    FftPoint getMax();

    /**
     * Retrieve and remove the largest item from the queue
     *
     * @return item with the highest value
     */
    FftPoint delMax();

    /**
     * Is the priority queue empty
     *
     * @return true if empty
     */
    bool isEmpty();

    /**
     * Get priority queue size
     *
     * @return number of items in the priority queue
     */
    uint32_t size();
};

/** @} */

