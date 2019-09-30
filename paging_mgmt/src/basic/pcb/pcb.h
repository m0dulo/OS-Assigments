#ifndef SRC_BASIC_PCB_H_
#define SRC_BASIC_PCB_H_

#pragma once;

#include <string>
#include <iostream>
#include "page_table.h"

class Pcb {
private:
    //std::string name_;
    //int base_addr_;
    int size_;
    int page_len_;
    int visit_;
    int absent_;

public:

    PageTable *page_table_;
    Pcb() {
        visit_ = 0;
        absent_ = 0;
    }

    int get_visit() const {
        return visit_;
    }

    int get_absent() const {
        return absent_; 
    }

    void add_visit() {
        visit_++;
    }

    void add_absent() {
        absent_++;
    }

    void set(int s, int len) {
        //name_ = n;
        //base_addr_ = b;
        size_ = s;
        page_len_ = len;
        page_table_ = new PageTable[page_len_];
    }

    int get_page_len() const {
        return (this -> page_len_);
    }

    // int get_addr() {
    //     return base_addr_;
    // }

    int get_size() {
        return size_;
    }

    // std::string get_name() {
    //     return name_;
    // }

    // bool operator<=(const Pcb &p) {
    //     if (this -> base_addr_ <= p.base_addr_)
    //         return true;
    //     return false;
    // }
    
    ~Pcb() {
        delete []page_table_;
        page_table_ = nullptr;
    }
};

#endif // SRC_BASIC_PCB_H_