#ifndef SRC_LIST_INSERT_H_
#define SRC_LIST_INSERT_H_

#pragma once;

#include "list.h"

template <typename T>
ListNode<T> *List<T>::insertAsFirst(T const &e) {
    size_++;
    return (header_ -> insertAsSucc(e));
}


template <typename T>
ListNode<T> *List<T>::insertAsLast(T const &e) {
    size_++;
    return (trailer_ -> insertAsPred(e));
}

template <typename T>
ListNode<T> *List<T>::insertAfter(ListNode<T> *p, T const &e) {
    size_++;
    return ( p -> insertAsSucc(e));
}

template <typename T>
ListNode<T> *List<T>::insertBefore(ListNode<T> *p, T const &e) {
    size_++;
    return ( p -> insertAsPred(e));
}

#endif // SRC_LIST_INSERT_H_