#ifndef SRC_MEMORY_MEM_NODE_OPERATOR_H_
#define SRC_MEMORY_MEM_NODE_OPERATOR_H_

#pragma once;

#include "mem_node.h"

bool MemNode::operator<=(MemNode const &p) const {
    if (this -> size_ <= p.size_)
        return true;
    return false;
}

bool MemNode::operator<(MemNode const &p) const {
    if (this -> begin_ <= p.begin_)
        return true;
    return false;
}

#endif // SRC_MEMORY_MEM_NODE_OPERATOR_H_