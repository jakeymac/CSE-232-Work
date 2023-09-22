/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *
 *        ____     _______        __
 *      .' __ '.  |  _____|   _  / /
 *      | (__) |  | |____    (_)/ /
 *      .`____'.  '_.____''.   / / _
 *     | (____) | | \____) |  / / (_)
 *     `.______.'  \______.' /_/
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
 * Just like the std :: vector <T> class
 ****************************************/
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
   vector(size_t numElements, const int & t);
   vector(const std::initializer_list<int>& l);
   vector(const vector &  rhs);
   vector(      vector && rhs);
   ~vector();

   //
   // Assign
   //

   void swap(vector& rhs)
   {

   }
   vector & operator = (const vector& rhs);
   vector & operator = (vector && rhs);
   
   //
   // Iterator
   //

   class iterator;
   iterator begin();
   iterator end();

   //
   // Access
   //

         int& operator [] (size_t index);
   const int& operator [] (size_t index) const;
         int& front();
   const int& front() const;
         int& back();
   const int& back() const;

   // 
   // Insert
   //

   void push_back(const int& t);
   void push_back(int&& t);
   void resize(size_t newElements);
   void resize(size_t newElements, const int& t);
   void reserve(size_t newCapacity);

   //
   // Remove
   //

   void clear()
   {
   }
   void pop_back()
   {
   }
   void shrink_to_fit();

   //
   // Status
   //

   size_t  size()     const { return 999;  }
   size_t  capacity() const { return 999;  }
   bool empty()       const { return true; }
   
private:
   
   int *   data;              // user data, a dynamically-allocated array
   size_t  numCapacity;       // the capacity of the array
   size_t  numElements;       // the number of items currently used
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
class vector::iterator
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   // constructors, destructors, and assignment operator
   iterator()                           {  }
   iterator(int* p)                     {  }
   iterator(const iterator& rhs)        {  }
   iterator(size_t index, vector& v)    {  }
   iterator& operator = (const iterator& rhs)
   {
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return true; }
   bool operator == (const iterator& rhs) const { return true; }

   // dereference operator
   int& operator * ()
   {
      return *(new int);
   }

   // prefix increment
   iterator& operator ++ ()
   {
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      return *this;
   }

   // prefix decrement
   iterator& operator -- ()
   {
      return *this;
   }

   // postfix decrement
   iterator operator -- (int postfix)
   {
      return *this;
   }

private:
   int* p;
};


/*****************************************
 * VECTOR :: DEFAULT constructors
 * efault constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
vector :: vector()
{
   data = new int[99];
   numElements = 17;
   numCapacity = 19;
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
vector :: vector(size_t num, const int & t) 
{
   data = new int[99];
   numElements = 17;
   numCapacity = 19;
}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
vector :: vector(const std::initializer_list<int> & l) 
{
   data = new int[99];
   numElements = 17;
   numCapacity = 19;
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
vector :: vector(size_t num)
{
   data = new int[99];
   numElements = 17;
   numCapacity = 19;
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
vector :: vector (const vector & rhs)
{
   data = new int[99];
   numElements = 17;
   numCapacity = 19;
}

/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
vector :: vector (vector && rhs)
{
   data = new int[99];
   numElements = 17;
   numCapacity = 19;
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
vector :: ~vector()
{
   
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
void vector :: resize(size_t newElements)
{
   numElements = 21;
}

void vector :: resize(size_t newElements, const int & t)
{
   numElements = 21;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
void vector :: reserve(size_t newCapacity)
{
   numCapacity = 21;
}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
void vector :: shrink_to_fit()
{
   
}



/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
int & vector :: operator [] (size_t index)
{
   return *(new int);
}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
const int & vector :: operator [] (size_t index) const
{
   return *(new int);
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
int & vector :: front ()
{
   return *(new int);
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
const int & vector :: front () const
{
   return *(new int);
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
int & vector :: back()
{
   return *(new int);
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
const int & vector :: back() const
{
   return *(new int);
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
void vector :: push_back (const int & t)
{
   
}

void vector :: push_back(int && t)
{
   
   
}

/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
vector & vector :: operator = (const vector & rhs)
{
   
   return *this;
}
vector& vector :: operator = (vector&& rhs)
{

   return *this;
}




/***************************************
 * VECTOR :: BEGIN
 * This will return an iterator referring to
 * the beginning element of the vector.
 *     INPUT  : None
 *     OUTPUT : iterator
 **************************************/
vector::iterator vector :: begin()
{
   return iterator();
}

/***************************************
 * VECTOR :: END
 * This will return an iterator referring to the past
 * the end element of the vector.
 *     INPUT  : None
 *     OUTPUT : iterator
 **************************************/
vector::iterator vector :: end()
{
   return iterator();
}



} // namespace custom
