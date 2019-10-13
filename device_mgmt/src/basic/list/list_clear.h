#ifndef SRC_BASIC_LIST_CLEAR_H_
#define SRC_BASIC_LIST_CLEAR_H_

#pragma once;
#include "list.h"

template <typename T> int List<T>::clear() {
    int old_size = size_;
    while (size_ > 0) 
        remove( header_ -> succ_);
    return old_size;
}
#endif // #ifndef SRC_BASIC_LIST_CLEAR_H_
