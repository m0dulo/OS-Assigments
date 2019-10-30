#ifndef SRC_PCB_PCB_H_
#define SRC_PCB_PCB_H_

#include <string>
#include <iostream>

class Pcb {
private:
    std::string name_;
    int base_address_;
    int size_;

public:
    Pcb() = default;

    Pcb(std::string n, int b, int si)
       :name_(n), base_address_(b), size_(si) {}

    void set(std::string n, int b, int s) {
        name_ = n;
        base_address_ = b;
        size_ = s;
    }

    int get_addr() {
        return base_address_;
    }

    int get_size() {
        return size_;
    }

    std::string get_name() {
        return name_;
    }


    

    bool operator<=(const Pcb &p);

    
};

#endif // SRC_PCB_PCB_H_