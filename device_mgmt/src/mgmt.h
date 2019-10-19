#ifndef SRC_MGMT_H_
#define SRC_MGMT_H_

#pragma once;

#include "nodes.h"
#include <set>

class Mgmt {
private:
    List<ChNode *> chct;
    List<CoNode *> coct;
    List<DcNode *> dct;

    List<Pcb *> ready;
    List<Pcb *> blocking;
public:
    void init();
    void helper();
    void show_allocation();
    void show_pcb_state();
    void add_dc();
    void add_co();
    void add_ch();
    void apply();
    void release();
};


#endif // SRC_MGMT_H_