/***********************************************************************
 * Header:
 *    BINARY NODE
 * Summary:
 *    One node in a binary tree (and the functions to support them).
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        BNode         : A class representing a BNode
 *    Additionally, it will contain a few functions working on Node
 * Authors:
 * Jacob Johnson and Tyler Elms
 ************************************************************************/

#pragma once

#include <iostream>  // for OFSTREAM
#include <cassert>

/*****************************************************************
 * BNODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <class T>
class BNode
{
public:
   // 
   // Construct
   //
   BNode()
   {
       data = 0;
       pLeft = pRight = pParent = nullptr;
       
   }
   BNode(const T& t)
   {
       data = t;
       pLeft = pRight = pParent = nullptr;
       
   }
   BNode(T&& t)
   {
       data = std::move(t);
       pLeft = pRight = pParent = nullptr;
   }

   //
   // Data
   //
   BNode <T>* pLeft;
   BNode <T>* pRight;
   BNode <T>* pParent;
   T data;
};

/*******************************************************************
 * SIZE BTREE
 * Return the size of a b-tree under the current node
 *******************************************************************/
template <class T>
inline size_t size(const BNode <T> * p)
{
    if (p == nullptr) {
        return 0;
    }
    size_t leftSize = size(p->pLeft);
    size_t rightSize = size(p->pRight);
    
    return leftSize + rightSize + 1;
}


/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft(BNode <T> * pNode, BNode <T> * pAdd)
{
    if (pNode) {
        pNode->pLeft = pAdd;
        pAdd->pParent = pNode;
    }
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
inline void addRight (BNode <T> * pNode, BNode <T> * pAdd)
{
    if (pNode) {
        pNode->pRight = pAdd;
        pAdd->pParent = pNode;
    }
}

/******************************************************
 * ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <class T>
inline void addLeft (BNode <T> * pNode, const T & t)
{
    pNode->pLeft = new BNode<T>(t);
    pNode->pLeft->pParent = pNode;
}

template <class T>
inline void addLeft(BNode <T>* pNode, T && t)
{
    pNode->pLeft = new BNode<T>(std::move(t));
    pNode->pLeft->pParent = pNode;
}

/******************************************************
 * ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <class T>
void addRight (BNode <T> * pNode, const T & t)
{
    pNode->pRight = new BNode<T>(t);
    pNode->pRight->pParent = pNode;
}

template <class T>
void addRight(BNode <T>* pNode, T && t)
{
    pNode->pRight = new BNode<T>(std::move(t));
    pNode->pRight->pParent = pNode;
}

/*****************************************************
 * DELETE BINARY TREE
 * Delete all the nodes below pThis including pThis
 * using postfix traverse: LRV
 ****************************************************/
template <class T>
void clear(BNode <T> * & pThis)
{
    if (!pThis) {
        return;
    }
    clear(pThis->pLeft);
    clear(pThis->pRight);
    
    delete pThis;
    pThis = nullptr;
    
}

/***********************************************
 * SWAP
 * Swap the list from LHS to RHS
 *   COST   : O(1)
 **********************************************/
template <class T>
inline void swap(BNode <T>*& pLHS, BNode <T>*& pRHS)
{
    BNode<T> * temporary = pLHS;
    pLHS = pRHS;
    pRHS = temporary;
}

/**********************************************
 * COPY BINARY TREE
 * Copy pSrc->pRight to pDest->pRight and
 * pSrc->pLeft onto pDest->pLeft
 *********************************************/
template <class T>
BNode <T> * copy(const BNode <T> * pSrc)
{
    if (!pSrc) {
        return nullptr;
    }
    BNode<T>* newNode = new BNode<T>(pSrc->data);
    
    newNode->pLeft = copy(pSrc->pLeft);
    if (newNode->pLeft) {
        newNode->pLeft->pParent = newNode;
    }
    
    newNode->pRight = copy(pSrc->pRight);
    if (newNode->pRight) {
        newNode->pRight->pParent = newNode;
    }
    
    return newNode;
}

/**********************************************
 * assign
 * copy the values from pSrc onto pDest preserving
 * as many of the nodes as possible.
 *********************************************/
template <class T>
void assign(BNode <T> * & pDest, const BNode <T>* pSrc)
{
    if (!pSrc) {
        clear(pDest);
        return;
    }
    if (!pDest && pSrc) {
        pDest = new BNode<T>(pSrc->data);
        assign(pDest->pLeft,pSrc->pLeft);
        assign(pDest->pRight,pSrc->pRight);
        
    }
    
    
    pDest  = new BNode<T>(pSrc->data);
    assign(pDest->pLeft,pSrc->pLeft);
    assign(pDest->pRight,pSrc->pRight);
    
    if(pDest->pLeft) {
        pDest->pLeft->pParent = pDest;
    }
    if(pDest->pRight) {
        pDest->pRight->pParent = pDest;
    }
}
