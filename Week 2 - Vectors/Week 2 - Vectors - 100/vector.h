/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An interator through Vector
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

    /*****************************************
     * VECTOR
     * Just like the std::vector<T> class
     ****************************************/
    template <typename T>
    class vector
    {
        friend class ::TestVector; // give unit tests access to the privates
        friend class ::TestStack;
        friend class ::TestPQueue;
        friend class ::TestHash;
    public:

        //
        // Construct
        //

        vector();
        vector(size_t numElements);
        vector(size_t numElements, const T& t);
        vector(const std::initializer_list<T>& l);
        vector(const vector& rhs);
        vector(vector&& rhs);
        ~vector();

        //
        // Assign
        //

        void swap(vector& rhs);

        vector& operator=(const vector& rhs);
        vector& operator=(vector&& rhs);

        //
        // Iterator
        //

        class iterator;
        iterator begin();
        iterator end();

        //
        // Access
        //

        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        //
        // Insert
        //

        void push_back(const T& t);
        void push_back(T&& t);
        void reserve(size_t newCapacity);
        void resize(size_t newElements);
        void resize(size_t newElements, const T& t);

        //
        // Remove
        //

        void clear();
        void pop_back();
        void shrink_to_fit();

        //
        // Status
        //

        size_t size() const;
        size_t capacity() const;
        bool empty() const;

        // adjust the size of the buffer

        // vector-specific interfaces

    private:

        T* data;                 // user data, a dynamically-allocated array
        size_t numCapacity;       // the capacity of the array
        size_t numElements;       // the number of items currently used
    };


/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
    template <typename T>
    class vector <T>::iterator
    {
        friend class ::TestVector; // give unit tests access to the privates
        friend class ::TestStack;
        friend class ::TestPQueue;
        friend class ::TestHash;
    public:
        // constructors, destructors, and assignment operator
        iterator() : p(nullptr) {}
        iterator(T* p) : p(p) {}
        iterator(const iterator& rhs) : p(rhs.p) {}
        iterator(size_t index, vector<T>& v) : p(index < v.numElements ? &v.data[index] : nullptr) {}
        iterator& operator=(const iterator& rhs)
        {
            p = rhs.p;
            return *this;
        }

        // equals, not equals operator
        bool operator != (const iterator& rhs) const { return p != rhs.p; }
        bool operator == (const iterator& rhs) const { return p == rhs.p; }

        // dereference operator
        T& operator * () { return *p; }

        // prefix increment
        iterator& operator ++ ()
        {
            ++p;
            return *this;
        }

        // postfix increment
        iterator operator ++ (int postfix)
        {
            iterator tmp = *this;
            ++p;
            return tmp;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            --p;
            return *this;
        }

        // postfix decrement
        iterator operator -- (int postfix)
        {
            iterator tmp = *this;
            --p;
            return tmp;
        }

    private:
        T* p;
    };

/*****************************************
 * VECTOR :: DEFAULT constructors
 * Default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector()
{
    data = nullptr;
    numCapacity = 0;
    numElements = 0;
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num, const T & t) 
{
    data = new T[num];
    numCapacity = num;
    numElements = num;
    
    for (int i = 0; i < numElements; ++i) {
        data[i] = t;
    }

}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T>
vector <T> :: vector(const std::initializer_list<T> & l) 
{
    numCapacity = l.size();
    numElements = l.size();
    data = new T[numCapacity];
    for (const T& element: l) {
        data[numElements++] = element;
    }
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num) 
{
    data = new T[num];
    numCapacity = num;
    numElements = num;
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T>
vector <T> :: vector (const vector & rhs) 
{
   data = new T[10];
   numCapacity = 99;
   numElements = 99;
}

/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T>
vector <T> :: vector (vector && rhs)
{
    numCapacity = rhs.size();
    numElements = rhs.size();
    data = rhs.data;
    
    rhs.data = nullptr;
    rhs.numCapacity = 0;
    rhs.numElements = 0;
   
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T>
vector <T> :: ~vector()
{
   
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: resize(size_t newElements)
{
   
}

template <typename T>
void vector <T> :: resize(size_t newElements, const T & t)
{
   
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: reserve(size_t newCapacity)
{
   numCapacity = 99;
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: shrink_to_fit()
{
   
}



/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: operator [] (size_t index)
{
   return *(new T);
   
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: operator [] (size_t index) const
{
   return *(new T);
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: front ()
{
   
   return *(new T);
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: front () const
{
   return *(new T);
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: back()
{
   return *(new T);
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: back() const
{
   return *(new T);
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T>
void vector <T> :: push_back (const T & t) {
    if (numElements >= numCapacity) {
        numCapacity += 1;
        T* new_data = new T[numCapacity];
        
        for (int i = 0; i < numElements; ++i) {
            new_data[i] = data[i];
        }
        
        delete[] data;
        
        data = new_data;
    }
    data[numElements++] = t;
}

template <typename T>
void vector <T> ::push_back(T && t)
{
    if (numElements >= numCapacity) {
        numCapacity += 1;
        T* new_data = new T[numCapacity];
        
        for (int i = 0; i < numElements; ++i) {
            new_data[i] = data[i];
        }
        
        delete[] data;
        
        data = new_data;
    }
    data[numElements++] = std::move(t);
   
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T>
vector <T> & vector <T> :: operator = (const vector & rhs)
{
    delete[] data;
    numCapacity = rhs.numCapacity;
    numElements = rhs.numElements;
    data = new T[numCapacity];
    
    for (size_t i = 0; i < numElements; ++i) {
        data[i] = rhs.data[i];
    }
    return (*this);
}
template <typename T>
vector <T>& vector <T> :: operator = (vector&& rhs)
{
    
    delete[] data;
    numCapacity = rhs.numCapacity;
    numElements = rhs.numElements;
    data = new T[numCapacity];
    
    for (int i = 0; i < numElements; i++) {
        data[i] = rhs.data[i];
    }
    rhs.data = nullptr;
    rhs.numCapacity = 0;
    rhs.numElements = 0;
    
    return (*this);
}




} // namespace custom

