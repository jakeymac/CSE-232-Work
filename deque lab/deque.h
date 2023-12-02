/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <stdexcept>
#include <iostream>

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *         iaFront
 * ia =   0   1   2   3   4
 *      +---+---+---+---+---+
 *      |   | A | B | C |   |
 *      +---+---+---+---+---+
 * id =       0   1   2
 * iaFront = 1
 * numElements = 3
 * numCapacity = 5
 *****************************************************/
template <class T>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   //
   // Construct
   //
   deque()
   {
       numCapacity = 0;
       numElements = 0;
       iaFront = 0;
       
   }
   deque(int newCapacity);
   deque(const deque <T> & rhs);
   ~deque()
   {
   }

   //
   // Assign
   //
   deque<T> & operator = (const deque <T> & rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin()
   {
      return iterator(this,0);
   }
   iterator end()
   {
       
       return iterator(this,numElements);
   }

   //
   // Access
   //
   T& front();
   T& back();
   const T & front() const;
   const T & back()  const;
   const T & operator[](size_t index) const;
   T& operator[](size_t index);

   //
   // Insert
   //
   void push_front(const T& t);
   void push_back(const T& t);

   //
   // Remove
   //
   void clear()
   {
       delete[] data;

    
       data = nullptr;
       numCapacity = 0;
       numElements = 0;
       iaFront = 0;
   }
   void pop_front();
   void pop_back();

   //
   // Status
   //
   size_t size() const
   {
      return numElements;
   }
   bool empty() const
   {
      return numElements == 0;
   }

   
private:

    // fetch array index from the deque index
    // raises from 52-56
    int iaFromID(int id) const
    {
        if (id < 0 || id >= static_cast<int>(numElements))
            throw std::out_of_range("Deque index out of range");

        return (iaFront + id) % numCapacity;
    }

    // no score change
    void resize(int newCapacity);

   // member variables
   T * data;           // dynamically allocated data for the deque
   size_t numCapacity; // the size of the data array
   size_t numElements; // number of elements in the deque
   int iaFront;        // the index of the first item in the array
};



/**********************************************************
 * DEQUE ITERATOR
 * Forward and reverse iterator through a deque, just call
 *********************************************************/
template <typename T>
class deque <T> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   //
   // Construct
   //
   iterator()
   {
   }
    iterator(custom::deque<T> *pDeque, int id) :  pDeque(pDeque), id(id)
   {
   }
   iterator(const iterator& rhs)
   {
       id = rhs.id;
       pDeque = rhs.pDeque;
       
    
   }

   //
   // Assign
   //
    // no score change
    iterator &operator=(const iterator &rhs)
    {
        if (this != &rhs)
        {
            pDeque = rhs.pDeque;
            id = rhs.id;
        }
        return *this;
    }


   //
   // Compare
   //
    // no score change
    bool operator==(const iterator &rhs) const
    {
        return pDeque == rhs.pDeque && id == rhs.id;
    }

    bool operator!=(const iterator &rhs) const
    {
        return !(*this == rhs);
    }

   //
   // Access
   //
    // raised score 56-63
    const T &operator*() const
    {
        if (pDeque)
            return (*pDeque)[id];
        else
            throw std::out_of_range("Dereferencing end() iterator");
    }

    T &operator*()
    {
        if (pDeque)
            return (*pDeque)[id];
        else
            throw std::out_of_range("Dereferencing end() iterator");
    }

    // Arithmetic
    // raises from 47-52
    int operator-(iterator it) const
    {
        return id - it.id;
    }

    iterator &operator+=(int offset)
    {
        id += offset;
        return *this;
    }

    iterator &operator++()
    {
        ++id;
        return *this;
    }

    iterator operator++(int postfix)
    {
        iterator temp(*this);
        ++id;
        return temp;
    }

    iterator &operator--()
    {
        --id;
        return *this;
    }

    iterator operator--(int postfix)
    {
        iterator temp(*this);
        --id;
        return temp;
    }

private:

   // Member variables
   int id;             // deque index
   deque<T> *pDeque;
};


/****************************************************
 * DEQUE : CONSTRUCTOR - non-default
 ***************************************************/
// score unchanged
template <class T>
deque<T>::deque(int newCapacity)
{
    numCapacity = newCapacity;
    numElements = 0;
    iaFront = 0;
    data = new T[newCapacity];
}


/****************************************************
 * DEQUE : CONSTRUCTOR - copy
 ***************************************************/
// gives issues
template <class T>
deque <T> :: deque(const deque <T> & rhs)
{
    numCapacity = rhs.numCapacity;
    numElements = rhs.numElements;
    data = rhs.data;
    iaFront = 0;
}


/****************************************************
 * DEQUE : ASSIGNMENT OPERATOR
 ***************************************************/
// gives issues
template <class T>
deque<T>& deque<T>::operator=(const deque<T> &rhs) {
    if (this != &rhs) {
        // Clean up existing resources
        
        //delete[] data;
        
        // Copy elements from rhs to the current deque
        if (rhs.numCapacity > numCapacity) {
            numCapacity = rhs.numCapacity;
        }
        iaFront = 0;
        data = new T[numCapacity];
        numElements = 0;
        int currentRHSiaFront = rhs.iaFront;
        while (numElements != rhs.numElements) {
            if (currentRHSiaFront == rhs.numElements) {
                currentRHSiaFront = 0;
            }
            
            data[iaFront] = rhs.data[currentRHSiaFront];
            currentRHSiaFront ++;
            iaFront ++;
            numElements ++;
        }
        
        iaFront = 0;
        // Allocate a new array for data
        
        
        // Copy elements
        //        for (size_t i = 0; i < numElements; ++i) {
        //                    size_t sourceIndex = (rhs.iaFront + i) % rhs.numCapacity;
        //                    size_t destIndex = (iaFront + i) % numCapacity;
        //                    data[destIndex] = rhs.data[sourceIndex];
        //                }
        //
        }
    return *this;
}


/**************************************************
 * DEQUE :: FRONT
 * Fetch the item that is at the beginning of the deque
 *************************************************/
// score raised 41-47
template <class T>
const T &deque<T>::front() const
{
    return data[iaFront];
}

template <class T>
T &deque<T>::front()
{
    return data[iaFront];
}


/**************************************************
 * DEQUE :: BACK
 * Fetch the item that is at the end of the deque
 *************************************************/
// score raised 34-41
template <class T>
const T &deque<T>::back() const
{
    size_t lastIndex = (iaFront + numElements - 1) % numCapacity;
    return data[lastIndex];
}

template <class T>
T &deque<T>::back()
{
    size_t lastIndex = (iaFront + numElements - 1) % numCapacity;
    return data[lastIndex];
}


/**************************************************
 * DEQUE :: SUBSCRIPT
 * Fetch the item in the deque
 *************************************************/
// score raised 28-34
template <class T>
const T &deque<T>::operator[](size_t index) const
{
    size_t actualIndex = (iaFront + index) % numCapacity;
    return data[actualIndex];
}

template <class T>
T &deque<T>::operator[](size_t index)
{
    size_t actualIndex = (iaFront + index) % numCapacity;
    return data[actualIndex];
}


/*****************************************************
 * DEQUE : POP_BACK
 *****************************************************/
//score raised 23-28
template <class T>
void deque<T>::pop_back()
{
    if (numElements > 0)
    {
        --numElements;

        if (numCapacity >= 2 * numElements && numCapacity > 1)
        {
            int newCapacity = numCapacity / 2;
            resize(newCapacity);
        }
    }
     else
     {
         // need to andle the case when the deque is already empty
         std::cout << "Error: Cannot pop_back from an empty deque." << std::endl;

     }
}


/*****************************************************
 * DEQUE : POP_FRONT
 *****************************************************/
// score raised 19-23
template <class T>
void deque<T>::pop_front()
{
    
    if (numElements > 0)
    {
        iaFront = (iaFront + 1) % numCapacity;
        --numElements;
        
        if (numCapacity >= 2 * numElements && numCapacity > 1)
        {
            int newCapacity = numCapacity / 2;
            resize(newCapacity);
        }
    }
     else
     {
         // need to handle the case when the deque is already empty
     }
}


/******************************************************
 * DEQUE : PUSH_BACK
 ******************************************************/
// score raised 13-19
template <class T>
void deque<T>::push_back(const T & t)
{
    
    if (numElements == numCapacity)
    {
        int newCapacity = (numCapacity == 0) ? 1 : numCapacity * 2;
        resize(newCapacity);
    }

    size_t newIndex = (iaFront + numElements) % numCapacity;
    data[newIndex] = t;
    ++numElements;
}


/******************************************************
 * DEQUE : PUSH_FRONT
 ******************************************************/
// raised score 6-13
template <class T>
void deque<T>::push_front(const T & t)
{
    if (numElements == numCapacity)
    {
        int newCapacity = (numCapacity == 0) ? 1 : numCapacity * 2;
        resize(newCapacity);
    }

    iaFront = (iaFront - 1 + numCapacity) % numCapacity;
    data[iaFront] = t;
    ++numElements;
}

/****************************************************
 * DEQUE :: RESIZE
 * Resize the deque so the numCapacity matches the newCapacity
 ***************************************************/
//score unchanged
template <class T>
void deque<T>::resize(int newCapacity)
{
    if (newCapacity <= numCapacity)
        return;

    T* newData = new T[newCapacity];

    for (size_t i = 0; i < numElements; ++i)
    {
        size_t oldIndex = (iaFront + i) % numCapacity;
        newData[i] = data[oldIndex];
    }

    delete[] data;
    data = newData;
    numCapacity = newCapacity;
    iaFront = 0;
}


} // namespace custom
