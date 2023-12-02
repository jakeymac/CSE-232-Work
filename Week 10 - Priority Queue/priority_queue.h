/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>
#include "vector.h"

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
template<class T>
class priority_queue
{
   friend class ::TestPQueue; // give the unit test class access to the privates
   template <class TT>
   friend void swap(priority_queue<TT>& lhs, priority_queue<TT>& rhs);
public:

   //
   // construct
   //
    priority_queue()
    {
    }
    
    priority_queue(const priority_queue& rhs) : container(rhs.container) {}
    
    priority_queue(priority_queue&& rhs) noexcept : container(std::move(rhs.container)) {}
    
    template <class Iterator>
    
    priority_queue(Iterator first, Iterator last) 
    {
        container.resize(0);
        while (first != last) {
            push(*first);
            ++first;
        }
    }
    explicit priority_queue(custom::vector<T>&& rhs) : container(std::move(rhs)) {}
    
    explicit priority_queue(custom::vector<T>& rhs) {
        for (const auto& element : rhs) {
            push(element);
        }
    }
    
    ~priority_queue() {}

    
   //
   // Access
   //
   const T & top() const;

   //
   // Insert
   //
   void  push(const T& t);
   void  push(T&& t);

   //
   // Remove
   //
   void  pop();

   //
   // Status
   //
    size_t size() const
    {
        return container.size();
    }

    bool empty() const
    {
        return container.empty();
    }
   
private:

   bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

   custom::vector<T> container;

};

/************************************************
 * P QUEUE :: TOP
 * Get the maximum item from the heap: the top item.
 ***********************************************/
template <class T>
const T & priority_queue <T> :: top() const
{
    if (!container.empty())
        {
            return container.front();
        }else
        {
            throw std::out_of_range("std:out_of_range");
        }
}

/**********************************************
 * P QUEUE :: POP
 * Delete the top item from the heap.
 **********************************************/
template <class T>
void priority_queue <T> :: pop()
{
    if (!container.empty())
        {
            std::swap(container[0], container.back());
            container.pop_back();
            percolateDown(0);
        }
}

/*****************************************
 * P QUEUE :: PUSH
 * Add a new element to the heap, reallocating as necessary
 ****************************************/
template <class T>
void priority_queue<T>::push(const T& t)
{
    container.push_back(t);
    size_t currentIndex;
    if (container.size() >= 3) {
        
        currentIndex = container.size() / 2 + 1;
        
    } else {
        currentIndex = 1;
        
        
    }
    while (currentIndex and percolateDown(currentIndex)){
        currentIndex = currentIndex / 2;
    }
    container.shrink_to_fit();
}

template <class T>
void priority_queue<T>::push(T&& t)
{
    container.push_back(std::move(t));
    size_t currentIndex;
    if (container.size() >= 3) {
        
        currentIndex = container.size() / 2 + 1;
        
    } else {
        currentIndex = 1;
        percolateDown(currentIndex);
    }
    while (currentIndex and percolateDown(currentIndex)){
        currentIndex = currentIndex / 2 ;
    }
    container.shrink_to_fit();
}


/************************************************
 * P QUEUE :: PERCOLATE DOWN
 * The item at the passed index may be out of heap
 * order. Take care of that little detail!
 * Return TRUE if anything changed.
 ************************************************/
template <class T>
bool priority_queue<T>::percolateDown(size_t indexHeap)
{
    std::cout << "Percolating\n";
    
    //{9}

    
//    size_t indexLeft;
//    size_t indexRight;
//
//    if (indexHeap == 0) {
//        indexLeft = 1;
//        indexRight = 2;
//    }
//
//    else {
    
    if (container.size() < 3) {
        if (container[0] < container[1]) {
            std::swap(container[0],container[1]);
            return true;
        }
        return false;
    } else {
        
        size_t indexLeft = indexHeap * 2 + 1;
        size_t indexRight = indexLeft + 1;
        size_t indexBigger;
        
        
        if (container[indexRight-1] <= container[indexHeap-1] && container[indexLeft-1] < container[indexRight-1]) {
            indexBigger = indexRight;
        }
        else {
            indexBigger = indexLeft;
        }
        
        if (container[indexHeap-1] < container[indexBigger-1]) {
            std::swap(container[indexHeap-1],container[indexBigger-1]);
            return percolateDown(indexBigger);
        }
        
        return false;
    }
}
//        k

/************************************************
 * SWAP
 * Swap the contents of two priority queues
 ************************************************/
template <class T>
inline void swap(custom::priority_queue <T>& lhs,
                 custom::priority_queue <T>& rhs)
{
    
    std::swap(lhs.container, rhs.container);
}

};
