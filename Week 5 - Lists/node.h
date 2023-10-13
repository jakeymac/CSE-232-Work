/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *
 *    __    ___     ___    _   __
 *   /_ |  / _ \   / _ \  (_) / /
 *    | | | | | | | | | |    / / 
 *    | | | | | | | | | |   / /  
 *    | | | |_| | | |_| |  / / _ 
 *    |_|  \___/   \___/  /_/ (_)
 *                             
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#include <cassert>     // for ASSERT
#include <iostream>    // for NULL

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <class T>
class Node
{
public:

   //
   // Construct
   //

   Node(               ) : pNext(nullptr), pPrev(nullptr), data(               ) { }
   Node(const T &  data) : pNext(nullptr), pPrev(nullptr), data(data           ) { }
   Node(      T && data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }

   //
   // Member variables
   //

   T data;                 // user data
   Node <T> * pNext;       // pointer to next node
   Node <T> * pPrev;       // pointer to previous node
};

/***********************************************
 * COPY
 * Copy the list from the pSource and return
 * the new list
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline Node <T> * copy(const Node <T> * pSource) 
{
   // trivial case
   if (nullptr == pSource)
      return nullptr;
   
   try
   {
      Node <T> * pDestination = new Node <T> (pSource->data);
      const Node <T> * pSrc = pSource;        // counter through source
      Node <T> * pDes = pDestination;         // counter through destination

      for (pSrc = pSrc->pNext; pSrc; pSrc = pSrc->pNext)
         pDes = insert(pDes, pSrc->data, true /* after */);
      
      return pDestination;
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a new Node";
   }
}

/***********************************************
 * Assign
 * Copy the values from pSource into pDestination
 * reusing the nodes already created in pDestination if possible.
 *   INPUT  : the list to be copied
 *   OUTPUT : return the new list
 *   COST   : O(n)
 **********************************************/
template <class T>
inline void assign(Node <T> * & pDestination, const Node <T> * pSource)
{
   const Node <T> * pSrc;
   Node <T> * pDes = pDestination;
   Node <T> * pDesPrevious = nullptr;

   // replace the slots the are already filled using the assignment operator
   for (pSrc = pSource; pSrc != nullptr && pDes != nullptr; pSrc = pSrc->pNext)
   {
      pDes->data = pSrc->data;
      pDesPrevious = pDes;
      pDes = pDes->pNext;
   }

   // if there are still items the source list, then insert them on the end
   if (pSrc != nullptr)
   {
      // since pDes must equal nullptr, we need to set pDes to end of the list.
      pDes = pDesPrevious;
      // for each node extra that pSrc has but not pDes.
      for (; pSrc != nullptr; pSrc = pSrc->pNext)
      {
         // create a new node
         pDes = insert(pDes, pSrc->data, true /* after */);
         // if the destination list was previously empty, fill it now
         if (nullptr == pDestination)
            pDestination = pDes;
      }
   }
   // if there are still items in the destination list that are not needed, then delete them
   else if (pDes != nullptr)
   {
      bool setToNull = false;
      // make the previous item in the list the last
      if (pDes->pPrev)
         pDes->pPrev->pNext = nullptr;
      else
         setToNull = true;
      // now delete the list from here on out
      clear(pDes);
      if (setToNull)
         pDestination = nullptr;
   }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
   Node <T>* p = pLHS;
   pLHS = pRHS;
   pRHS = p;
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : the node to be removed
 *   OUTPUT : the pointer to the parent node
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node <T> * remove(const Node <T> * pRemove) 
{
   // trivial case
   if (NULL == pRemove)
      return nullptr;

   // update the pointers in pRemove->pPrev and pRemove->pNext
   if (pRemove->pPrev)
      pRemove->pPrev->pNext = pRemove->pNext;
   if (pRemove->pNext)
      pRemove->pNext->pPrev = pRemove->pPrev;

   // determine which node we will return.
   Node <T> * pReturn;
   if (pRemove->pPrev)
      pReturn = pRemove->pPrev;
   else
      pReturn = pRemove->pNext;

   // now delete the node and return
   delete pRemove;
   return pReturn;
}

/**********************************************
 * INSERT 
 * Insert a new node the the value in "t" into a linked
 * list immediately before the current position.
 *   INPUT   : t - the value to be used for the new node
 *             pCurrent - a pointer to the node before which
 *                we will be inserting the new node
 *             after - whether we will be inserting after
 *   OUTPUT  : return the newly inserted item
 *   COST    : O(1)
 **********************************************/
template <class T>
inline Node <T> * insert(Node <T> * pCurrent,
                  const T & t,
                  bool after = false)
{
   try
   {
      // allocate a new node
      Node <T> * pNew = new Node <T> (t);

      // do we insert before
      if (pCurrent != nullptr && after == false)
      {
         pNew->pNext = pCurrent;
         pNew->pPrev = pCurrent->pPrev;
         pCurrent->pPrev = pNew;
         if (pNew->pPrev)
            pNew->pPrev->pNext = pNew;
      }
      else if (pCurrent != nullptr && after == true)
      {
         pNew->pPrev = pCurrent;
         pNew->pNext = pCurrent->pNext;
         pCurrent->pNext = pNew;
         if (pNew->pNext)
            pNew->pNext->pPrev = pNew;
      }
      
      return pNew;
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a new Node";
   }
}

/******************************************************
 * FIND
 * Find a given node in an unsorted linked list.  Return
 * a pointer to the node if it is found, NULL otherwise.
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : a pointer to the node if it is found
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
   size_t s = 0;
   // loop through the linked list
   for (const Node <T> * p = pHead; p != nullptr; p = p->pNext)
      s++;

   return s;
}

/***********************************************
 * DISPLAY
 * Display all the items in the linked list from here on back
 *    INPUT  : the output stream
 *             pointer to the linked list
 *    OUTPUT : the data from the linked list on the screen
 *    COST   : O(n)
 **********************************************/
template <class T>
inline std::ostream & operator << (std::ostream & out, const Node <T> * pHead)
{
   // paranoia
   if (pHead == nullptr)
      return out;

   // display the data
   out << pHead->data;

   // if there is something after us, display it
   if (pHead->pNext)
      return out << ", " << pHead->pNext;
   else
      return out;      
}

/*****************************************************
 * CLEAR
 * Free all the data currently in the linked list
 *   INPUT   : pointer to the head of the linked list
 *   OUTPUT  : pHead set to NULL
 *   COST    : O(n)
 ****************************************************/
template <class T>
inline void clear(Node <T> * & pHead)
{
   Node <T> * pNext;
   for (Node <T> * p = pHead; p; p = pNext)
   {
      pNext = p->pNext;
      delete p;
   }
   pHead = nullptr;
}


