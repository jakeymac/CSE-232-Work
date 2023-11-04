/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair
#include <iostream>

class TestBST; // forward declaration for unit tests
class TestMap;
class TestSet;

namespace custom
{

template <class TT>
class set;
template <class KK, class VV>
class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
    friend class ::TestBST; // give unit tests access to the privates
    friend class ::TestMap;
    friend class ::TestSet;
    
    template <class KK, class VV>
    friend class map;
    
    template <class TT>
    friend class set;
    
    template <class KK, class VV>
    friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
    //
    // Construct
    //
    
    BST();
    BST(const BST &  rhs);
    BST(      BST && rhs);
    BST(const std::initializer_list<T>& il);
    ~BST();
    
    //
    // Assign
    //
    
    BST & operator = (const BST &  rhs);
    BST & operator = (      BST && rhs);
    BST & operator = (const std::initializer_list<T>& il);
    void swap(BST & rhs);
    
    //
    // Iterator
    //
    
    class iterator;
    iterator   begin() const noexcept;
    iterator   end()   const noexcept { return iterator(nullptr); }
    
    //
    // Access
    //
    
    iterator find(const T& t);
    
    //
    // Insert
    //
    
    std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
    std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);
    
    //
    // Remove
    //
    
    iterator erase(iterator& it);
    void   clear() noexcept;
    
    
    //
    // Status
    //
    
    bool   empty() const noexcept { return (root == nullptr); }
    size_t size()  const noexcept { return numElements;   }
    
    
private:
    
    class BNode;
    BNode * root;              // root node of the binary search tree
    size_t numElements;        // number of elements currently in the tree
    
    void clearNode(BNode* currentNode) noexcept;
    BNode* copyTree(const BNode* rhsNode);
    BNode* insertRecursive(BNode*& node, BNode* parent, const T& t,  bool keepUnique);
    BNode* insertRecursive(BNode*& node, BNode* parent, T&& t, bool keepUnique);

};

/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
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
    BNode(const T &  t)
    {
        data = t;
        pLeft = pRight = pParent = nullptr;
    }
    BNode(T && t)
    {
        data = std::move(t);
        pLeft = pRight = pParent = nullptr;
    }
    
    //
    // Insert
    //
    void addLeft (BNode * pNode);
    void addRight(BNode * pNode);
    void addLeft (const T &  t);
    void addRight(const T &  t);
    void addLeft(       T && t);
    void addRight(      T && t);
    
    //
    // Status
    //
    bool isRightChild(BNode * pNode) const {
        if (pParent) {
            return pParent->pRight == this;
        }
        return false;
    }
    bool isLeftChild( BNode * pNode) const {
        if (pParent) {
            return pParent->pRight == this;
        }
        return false;
    }
    
    //
    // Data
    //
    T data;                  // Actual data stored in the BNode
    BNode* pLeft;          // Left child - smaller
    BNode* pRight;         // Right child - larger
    BNode* pParent;        // Parent
    bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
    friend class ::TestBST; // give unit tests access to the privates
    friend class ::TestMap;
    friend class ::TestSet;
    
    template <class KK, class VV>
    friend class map;
    
    template <class TT>
    friend class set;
public:
    // constructors and assignment
    iterator(BNode * p = nullptr): pNode(p) {}
    iterator(const iterator & rhs): pNode(rhs.pNode) {}
    
    iterator & operator = (const iterator & rhs)
    {
        if(this == &rhs) {
            return *this;
        }
        pNode = rhs.pNode;
        return *this;
    }
    
    // compare
    bool operator == (const iterator & rhs) const
    {
        return pNode == rhs.pNode;
    }
    
    
    bool operator != (const iterator & rhs) const
    {
        return pNode != rhs.pNode;
    }
    
    // de-reference. Cannot change because it will invalidate the BST
    const T & operator * () const
    {
        if(!pNode) {
            throw std::out_of_range("End Iterator error");
        }
        
        return pNode->data;
        
    }
    
    // increment and decrement
    iterator & operator ++ ();
    iterator   operator ++ (int postfix)
    {
        iterator temp = *this;
        ++(*this);
        return temp;
    }
    iterator & operator -- ();
    iterator   operator -- (int postfix)
    {
        iterator temp = *this;
        --(*this);
        return temp;
    }
    
    BNode* getNode() {
        return pNode;
    }
    
    // must give friend status to remove so it can call getNode() from it
    friend BST <T> :: iterator BST <T> :: erase(iterator & it);
    
private:
    
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


/*********************************************
 * BST :: DEFAULT CONSTRUCTOR
 ********************************************/
template <typename T>
BST <T> ::BST()
{
    numElements = 0;
    root = nullptr;
}


BNode* findParentNode(BNode* current, BNode* target) {
    if (!current || current == target) {
        return nullptr;
    }
    
    if ((current->pLeft == target) || (current->pRight == target)) {
        return current;
    }
    
    if(target->data < current->data) {
        return findParentNode(current->pLeft, target);
    } else {
        return findParentNode(current->pRight, target);
    }
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) : numElements(rhs.numElements), root(copyTree(rhs.root)){}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs)
{
    root = rhs.root;
    numElements = rhs.numElements;
    
    rhs.root = nullptr;
    rhs.numElements = 0;
}

/*********************************************
 * BST :: INITIALIZER LIST CONSTRUCTOR
 * Create a BST from an initializer list
 ********************************************/

template <typename T>
BST<T> ::BST(const std::initializer_list<T>& il) : numElements(0), root(nullptr)
{
    for (const T& value : il) {
        insert(value);
    }
}


/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
    clear();
}

template <typename T>
typename BST<T>::BNode* BST<T>::copyTree(const BNode* rhsNode) {
    if (!rhsNode) {
        return nullptr;
    }
    
    BNode* newNode = new BNode(rhsNode->data);
    newNode->pLeft = copyTree(rhsNode->pLeft);
    newNode->pRight = copyTree(rhsNode->pRight);
    
    return newNode;
}
/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
    if (this == &rhs) {
        return *this;
    }
    
    clear();
    
    if (rhs.root) {
        root = copyTree(rhs.root);
    }
    
    return *this;
}



/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
    clear();
    for (const T& value: il) {
        insert(value);
    }
    return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
    
    clear();
    swap(rhs);
    
    rhs.root = nullptr;
    return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
    std::swap(numElements, rhs.numElements);
    std::swap(root, rhs.root);
}

template <typename T>
typename BST<T>::BNode* BST<T>::insertRecursive(BNode*& node, BNode* parent, const T& t,  bool keepUnique) {
    if (!node) {
        node = new BNode(t);
        node->pParent = parent;
        ++numElements;
        return node;
    }
    
    if (t == node->data) {
        if (!keepUnique)
        {
            node->data = t;
            return node;
        }
        return nullptr;
    } else if (t < node->data) {
        return insertRecursive(node->pLeft, node, t, keepUnique);
    } else {
        return insertRecursive(node->pRight, node, t, keepUnique);
    }
}

template <typename T>
typename BST<T>::BNode* BST<T>::insertRecursive(BNode*& node, BNode* parent, T&& t, bool keepUnique) {
    if (!node) {
        node = new BNode(std::forward<T>(t));
        node->pParent = parent;
        ++numElements;
        return node;
    }
    
    if (t==node->data) {
        if(!keepUnique) {
            node->data = std::forward<T>(t);
            return node;
        }
        return nullptr;
    } else if (t < node->data) {
        return insertRecursive(node->pLeft, node, std::forward<T>(t), keepUnique);
    } else {
        return insertRecursive(node->pRight,node,std::forward<T>(t),keepUnique);
    }
}


/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/

template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
    
        std::pair<iterator,bool> pairReturn(end(), false);
        try
        {
            if (root == nullptr) {
                assert(numElements==0);
                root = new BNode(t);
                numElements = 1;
                pairReturn.first = iterator(root);
                pairReturn.second = true;
            }
            
            BNode* node = root;
            bool done = false;
            while (!done) {
                if (keepUnique && t == node->data) {
                    pairReturn.first = iterator(node);
                    pairReturn.second = false;
                    return pairReturn;
                }
                
                if (t < node->data) {
                    if (node->pLeft) {
                        node = node->pLeft;
                    } else {
                        node->addLeft(t);
                        done = true;
                        pairReturn.first = iterator(node->pLeft);
                        pairReturn.second = true;
                    }
                } else {
                    if (node->pRight) {
                        node = node->pRight;
                    } else {
                        node->addRight(t);
                        done = true;
                        pairReturn.first = iterator(node->pRight);
                        pairReturn.second = true;
                    }
                }
                
                assert(root != nullptr);
                numElements++;
                while(root->pParent) {
                    root = root->pParent;
                    
                }
                assert(root->pParent == nullptr);
            }
        }
        catch (const char* errorMsg) {
            std::cerr << "Caught exception: " << errorMsg << std::endl;
        }
        return pairReturn;
    
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST<T>::iterator BST<T>::erase(iterator& it)
{
    if (it == end()) {
        return end();
    }

    BNode* nodeToDelete = it.getNode();

    if (!nodeToDelete) {
        return end();
    }

    BNode* parent = nodeToDelete->pParent;
    BNode* leftChild = nodeToDelete->pLeft;
    BNode* rightChild = nodeToDelete->pRight;

    BNode* successor = nullptr; // Declare successor here

    if (!leftChild && !rightChild) {
        if (parent) {
            if (parent->pLeft == nodeToDelete) {
                parent->pLeft = nullptr;
            } else {
                parent->pRight = nullptr;
            }
        } else {
            root = nullptr;
        }
    } else if (!leftChild && rightChild) {
        if (parent) {
            if (parent->pLeft == nodeToDelete) {
                parent->pLeft = rightChild;
            } else {
                parent->pRight = rightChild;
            }
            rightChild->pParent = parent;
        } else {
            root = rightChild;
            rightChild->pParent = nullptr;
        }
    } else {
        successor = rightChild;
        while (successor->pLeft) {
            successor = successor->pLeft;
        }

        if (successor->pParent != nodeToDelete) {
            successor->pParent->pLeft = successor->pRight;
            if (successor->pRight) {
                successor->pRight->pParent = successor->pParent;
            }
            successor->pRight = rightChild;
            rightChild->pParent = successor;
        }

        if (parent) {
            if (parent->pLeft == nodeToDelete) {
                parent->pLeft = successor;
            } else {
                parent->pRight = successor;
            }
        } else {
            root = successor;
        }

        successor->pParent = parent;
        successor->pLeft = leftChild;
        leftChild->pParent = successor;
    }
    delete nodeToDelete;
    --numElements;

    return iterator(successor);
}



/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
    clearNode(root);
    root = nullptr;
    numElements = 0;
}

template <typename T>
void BST <T>:: clearNode(BNode* currentNode) noexcept {
    if (currentNode) {
        clearNode(currentNode->pLeft);
        clearNode(currentNode->pRight);
        delete currentNode;
    }
}


/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
    if (!root) {
        return end();
    }
    BNode * currentNode = root;
    while(currentNode->pLeft) {
        currentNode = currentNode->pLeft;
    }
    return currentNode;
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
        BNode* currentNode = root;

        while (currentNode != nullptr) {
            if (t == currentNode->data) {
                // Found the element, return an iterator pointing to it.
                return iterator(currentNode);
            } else if (t < currentNode->data) {
                // The element might be in the left subtree.
                currentNode = currentNode->pLeft;
            } else {
                // The element might be in the right subtree.
                currentNode = currentNode->pRight;
            }
        }

        // Element not found, return the end iterator.
        return end();
    }


/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
    pLeft = pNode;
    if (pNode) {
        pNode->pParent = this;
    }
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    pRight = pNode;
    if (pNode) {
        pNode->pParent = this;
    }
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    BNode* newNode = new BNode(t);
    addLeft(newNode);
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    BNode* newNode = new BNode(t);
    addRight(newNode);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
    BNode* newNode = new BNode(std::forward<T>(t));
    addLeft(newNode);
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
    BNode* newNode = new BNode(std::forward<T>(t));
    addRight(newNode);
}



/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/     

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/



template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
    if (!pNode) {
        return *this;
    }
    
    if (pNode->pRight) {
        pNode = pNode->pRight;
        while(pNode->pLeft) {
            pNode = pNode->pLeft;
        }
    }
        else {
            BNode* parent = pNode->pParent;
            while(parent && pNode == parent->pRight) {
                pNode = parent;
                parent = parent->pParent;
            }
            pNode = parent;
        }
        return *this;
    }


/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
    {
        if (!pNode) {
            return *this;
        }
        
        if (pNode->pLeft) {
            pNode = pNode->pLeft;
            while (pNode->pRight) {
                pNode = pNode->pRight;
            }
        }
        else {
            BNode* parent = pNode->pParent;
            while (parent && pNode == parent->pLeft) {
                pNode = parent;
                parent = parent->pParent;
            }
            pNode = parent;
        }
        return *this;
    }
}


} // namespace custom


