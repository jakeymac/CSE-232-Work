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
 *    <Jacob Johnson and Tyler Elms>
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
    // Default Constructor
    Node()
    {
        pPrev = pNext = this;
    }

    // Copy Constructor
    Node(const Node<T> &other)
    {
        data = other.data;
        pPrev = pNext = this;
    }

    // Move Constructor
    Node(Node<T> &&other)
    {
        data = std::move(other.data);
        pPrev = pNext = this;
    }

    // Constructor
    Node(const T& value)
        : data(value)
    {
        pPrev = pNext = this;
    }

    T data;
    Node<T>* pNext;
    Node<T>* pPrev;
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
inline Node<T>* copy(const Node<T>* pSource)
{
    if (!pSource)
    {
        return nullptr;
    }

    Node<T>* pNewHead = new Node<T>;
    Node<T>* pCurrentSource = const_cast<Node<T>*>(pSource);
    Node<T>* pCurrentNew = pNewHead;

    while (pCurrentSource)
    {
        pCurrentNew->data = pCurrentSource->data;

        if (pCurrentSource->pNext)
        {
            pCurrentNew->pNext = new Node<T>;
            pCurrentNew->pNext->pPrev = pCurrentNew;
            pCurrentNew = pCurrentNew->pNext;
        }
        else
        {
            pCurrentNew->pNext = nullptr;
        }

        pCurrentSource = pCurrentSource->pNext;
    }

    return pNewHead;
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
inline void assign(Node<T>*& pDestination, const Node<T>* pSource)
{
    if (!pSource)
    {
        while (pDestination)
        {
            Node<T>* temp = pDestination;
            pDestination = pDestination->pNext;
            delete temp;
        }
        return;
    }

    Node<T>* pDestCurrent = pDestination;
    const Node<T>* pSourceCurrent = pSource;

    while (pSourceCurrent)
    {
        if (pDestCurrent)
        {
            pDestCurrent->data = pSourceCurrent->data;
        }
        else
        {
            pDestCurrent = new Node<T>;
            pDestCurrent->data = pSourceCurrent->data;

            if (pDestination)
            {
                pDestCurrent->pPrev = pDestination->pPrev;
                pDestination->pPrev->pNext = pDestCurrent;
                pDestination->pPrev = pDestCurrent;
            }
            else
            {
                pDestination = pDestCurrent;
            }
        }

        pDestCurrent = pDestCurrent->pNext;
        pSourceCurrent = pSourceCurrent->pNext;
    }

    while (pDestCurrent)
    {
        Node<T>* temp = pDestCurrent;
        pDestCurrent = pDestCurrent->pNext;
        delete temp;
    }
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(Node<T>*& pLHS, Node<T>*& pRHS)
{

    Node<T>* temp = pLHS;
    pLHS = pRHS;
    pRHS = temp;

    if (pLHS)
    {
        pLHS->pPrev = nullptr;
    }
    if (pRHS)
    {
        pRHS->pPrev = nullptr;
    }
}

/***********************************************
 * REMOVE
 * Remove the node pSource in the linked list
 *   INPUT  : pRemove - the node to be removed
 *   OUTPUT : Returns a pointer to the parent node
 *            of the removed node (or nullptr if pRemove is the head node)
 *   COST   : O(1)
 **********************************************/
template <class T>
inline Node<T>* remove(const Node<T>* pRemove)
{
    if (!pRemove)
    {
        return nullptr;
    }

    Node<T>* pPrevNode = pRemove->pPrev;
    Node<T>* pNextNode = pRemove->pNext;

    if (pPrevNode)
    {
        pPrevNode->pNext = pNextNode;
    }
    
    if (pNextNode)
    {
        pNextNode->pPrev = pPrevNode;
    }

    if (pRemove == pPrevNode)
    {
        return nullptr;
    }
    else
    {
        return pPrevNode;
    }
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
inline Node<T>* insert(Node<T>* pCurrent, const T& t, bool after = false)
{
    Node<T>* pNewNode = new Node<T>;
    pNewNode->data = t;

    if (!pCurrent)
    {
        return pNewNode;
    }

    if (after)
    {
        pNewNode->pNext = pCurrent->pNext;
        pNewNode->pPrev = pCurrent;

        if (pCurrent->pNext)
        {
            pCurrent->pNext->pPrev = pNewNode;
        }

        pCurrent->pNext = pNewNode;
    }
    else
    {
        pNewNode->pNext = pCurrent;
        pNewNode->pPrev = pCurrent->pPrev;

        if (pCurrent->pPrev)
        {
            pCurrent->pPrev->pNext = pNewNode;
        }

        pCurrent->pPrev = pNewNode;

        if (pCurrent == pCurrent->pPrev)
        {
            return pNewNode;
        }
    }

    return pCurrent->pPrev;
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
inline size_t size(const Node<T>* pHead)
{
    size_t count = 0;

    const Node<T>* currentNode = pHead;
    while (currentNode)
    {
        count++;
        currentNode = currentNode->pNext;
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
inline std::ostream& operator<<(std::ostream& out, const Node<T>* pHead)
{
    const Node<T>* currentNode = pHead;

    while (currentNode)
    {
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
inline void clear(Node<T>*& pHead)
{
    while (pHead)
    {
        Node<T>* temp = pHead;
        pHead = pHead->pNext;
        delete temp;
    }
}
