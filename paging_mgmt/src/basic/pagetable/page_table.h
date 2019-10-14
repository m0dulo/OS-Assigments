#ifndef SRC_BASIC_PAGE_TABLE_H_
#define SRC_BASIC_PAGE_TABLE_H_

#pragma once;

class PageTable {
private:
    int block_;
    int flag_;
public:
    PageTable() {block_ = -1; flag_ = 0;}
    
    void set_block(int b) {
        this -> block_ = b;
    }

    void set_flag(int f) {
        this -> flag_ = f;
    }

    int get_block() const {
        return (this -> block_);
    }

    int get_flag() const {
        return (this -> flag_);
    }
};

#endif // SRC_BASIC_PAGE_TABLE_H_