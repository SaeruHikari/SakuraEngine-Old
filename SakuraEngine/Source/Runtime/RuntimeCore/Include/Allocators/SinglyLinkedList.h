/*
 * @Author: your name
 * @Date: 2020-08-11 17:19:36
 * @LastEditTime: 2020-08-11 17:29:25
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \allocators\SinglyLinkedList.h
 */
#ifndef singly_linked_list_H
#define singly_linked_list_H

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

#endif /* singly_linked_list_H */

