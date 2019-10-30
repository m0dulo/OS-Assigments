#ifndef SRC_MEMORY_MemNode_H_
#define SRC_MEMORY_MemNode_H_

#pragma once;

#include <string>

class MemNode {
private:
    int begin_;
    int end_;
    int size_;

public:
    MemNode() = default;

    void set(int b, int e) {
        begin_ = b;
        end_ = e;
        size_ = end_ - begin_;
    }

    void cut(int size) {
        begin_ += size;
        size_ = end_ - begin_;
    }

    int get_begin() {
        return begin_;
    }

    int get_end() {
        return end_;
    }

    int get_size() {
        return size_;
    }

    

    bool operator<=(MemNode const &e) const;
    bool operator<(MemNode const &p) const;
};

#endif // SRC_MEMORY_MemNode_H_