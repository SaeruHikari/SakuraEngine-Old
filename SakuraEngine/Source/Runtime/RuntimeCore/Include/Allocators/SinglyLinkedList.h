#pragma once

template <class T>
class singly_linked_list {
public:
    struct Node {
        T data;
        Node * next;
    };
    
    Node * head;
    
public:
    singly_linked_list();

    void insert(Node * previousNode, Node * newNode);
    void remove(Node * previousNode, Node * deleteNode);
};

#include "SinglyLinkedListImpl.h"

