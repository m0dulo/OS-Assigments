#ifndef SRC_BASIC_BLOCK_H_
#define SRC_BASIC_BLOCK_H_

#pragma once;

#include <stdint.h>
#include "fcb.h"

struct Block {
    Fcb f[32];
};


#endif // SRC_BASIC_BLOCK_H_