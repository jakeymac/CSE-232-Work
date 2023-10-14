/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
 
class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:
   //
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list()
   {
   }

   //
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);
   void swap(list <T>& rhs);

   //
   // Iterator
   //

    class  iterator;
    iterator begin()  { return iterator(pHead); }
    iterator rbegin() { return iterator(pTail); }
    iterator end() {
        if (!pTail) {
            return nullptr;
        }
        
        return iterator(pTail->pNext);
    }

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   //
   // Status
   //

   bool empty()  const { return (pHead == nullptr); }
   size_t size() const { return numElements;   }


private:
   // nested linked list class
   class Node;

   // member variables
   size_t numElements; // though we could count, it is faster to keep a variable
   Node * pHead;    // pointer to the beginning of the list
   Node * pTail;    // pointer to the ending of the list
};

/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   //
   // Construct
   //
    Node(               ) : pNext(nullptr), pPrev(nullptr), data(               ) { }
    Node(const T &  data) : pNext(nullptr), pPrev(nullptr), data(data           ) { }
    Node(      T && data) : pNext(nullptr), pPrev(nullptr), data(std::move(data)) { }
   //
   // Data
   //

   T data;                 // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator()
   {
       //Node * newNode = new list<T> :: Node();
       p = nullptr;
   }
   iterator(Node * p)
   {
       this->p = p;
   }
   iterator(const iterator  & rhs)
   {
       p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
       if (this != &rhs) {
           p = rhs.p;
       }
       
       return *this;
   }
   
   // equals, not equals operator
    bool operator == (const iterator & rhs) const { return (p == rhs.p); }
    bool operator != (const iterator & rhs) const { return !(p == rhs.p); }

   // dereference operator, fetch a node
   T & operator * ()
   {
       return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
       iterator original = *this;
       if (p) {
           p = p->pNext;
       }
       return original;
   }

   // prefix increment
   iterator & operator ++ ()
   {
       p = p->pNext;
       return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
       iterator original = *this;
      if(p) {
           p = p->pPrev;
       }
       return original;
   }

   // prefix decrement
   iterator & operator -- ()
   {
       if (p) {
           p = p->pPrev;
       }
       
       return *this;
   }

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it,       T && data);
   friend iterator list <T> :: erase(const iterator & it);

private:

   typename list <T> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) : numElements(num), pHead(nullptr), pTail(nullptr) {

    for (size_t i =0; i< num; i++) {
        list<T>:: Node * newNode = new list<T>::Node(t);
        
        if (!pHead) {
            pHead = newNode;
            pTail = newNode;
        } else {
            newNode->pPrev = pTail;
            pTail->pNext = newNode;
            pTail = newNode;
        }
    }
}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list<T>::list(Iterator first, Iterator last) : numElements(0), pHead(nullptr), pTail(nullptr) {
    Node* previous = nullptr;
    
    for (Iterator it = first; it != last; ++it) {
        Node* current = new list<T>::Node(*it); // Create a new node with data from the iterator
        
        if (!pHead) {
            pHead = current;
        } else {
            current->pPrev = previous;
            previous->pNext = current;
        }
        
        previous = current;
        ++numElements;
    }
    
    pTail = previous;
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il): numElements(0), pHead(nullptr), pTail(nullptr) {

    for (const T& item: il) {
        push_back(item);
    }
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num): numElements(0), pHead(nullptr), pTail(nullptr) {

    for (size_t i = 0; i < num; ++i) {
        push_back(T());
    }
}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list()
{
    numElements = 0;
    pHead = pTail = nullptr;
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) : numElements(0), pHead(nullptr),pTail(nullptr) {
    for (list<T>:: Node* current = rhs.pHead; current; current = current->pNext) {
        push_back(current->data);
    }
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs) : numElements(rhs.numElements), pHead(rhs.pHead), pTail(rhs.pTail) {

    rhs.numElements = 0;
    rhs.pHead = nullptr;
    rhs.pTail = nullptr;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
   
    clear();
    
    numElements = rhs.numElements;
    pHead = rhs.pHead;
    pTail = rhs.pTail;
    
    rhs.numElements = 0;
    rhs.pHead = rhs.pTail = nullptr;

    
    return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{

    clear();
    for (list<T>::Node* current = rhs.pHead; current; current = current->pNext) {
        push_back(current->data);
    }
    return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
    clear();
    for (const T& item: rhs) {
        push_back(item);
    }
    return *this;
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{
    numElements = 0;
    list<T> :: Node * current = pHead;
    while (current != nullptr) {
        list<T> :: Node * next = current->pNext;
        delete current;
        current = next;
    }
    pHead = nullptr;
    pTail = nullptr;
    
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_back(const T & data)
{
    list<T>:: Node* newNode = new list<T>:: Node(data);
    if(!pTail) {
        pHead = newNode;
        pTail = newNode;
    } else {
        pTail->pNext = newNode;
        newNode->pPrev = pTail;
        pTail = newNode;
    }
    ++numElements;
}

template <typename T>
void list <T> ::push_back(T && data)
{
    list<T>:: Node* newNode = new list<T>:: Node(std::forward<T>(data));
    if(!pTail) {
        pHead = newNode;
        pTail = newNode;
    } else {
        pTail->pNext = newNode;
        newNode->pPrev = pTail;
        pTail = newNode;
    }
    ++numElements;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> :: push_front(const T & data)
{
    list<T>::Node * newNode = new list<T> :: Node(data);
    if(!pHead) {
        pHead = pTail = newNode;
    }
    else {
        newNode->pNext = pHead;
        pHead->pPrev= newNode;
        pHead = newNode;
    }
    ++numElements;
}

template <typename T>
void list <T> ::push_front(T && data)
{
    list<T>::Node * newNode = new list<T> :: Node(std::forward<T>(data));
    if(!pHead) {
        pHead = pTail = newNode;
    }
    else {
        newNode->pNext = pHead;
        pHead->pPrev= newNode;
        pHead = newNode;
    }
    ++numElements;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
    if(pTail) {
        if (pTail->pPrev) {
            list<T>::Node* newTail = pTail->pPrev;
            pTail->pPrev->pNext = nullptr;
            pTail = newTail;
        } else {
            pTail= pHead = nullptr;
        }
        --numElements;
    }
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
    if (pHead) {
        if (pHead->pNext) {
            list<T>::Node* newHead = pHead->pNext;
            pHead->pNext->pPrev = nullptr;
            pHead = newHead;
        } else {
            pHead = pTail = nullptr;
        }
        --numElements;
    }
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: front()
{
    if(pHead) {
        return pHead->data;
    }
    static T default_value;
    return default_value;
    
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  :
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
    if(pHead) {
        return pTail->data;
    }
    static T default_value;
    return default_value;
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
    if(it == end() || it.p == nullptr) {
        return end();
    }
    
    list<T>::Node* target = it.p;
    list<T>::Node* prevNode = target->pPrev;
    list<T>::Node* nextNode = target->pNext;
    
    if(prevNode) {
        prevNode->pNext = nextNode;
        
    } else {
        pHead = nextNode;
    }
    
    if (nextNode) {
        nextNode->pPrev = prevNode;
    } else {
        pTail = prevNode;
    }
    
    delete target;
    numElements--;
    

    return list<T>::iterator(nextNode);
    
    return list<T>::iterator(prevNode);
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data)
{
    list<T>::Node* newNode = new list<T>::Node(data);

        if (empty()) {
            // If the list is empty, set pHead and pTail to the new node
            pHead = pTail = newNode;
            numElements = 1;
            return iterator(newNode);
        }

        if (!it.p) {
            // Insert at the end
            pTail->pNext = newNode;
            newNode->pPrev = pTail;
            pTail = newNode;
            numElements++;
            return iterator(newNode);
        } else {
            // Insert at a specific position
            list<T>::Node* prevNode = it.p->pPrev;
            newNode->pNext = it.p;
            newNode->pPrev = prevNode;
            
            if (prevNode) {
                prevNode->pNext = newNode;
            } else {
                // If inserting at the beginning, update pHead
                pHead = newNode;
            }

            it.p->pPrev = newNode;
            numElements++;
            return iterator(newNode);
    }
    
    
}


template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
    list<T>::Node* newNode = new list<T>::Node(std::forward<T>(data));

        if (empty()) {
            // If the list is empty, set pHead and pTail to the new node
            pHead = pTail = newNode;
            numElements = 1;
            return iterator(newNode);
        }

        if (!it.p) {
            // Insert at the end
            pTail->pNext = newNode;
            newNode->pPrev = pTail;
            pTail = newNode;
            numElements++;
            return iterator(newNode);
        } else {
            // Insert at a specific position
            list<T>::Node* prevNode = it.p->pPrev;
            newNode->pNext = it.p;
            newNode->pPrev = prevNode;
            
            if (prevNode) {
                prevNode->pNext = newNode;
            } else {
                // If inserting at the beginning, update pHead
                pHead = newNode;
            }

            it.p->pPrev = newNode;
            numElements++;
            return iterator(newNode);
    }
    
    
}



/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
    std::swap(lhs.pHead,rhs.pHead);
    std::swap(lhs.pTail,rhs.pTail);
    std::swap(lhs.numElements, rhs.numElements);
}

template <typename T>
void list<T>::swap(list <T>& rhs)
{
    std::swap(pHead,rhs.pHead);
    std::swap(pTail,rhs.pTail);
    std::swap(numElements,rhs.numElements);
}

//#endif
}; // namespace custom
