#ifndef SRC_BASIC_LIST_FIND_H_
#define SRC_BASIC_LIST_FIND_H_

#pragma once;
#include "list.h"

template <typename T> 

bool List<T>::check(std::string name)  {
    if (size_ == 0) return false;
    if (size_ > 0) {
        int rank = 0;
        int flag = 0;
        for (; rank < size(); rank++) {
            if(name == (*this)[rank] -> data_) {
                flag = 1;
                break;
            }
        }   
        if (flag) {return true;}
        else {return false;}
    }
}

template <typename T> 
int List<T>::find(std::string name) const {
    if (size_ > 0) {
        int rank = 0;
        for (; rank < size(); rank++) {
            if(name == (*this)[rank] -> data_.get_name())
            break;
        }
        return rank;
    }
}

template <typename T> 
int List<T>::find_type(std::string name) const {
    if (size_ > 0) {
        int rank = 0;
        for (; rank < size(); rank++) {
            if(name == (*this)[rank] -> data_->get_type())
            break;
        }
        return rank;
    }
}

template <typename T>
std::tuple<int, std::vector<int>> List<T>::find_pos(int i) {
    
}

#endif // SRC_BASIC_LIST_FIND_H_