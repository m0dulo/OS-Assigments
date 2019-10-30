#ifndef SRC_PCB_PCB_OPERATOR_H_
#define SRC_PCB_PCB_OPERATOR_H_

#pragma once;

#include "pcb.h"

bool Pcb::operator<=(const Pcb &p){
    if (this -> base_address_ <= p.base_address_)
        return true;
    return false;
}

#endif // SRC_PCB_PCB_OPERATOR_H_