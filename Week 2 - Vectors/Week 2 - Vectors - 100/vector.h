/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *
 *    __       ____       ____         __
 *   /  |    .'    '.   .'    '.   _  / /
 *   `| |   |  .--.  | |  .--.  | (_)/ /
 *    | |   | |    | | | |    | |   / / _
 *   _| |_  |  `--'  | |  `--'  |  / / (_)
 *  |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author:
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

    //
    // VECTOR METHOD IMPLEMENTATIONS
    //

    // Constructors
    template <typename T>
    vector <T>::vector()
    {
        data = nullptr;
        numCapacity = 0;
        numElements = 0;
    }

    template <typename T>
    vector <T>::vector(size_t numElements)
    {
        data = new T[numElements];
        numCapacity = numElements;
        this->numElements = numElements;
    }

    template <typename T>
    vector <T>::vector(size_t numElements, const T& t)
    {
        data = new T[numElements];
        numCapacity = numElements;
        this->numElements = numElements;

        for (size_t i = 0; i < numElements; ++i) {
            data[i] = t;
        }
    }

    template <typename T>
    vector <T>::vector(const std::initializer_list<T>& l)
    {
        numCapacity = l.size();
        numElements = l.size();
        data = new T[numCapacity];

        size_t i = 0;
        for (const T& element : l) {
            data[i++] = element;
        }
    }

    template <typename T>
    vector <T>::vector(const vector& rhs)
    {
        numCapacity = rhs.numCapacity;
        numElements = rhs.numElements;
        data = new T[numCapacity];

        for (size_t i = 0; i < numElements; ++i) {
            data[i] = rhs.data[i];
        }
    }

    template <typename T>
    vector <T>::vector(vector&& rhs)
    {
        numCapacity = rhs.numCapacity;
        numElements = rhs.numElements;
        data = rhs.data;

        rhs.data = nullptr;
        rhs.numCapacity = 0;
        rhs.numElements = 0;
    }

    // Destructor
    template <typename T>
    vector <T>::~vector()
    {
        delete[] data;
    }

    // Iterator Methods
    template <typename T>
    typename vector <T>::iterator vector <T>::begin()
    {
        return iterator(data);
    }

    template <typename T>
    typename vector <T>::iterator vector <T>::end()
    {
        return iterator(data + numElements);
    }

    // Access Methods
    template <typename T>
    T& vector <T>::operator[](size_t index)
    {
        assert(index < numElements);
        return data[index];
    }

    template <typename T>
    const T& vector <T>::operator[](size_t index) const
    {
        assert(index < numElements);
        return data[index];
    }

    template <typename T>
    T& vector <T>::front()
    {
        assert(!empty());
        return data[0];
    }

    template <typename T>
    const T& vector <T>::front() const
    {
        assert(!empty());
        return data[0];
    }

    template <typename T>
    T& vector <T>::back()
    {
        assert(!empty());
        return data[numElements - 1];
    }

    template <typename T>
    const T& vector <T>::back() const
    {
        assert(!empty());
        return data[numElements - 1];
    }

    // Insert Methods
    template <typename T>
    void vector <T>::push_back(const T& t)
    {
        if (numElements >= numCapacity) {
            reserve(numCapacity == 0 ? 1 : numCapacity * 2);
        }
        data[numElements++] = t;
    }

    template <typename T>
    void vector <T>::push_back(T&& t)
    {
        if (numElements >= numCapacity) {
            reserve(numCapacity == 0 ? 1 : numCapacity * 2);
        }
        data[numElements++] = std::move(t);
    }

    template <typename T>
    void vector <T>::reserve(size_t newCapacity)
    {
        if (newCapacity <= numCapacity) {
            return;
        }

        T* newData = new T[newCapacity];

        for (size_t i = 0; i < numElements; ++i) {
            newData[i] = std::move(data[i]);
        }

        delete[] data;
        data = newData;
        numCapacity = newCapacity;
    }

    template <typename T>
    void vector <T>::resize(size_t newElements)
    {
        if (newElements < numElements) {
            numElements = newElements;
        }
        else if (newElements > numCapacity) {
            reserve(newElements);
            for (size_t i = numElements; i < newElements; ++i) {
                data[i] = T();
            }
            numElements = newElements;
        }
        else {
            for (size_t i = numElements; i < newElements; ++i) {
                data[i] = T();
            }
            numElements = newElements;
        }
    }

    template <typename T>
    void vector <T>::resize(size_t newElements, const T& t)
    {
        if (newElements < numElements) {
            numElements = newElements;
        }
        else if (newElements > numCapacity) {
            reserve(newElements);
            for (size_t i = numElements; i < newElements; ++i) {
                data[i] = t;
            }
            numElements = newElements;
        }
        else {
            for (size_t i = numElements; i < newElements; ++i) {
                data[i] = t;
            }
            numElements = newElements;
        }
    }

    // Remove Methods
    template <typename T>
    void vector <T>::clear()
    {
        numElements = 0;
    }

    template <typename T>
    void vector <T>::pop_back()
    {
        if (!empty()) {
            --numElements;
        }
    }

    template <typename T>
    void vector <T>::shrink_to_fit()
    {
        if (numCapacity > numElements) {
            reserve(numElements);
        }
    }

    // Status Methods
    template <typename T>
    size_t vector <T>::size() const
    {
        return numElements;
    }

    template <typename T>
    size_t vector <T>::capacity() const
    {
        return numCapacity;
    }

    template <typename T>
    bool vector <T>::empty() const
    {
        return numElements == 0;
    }

} // namespace custom

