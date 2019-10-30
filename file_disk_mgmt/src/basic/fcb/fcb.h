#ifndef SRC_BASIC_FCB_H_
#define SRC_BASIC_FCB_H_

#pragma once;

#include <stdint.h>
#include <string>

struct Fcb{
    char name_[5];
    int size_;
    int first_block_;
    int type_ = 0;
    char date_time_[10];
};

#endif // SRC_BASIC_FCB_H_