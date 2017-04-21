/**
 * lmpriority_queue.h
 *
 * Max priority queue API
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

