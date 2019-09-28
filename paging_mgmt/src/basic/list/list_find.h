#ifndef SRC_BASIC_LIST_FIND_H_
#define SRC_BASIC_LIST_FIND_H_

#pragma once;
#include "list.h"

template <typename T> 

int List<T>::find(std::string s) const {
    int rank = 0;
    for (; rank < size(); rank++) {
        if(s == (*this)[rank] -> data_.name_)
            break;
    }
    return rank;
}

#endif // SRC_BASIC_LIST_FIND_H_