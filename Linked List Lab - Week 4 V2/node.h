/***********************************************************************
 * Header:
 *    NODE
 * Summary:
 *    One node in a linked list (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        Node         : A class representing a Node
 *    Additionally, it will contain a few functions working on Node
 * Author
 *    <your names here>
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

    Node() : data(T()), pNext(nullptr), pPrev(nullptr){}
    Node(const T &  data) : data(data), pNext(nullptr), pPrev(nullptr) {}
    Node(T&& data) : data(std::move(data)), pNext(nullptr), pPrev(nullptr) {}

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
        if (!pSource) {
            return nullptr;
        }

        Node<T>* pNewList = new Node<T>(pSource->data);

        Node<T>* pNewNode = pNewList;
        pSource = pSource->pNext;

        while (pSource) {
            pNewNode->pNext = new Node<T>(pSource->data);
            pNewNode->pNext->pPrev = pNewNode;
            pNewNode = pNewNode->pNext;
            pSource = pSource->pNext;
        }

        return pNewList;
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
//    Node<T>* pDestCurrent = pDestination;
//    
//    while (pSource != nullptr) {
//        if (pDestCurrent == nullptr) {
//            pDestCurrent = new Node<T>(pSource->data);
//        } else {
//            pDestCurrent->data = pSource->data;
//        }
//        
//        pSource = pSource->pNext;
//        if (pDestCurrent != nullptr) {
//            pDestCurrent = pDestCurrent->pNext;
//        }
//    }
//    
//    while (pDestCurrent != nullptr) {
//        Node<T>* temp = pDestCurrent;
//        pDestCurrent = pDestCurrent->pNext;
//        delete temp;
//    }
//    
//    pDestination = pDestination ? pDestination : pDestCurrent;
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node <T>* &pLHS, Node <T>* &pRHS)
{
    std::swap(pLHS, pRHS);
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
    if (!pRemove) {
        return nullptr;
    }

    Node<T>* pRoot = pRemove->pPrev;

    if (pRoot) {
        pRoot ->pNext = pRemove->pNext;
    }

    if (pRemove->pNext) {
        pRemove->pNext->pPrev = pRoot; // Update the previous pointer of the next node
    }

    delete pRemove;

    return pRoot;
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
        Node<T>* pNewNode = new Node<T>(t);

        if (!pCurrent) {
            pNewNode->pNext = nullptr;
            pNewNode->pPrev = nullptr;
            return pNewNode;
        }

        if (after) {
            pNewNode->pNext = pCurrent->pNext;
            pNewNode->pPrev = pCurrent;
            if (pCurrent->pNext) {
                pCurrent->pNext->pPrev = pNewNode;
            }
            pCurrent->pNext = pNewNode;
        } else {
            pNewNode->pNext = pCurrent;
            pNewNode->pPrev = pCurrent->pPrev;
            if (pCurrent->pPrev) {
                pCurrent->pPrev->pNext = pNewNode;
            }
            pCurrent->pPrev = pNewNode;
        }

        return pNewNode;
    }

/******************************************************
 * SIZE
 * Find the size an unsorted linked list.  
 *  INPUT   : a pointer to the head of the linked list
 *            the value to be found
 *  OUTPUT  : number of nodes
 *  COST    : O(n)
 ********************************************************/
template <class T>
inline size_t size(const Node <T> * pHead)
{
    size_t count = 0;
    while (pHead) {
        count++;
        pHead = pHead->pNext;
    }

    return count;
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
    const Node<T>* currentNode = pHead;

    while (currentNode) {
        out << currentNode->data << " ";
        currentNode = currentNode->pNext;
    }

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
    while (pHead) {
        Node<T>* temp = pHead;
        pHead = pHead->pNext;
        delete temp;
    }
    pHead = nullptr;
}


