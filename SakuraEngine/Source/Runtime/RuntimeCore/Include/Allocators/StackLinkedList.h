#ifndef stack_linked_list_H
#define stack_linked_list_H

template <class T>
class stack_linked_list {
public:
    struct Node {
        T data;
        Node* next;
    };
    
    Node* head;
public:
    stack_linked_list() = default;
    stack_linked_list(stack_linked_list &stack_linked_list) = delete;
    void push(Node * newNode);
    Node* pop();
};

#include "StackLinkedListImpl.h"

#endif /* stack_linked_list_H */

