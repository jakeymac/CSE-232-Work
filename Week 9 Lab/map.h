/***********************************************************************
 * Header:
 *    map
 * Summary:
 *    Our custom implementation of a std::map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    <Tyler Elms and Jacob Johnson>
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   using Pairs = custom::pair<K, V>;

    //
    // Construct
    //
    map() = default;

    map(const map& rhs) : bst(rhs.bst)
    {
    }

    map(map&& rhs) noexcept : bst(std::move(rhs.bst))
    {
    }

    template <class Iterator>
    map(Iterator first, Iterator last) : bst()
    {
        while (first != last) {
            bst.insert(*first);
            ++first;
        }
    }

    map(const std::initializer_list<Pairs>& il) : bst(il)
    {
    }

    ~map()
    {
        clear();
    }


   //
   // Assign
   //
    map &operator=(const map &rhs)
    {
       if (this != &rhs)
       {
          bst = rhs.bst;
       }
       return *this;
    }

    map &operator=(map &&rhs) noexcept
    {
       if (this != &rhs)
       {
          bst = std::move(rhs.bst);
       }
       return *this;
    }

    map &operator=(const std::initializer_list<Pairs> &il)
    {
        bst.clear();
       bst = il;
       return *this;
    }

   
   //
   // Iterator
   //
   class iterator;
   iterator begin()
   {
      return iterator();
   }
   iterator end()
   {
      return iterator();
   }

   //
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K& k) const;
         V & at (const K& k);
    iterator find(const K& k)
    {
        // Create a pair with the key to perform the search in the BST
        Pairs searchPair(k, V());

        // Call the BST's find function and return an iterator
        return iterator(bst.find(searchPair));
    }
    
   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
       // Assuming your insert method takes a std::pair<K, V>
      auto result = bst.insert(std::move(rhs));

              // Assuming your insert method returns a pair
      return make_pair(iterator(result.first), result.second);
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
       auto result = bst.insert(rhs);

               // Assuming your insert method returns a pair
       return make_pair(iterator(result.first), result.second);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       while (first != last) {
           insert(std::move(*first));
           ++first;
       }
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
       for (const auto& pair: il) {
           insert(std::move(pair));
       }
   }

   //
   // Remove
   //
   void clear() noexcept
   {
   }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept { return bst.empty(); }
   size_t size() const noexcept { return bst.size(); }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map<K, V>::iterator
{
   friend class ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend class custom::map;

public:
   //
   // Construct
   //
   iterator() = default;

   // Constructor from BST iterator
   iterator(const typename BST<pair<K, V>>::iterator& rhs) : it(rhs)
   {
   }

   // Copy constructor
   iterator(const iterator& rhs) : it(rhs.it)
   {
   }

   //
   // Assign
   //
   iterator& operator=(const iterator& rhs)
   {
      if (this != &rhs)
         it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator==(const iterator& rhs) const
   {
      return it == rhs.it;
   }

   bool operator!=(const iterator& rhs) const
   {
      return !(*this == rhs);
   }

   //
   // Access
   //
    const pair<K, V>& operator*() const
    {
        return it.pNode->data;
    }
    
   //
   // Increment
   //
   iterator& operator++()
   {
      ++it;
      return *this;
   }

   iterator operator++(int postfix)
   {
      iterator temp(*this);
      ++(*this);
      return temp;
   }

   iterator& operator--()
   {
      --it;
      return *this;
   }

   iterator operator--(int postfix)
   {
      iterator temp(*this);
      --(*this);
      return temp;
   }

private:
   // Member variable
   typename BST<pair<K, V>>::iterator it;
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map<K, V>::operator[](const K& key)
{
    // Try to find the key in the map
    iterator it = find(key);

    // If the key exists, return a reference to its value
    if (it != end()) {
        return const_cast<V&>(it.operator*().second);
    }
    // If the key doesn't exist, insert a default-constructed pair and return a reference to its value
       auto result = insert({key, V()});
       return const_cast<V&>(result.first.it.operator*().second);
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map<K, V>::operator[](const K& key) const
{
    const iterator it = find(key);
    
    if (it != end()) {
        return (*it).second;
    }
    throw std::out_of_range("invalid map<K, T> key");

}


/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map<K, V>::at(const K& key)
{
    iterator it = find(key);

    if (it != end()) {
        return const_cast<V&>(it.operator*().second);
    }

    throw std::out_of_range("invalid map<K, T> key");
}


/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map<K, V> ::at(const K& key) const
{
    iterator it = find(key);

    if (it != end()) {
        return it.operator*().second;
    }

    throw std::out_of_range("invalid map<K, T> key");
}


/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
    using std::swap;

    swap(lhs.bst, rhs.bst);
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
    iterator it = find(k);

       if (it != end())
       {
          erase(it);

          return 1;
       }

       return 0;
}


/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   while (first != last)
   {
      first = erase(first);
   }
   return last;
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
    if (it == end()) {
          
          return iterator();
       }

       typename BST<pair<K, V>>::iterator bstIterator = it.it;

       typename BST<pair<K, V>>::iterator nextBstIterator = bst.erase(bstIterator);

       return iterator(nextBstIterator);
}

}; //  namespace custom
