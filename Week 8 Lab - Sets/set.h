/***********************************************************************
* Header:
*    Set
* Summary:
*      __       ____       ____         __
*     /  |    .'    '.   .'    '.   _  / /
*     `| |   |  .--.  | |  .--.  | (_)/ /
*      | |   | |    | | | |    | |   / / _
*     _| |_  |  `--'  | |  `--'  |  / / (_)
*    |_____|  '.____.'   '.____.'  /_/
*
*    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    <Tyler Elms and Jacob Johnson>
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less

class TestSet;        // forward declaration for unit tests

namespace custom
{

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
   friend class ::TestSet; // give unit tests access to the privates
public:
   
   //
   // Construct
   //
   set()
   {
   }
   set(const set& rhs) : bst(rhs.bst)
   {
   }
   set(set&& rhs) : bst(std::move(rhs.bst))
   {
   }
   set(const std::initializer_list<T>& il) : bst(il)
   {
   }
   template <class Iterator>
   set(Iterator first, Iterator last) // may need adjustment
   {
   }
  ~set() { }

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
       if (this != &rhs) {
           bst = rhs.bst;
       }
       return *this;
   }
   set & operator = (set && rhs)
   {
       if (this != &rhs) {
           bst = std::move(rhs.bst);
       }
       return *this;
   }
    set & operator = (const std::initializer_list<T> & il)
    {
        bst.clear();

        for (const auto &item : il) {
            bst.insert(item);
        }

        return *this;
    }
   void swap(set& rhs) noexcept
   {
       std::swap(bst, rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept
   {
       return bst.begin();
   }
   iterator end() const noexcept
   {
       return bst.end();
   }

   //
   // Access
   //
   iterator find(const T& t)
   {
       return bst.find(t);
   }

   //
   // Status
   //
   bool   empty() const noexcept
   {
       return bst.empty();
   }
   size_t size() const noexcept
   {
       return bst.size();
   }

   //
   // Insert
   //
    std::pair<iterator, bool> insert(const T& t)
    {
        return bst.insert(t);
    }

    std::pair<iterator, bool> insert(T&& t)
    {
        return bst.insert(std::move(t));
    }

    void insert(const std::initializer_list<T>& il)
    {
        for (const T& element : il) {
            bst.insert(element);
        }
    }

    template <class Iterator>
    void insert(Iterator first, Iterator last)
    {
        for (auto it = first; it != last; ++it) {
            bst.insert(*it);
        }
    }



   //
   // Remove
   //
    void clear() noexcept
    {
        bst.clear();
    }

    iterator erase(iterator& it)
    {
        return iterator(); //needs work
    }

    size_t erase(const T& t)
    {
        return 99; //needs work
    }

    iterator erase(iterator& itBegin, iterator& itEnd)
    {
        return iterator(); //needs work
    }


private:
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
   friend class ::TestSet; // give unit tests access to the privates
   friend class custom::set<T>;

public:
    typename custom::BST<T>::iterator bstIterator; // not sure if this is how it should work
   // constructors, destructors, and assignment operator
   iterator()
   {
   }
    iterator(const typename custom::BST<T>::iterator& itRHS)
           : bstIterator(itRHS)
    {
    }

   iterator(const iterator & rhs)
       : bstIterator(rhs.bstIterator)
    {
    }
   iterator & operator = (const iterator & rhs)
   {
      return *this; // not sure if this needs to change
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const
   {
      return true; // needs work
   }
   bool operator == (const iterator & rhs) const
   {
      return true; // needs work
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const
   {
      return *(new T);
   }

    // prefix increment
    iterator & operator ++ ()
    {
        ++bstIterator;
        return *this;
    }

    // postfix increment
    iterator operator++ (int postfix)
    {
        iterator temp = *this;
        ++(*this);
        return temp;
    }

    // prefix decrement
    iterator & operator -- ()
    {
        --bstIterator;
        return *this;
    }

    // postfix decrement
    iterator operator-- (int postfix)
    {
        iterator temp = *this;
        --(*this);
        return temp;
    }
   
private:

   typename custom::BST<T>::iterator it;
};


}; // namespace custom
