#ifndef SRC_BASIC_LIST_INIT_H_
#define SRC_BASIC_LIST_INIT_H_

#pragma once;

#include "list.h"
#include "list_node.h"

template <typename T>
void List<T>::init() {
    header_ = new ListNode<T>;
    trailer_ = new ListNode<T>;

    header_ -> succ_ = trailer_;
    header_ -> pred_ = nullptr;

    trailer_ -> pred_ = header_;
    trailer_ -> succ_ = nullptr;
}
#endif // SRC_BASIC_LIST_INIT_H_