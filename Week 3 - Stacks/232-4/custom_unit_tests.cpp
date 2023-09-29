//
//  custom_unit_tests.cpp
//  LabStack
//
//  Created by Jacob on 9/29/23.
//

#include "custom_unit_tests.hpp"
#include "stack.h"
#include <iostream>


int main() {
    //Test 1
    custom::stack<int> stack1;
    stack1.container.resize(4);
    stack1.container[0] = 5;
    stack1.container[1] = 12;
    stack1.container[2] = 3;
    stack1.container[3] = 9;
    
    stack1.pop();
    assert(stack1.size() == 3);
    assert(stack1.top() == 3);
    
    delete[] stack1;
    
    //Test 2
    custom::stack<int> stack2;
    stack2.container.resize(6);
    stack2.container[0] = 1;
    stack2.container[1] = 11;
    stack2.container[2] = 8;
    stack2.container[3] = 15;
    stack2.container[4] = 2;
    stack2.container[5] = 9;
    
    stack2.pop();
    std::cout << Spy::numDelete() << std::endl;
    stack2.pop();
    assert(stack2.size() == 4);
    assert(stack2.top() == 15);
    
    delete[] stack2;
    
    //Test 3
    custom stack<int> stack3;
    stack3.pop();
    std::cout << Spy::numDelete() << std::endl;
    
    
    //Test 4
    
    
    //Test 5
}

