#ifndef SRC_BASIC_PCB_H_
#define SRC_BASIC_PCB_H_

#pragma once;

#include <string>

class Pcb {
private:
    std::string pcb_name_;
public:
    Pcb(){}
    Pcb(std::string n) : pcb_name_(n) {}
    void init(std::string n) {
        pcb_name_ = n;
    }
    std::string get_name() const {
        return pcb_name_;
    }
};

#endif // SRC_BASIC_PCB_H_