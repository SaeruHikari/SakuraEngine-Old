#include "StackLinkedList.h"

template <class T>
void stack_linked_list<T>::push(Node * newNode) {
    newNode->next = head;
    head = newNode;
}

template <class T>
typename stack_linked_list<T>::Node* stack_linked_list<T>::pop() {
    Node * top = head;
    head = head->next;
    return top;
}