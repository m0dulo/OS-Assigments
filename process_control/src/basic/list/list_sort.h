#ifndef SRC_LIST_LIST_SORT_H_
#define SRC_LIST_LIST_SORT_H_

#pragma once;

#include "list.h"

template <typename T>
ListNode<T> *List<T>::search_size(int n, ListNode<T> *p) const{
    T const &e = p -> data_;
    while (n-- >= 0) {
        if ((( p = p -> pred_ ) -> data_ ) <= e)
            break;
    }
    return p;
}

template <typename T>
ListNode<T> *List<T>::search_addr(int n, ListNode<T> *p) const{
    T const &e = p -> data_;
    while (n-- >= 0) {
        if ((( p = p -> pred_ ) -> data_ ) < e)
            break;
    }
    return p;
}


template <typename T>
void List<T>::sort(int flag) {
    if (flag == 0) {
        ListNode<T> *p = first();
        for(int i = 0; i < size_; i++) {
            ListNode<T> *pos = search_size (i, p);
            insertAfter(pos, p -> data_);
            p = p -> succ_;
            remove(p -> pred_);
        }
    }
    else if (flag == 1) {
        ListNode<T> *p = first();
        for(int i = 0; i < size_; i++) {
            ListNode<T> *pos = search_addr (i, p);
            insertAfter(pos, p -> data_);
            p = p -> succ_;
            remove(p -> pred_);
        }
    }
}

#endif // SRC_LIST_LIST_SORT_H_