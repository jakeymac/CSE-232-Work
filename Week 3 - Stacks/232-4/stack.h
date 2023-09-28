/***********************************************************************
 * Module:
 *    Stack
 * Summary:
 *    Our custom implementation of std::stack
 *
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *       stack             : similar to std::stack
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
//#include "vector.h"
#include <vector>

class TestStack; // forward declaration for unit tests

namespace custom
{

/**************************************************
 * STACK
 * First-in-Last-out data structure
 *************************************************/
template<class T>
class stack
{
   friend class ::TestStack; // give unit tests access to the privates
public:
  
   // 
   // Construct
   // 

    stack()                            { std::vector<T> container; }
    stack(const stack <T> &  rhs)      { std::vector<T> container = rhs.container; }
    stack(      stack <T> && rhs)      { std::vector<T> container = rhs.container; }
    stack(const std::vector<T> &  rhs) { std::vector<T> container = rhs; }
    stack(      std::vector<T> && rhs) { std::vector<T> container = rhs; }
   ~stack()                           {                      }

   //
   // Assign
   //

   stack <T> & operator = (const stack <T> & rhs)
   {
       container = rhs.container;
       return *this;
   }
   stack <T>& operator = (stack <T> && rhs)
   {
       container = rhs.container;
       return *this;
   }
   void swap(stack <T>& rhs)
   {
       std::vector<T> oldContainer = container;
       container = rhs.container;
       rhs.container = oldContainer;
   }

   // 
   // Access
   //

    T& top() {
        if (container.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return container.back();
    }
    const T& top() const {
        if (container.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return container.back();
    }

   // 
   // Insert
   // 

    void push(const T&  t) { container.push_back(t); }
    void push(      T&& t) { container.push_back(t); }

   //
   // Remove
   //

   void pop() 
   { 
       container.pop_back();
   }

   //
   // Status
   //
    size_t  size () const { return container.size();  }
    bool empty   () const { return container.size() == 0; }
   
private:
   
  std::vector<T> container;  // underlying container
};



} // custom namespace


