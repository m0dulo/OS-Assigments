#ifndef SRC_BASIC_LIST_FIND_H_
#define SRC_BASIC_LIST_FIND_H_

#pragma once;
#include "list.h"

template <typename T> 

bool List<T>::check(std::string name)  {
    int rank = 0;
    int flag = 0;
    for (; rank < size(); rank++) {
        if(name == (*this)[rank] -> data_.node_name) {
            flag = 1;
            break;
        }
    }
    if (flag) {return true;}
    else {return false;}
}

template <typename T> 
int List<T>::find(std::string name) const {
    int rank = 0;
    for (; rank < size(); rank++) {
        if(name == (*this)[rank] -> data_.node_name)
            break;
    }
    return rank;
}

#endif // SRC_BASIC_LIST_FIND_H_