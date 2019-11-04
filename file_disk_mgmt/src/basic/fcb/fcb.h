#ifndef SRC_BASIC_FCB_H_
#define SRC_BASIC_FCB_H_

#pragma once;

#include <stdint.h>
#include <string>

struct Fcb{
    char name_[8];
    int size_;
    int first_block_;
    char type_ = 0;
    char date_time_[15];
};

#endif // SRC_BASIC_FCB_H_