#ifndef SRC_BASIC_PCB_H_
#define SRC_BASIC_PCB_H_

#pragma once;

#include <string>

class Pcb {
private:
    std::string pcb_name_;
    std::string state_;
public:
    Pcb(){}

    Pcb(std::string n) : pcb_name_(n) {}

    void init(std::string n) {
        pcb_name_ = n;
        state_ = "Ready";
    }
    std::string get_name() const {
        return pcb_name_;
    }
    std::string get_state() const {
        return state_;
    }

    void set_bolcking() {
        state_ = "Blcoking";
    }

    void set_ready() {
        state_ = "Ready";
    }

    bool is_ready() {
        if (state_ == "Ready")
            return true;
        else
            return false;        
    }

    bool is_blocking() {
        if (state_ == "Blocking")
            return true;
        else
            return false;
    }
};

#endif // SRC_BASIC_PCB_H_