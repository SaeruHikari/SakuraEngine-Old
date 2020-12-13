/*
 * @Author: your name
 * @Date: 2020-08-11 17:19:36
 * @LastEditTime: 2020-08-11 17:25:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \allocators\DoublyLinkedList.h
 */
#ifndef doubly_linked_list_H
#define doubly_linked_list_H

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

#endif /* doubly_linked_list_H */

