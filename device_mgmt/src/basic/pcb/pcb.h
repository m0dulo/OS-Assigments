#ifndef SRC_BASIC_PCB_H_
#define SRC_BASIC_PCB_H_

#pragma once;

#include <string>

class Pcb {
private:
    std::string pcb_name_;
public:
    void set_name(std::string n) {
        pcb_name_ = n;
    }
    std::string get_name() {
        return pcb_name_;
    }
};

#endif // SRC_BASIC_PCB_H_