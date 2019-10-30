#ifndef SRC_LIST_VISIT_H_
#define SRC_LIST_VISIT_H_

#pragma once

#include "list.h"
#include <stdexcept>

template <typename T> 
ListNode<T> *List<T>::operator[] (int rank) const {
    if (rank >= this -> size())
        throw std::out_of_range("越界！请检查！");
    ListNode<T> *p = first();
    while (rank-- > 0)
        p = p -> succ_;
    return p;
}

#endif // SRC_LIST_VISIT_H_