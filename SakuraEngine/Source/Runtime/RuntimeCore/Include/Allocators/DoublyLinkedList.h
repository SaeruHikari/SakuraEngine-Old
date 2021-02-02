#pragma once

template <class T>
class doubly_linked_list {
public:
    struct Node {
        T data;
        Node* previous;
        Node* next;
    };
    Node* head;
public:
    doubly_linked_list();

    void insert(Node* previousNode, Node* newNode);
    void remove(Node* deleteNode);
private:
    doubly_linked_list(doubly_linked_list &doubly_linked_list);
};

#include "DoublyLinkedListImpl.h"

