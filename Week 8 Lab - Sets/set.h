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
*    <Your name here>
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
        bst = BST<T>();
       bst.root = nullptr;
       bst.numElements = 0;
   }
   set(const set &  rhs)
   {
       bst = rhs.bst;
   }
   set(set && rhs)
   {
       bst = std::move(rhs.bst);
   }
   set(const std::initializer_list <T> & il)
   {
       clear();
       for (auto element: il) {
           insert(element);
       }
   }
   template <class Iterator>
   set(Iterator first, Iterator last)
   {
       for (Iterator it = first; it != last; ++it) {
           insert(*it);
       }
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
   set & operator = (const std::initializer_list <T> & il)
   {
       clear();
       for (const T & value : il) {
               insert(value);
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
      return iterator(bst.begin());
   }
   iterator end() const noexcept
   {
      return iterator(bst.end());
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
   void insert(const std::initializer_list <T>& il)
   {
       for (const T& element: il) {
           bst.insert(element);
       }
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       for (Iterator it = first; it != last; ++it) {
           insert(*it);
       }
   }


   //
   // Remove
   //
   void clear() noexcept
   {
       bst.clear();
   }
   iterator erase(iterator &it)
   {
       return iterator(bst.erase(it.it));
   }
   size_t erase(const T & t)
   {
       auto it = bst.find(t);
       bst.erase(it);
       return bst.size();
   }
   iterator erase(iterator &itBegin, iterator &itEnd)
    {
       while (itBegin != itEnd) {
           itBegin = erase(itBegin);
       }
       return itEnd;
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
   // constructors, destructors, and assignment operator
   iterator()
   {
   }
   iterator(const typename custom::BST<T>::iterator& itRHS) : it(itRHS)
   {
       
   }
   iterator(const iterator & rhs) :it(rhs.it)
   {
   }
   iterator & operator = (const iterator & rhs)
   {
       {
                   if (this != &rhs) {
                       it = rhs.it;
                   }
                   return *this;
               }
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const
   {
       return it != rhs.it;
   }
   bool operator == (const iterator & rhs) const
   {
       return it == rhs.it;
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const
   {
       return *it;
   }

   // prefix increment
   iterator & operator ++ ()
   {
       ++it;
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
       iterator temp(*this);
       ++(*this);
       return temp;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
       --it;
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
       iterator temp(*this);
       --(*this);
       return temp;
   }
   
private:
    
   typename custom::BST<T>::iterator it;
};


}; // namespace custom



