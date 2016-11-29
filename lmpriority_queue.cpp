/**
 * lmpriority_queue.cpp
 *
 * Max priority queue implementation. Current is implementation is just a
 * wrapper around std::priority_queue. Wrapper is used for API unification
 * across the library with further possibility of optimization
 */

#include "lmpriority_queue.h"

using namespace std;

template <class Item>
PriorityQueue<Item>::PriorityQueue()
{
    __mQueue = new priority_queue<Item>();
}

template <class Item>
PriorityQueue<Item>::~PriorityQueue()
{
    delete __mQueue;
}

template <class Item>
void PriorityQueue<Item>::insert(Item item)
{
    __mQueue->push(item);
}

template <class Item>
Item PriorityQueue<Item>::getMax()
{
    return __mQueue->top();
}

template <class Item>
Item PriorityQueue<Item>::delMax()
{
    Item max = __mQueue->top();

    __mQueue->pop();

    return max;
}

template <class Item>
bool PriorityQueue<Item>::isEmpty()
{
    return __mQueue->empty();
}

template <class Item>
uint32_t PriorityQueue<Item>::size()
{
    return __mQueue->size();
}

