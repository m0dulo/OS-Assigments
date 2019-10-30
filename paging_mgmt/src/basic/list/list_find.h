#ifndef SRC_BASIC_LIST_FIND_H_
#define SRC_BASIC_LIST_FIND_H_

#pragma once;
#include "list.h"

template <typename T> 

bool List<T>::check(T data)  {
    int rank = 0;
    int flag = 0;
    for (; rank < size(); rank++) {
        if(data == (*this)[rank] -> data_) {
            flag = 1;
            break;
        }
    }
    if (flag) {return true;}
    else {return false;}
}

template <typename T> 
int List<T>::find(T data) const {
    int rank = 0;
    for (; rank < size(); rank++) {
        if(data == (*this)[rank] -> data_)
            break;
    }
    return rank;
}

#endif // SRC_BASIC_LIST_FIND_H_