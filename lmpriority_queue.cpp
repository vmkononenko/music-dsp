/**
 * lmpriority_queue.cpp
 *
 * Max priority queue implementation. Current is implementation is just a
 * wrapper around std::priority_queue. Wrapper is used for API unification
 * across the library with further possibility of optimization
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
