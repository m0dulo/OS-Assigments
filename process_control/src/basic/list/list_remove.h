#ifndef SRC_LIST_LIST_REMOVE_H_
#define SRC_LIST_LIST_REMOVE_H_

#pragma once;

#include "list.h"

template <typename T>
void List<T>::remove(ListNode<T> *p) {
    T e = p -> data_;
    p -> pred_ -> succ_ = p -> succ_;
    p -> succ_ -> pred_ = p -> pred_;
    delete p;
    size_--;
    //return e;
}


#endif // SRC_LIST_LIST_REMOVE_H_