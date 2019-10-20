#ifndef SRC_MGMT_H_
#define SRC_MGMT_H_

#pragma once;

#include "nodes.h"
#include <set>


class Mgmt {
private:
    List<ChNode*> chct;
    List<CoNode*> coct;
    List<DcNode*> dct;

    List<Pcb*> ready;
    List<Pcb*> blocking;
public:
    Node *cpu = new Node;
    void init();
    void helper();
    void show_allocation();
    void show_pcb_state();
    void add_dc();
    void add_co();
    void add_ch();
    void dele_dc();
    void apply(std::string dc_name);
    void release(std::string n);
    void apply_for_the_same_type();
    void terminate();
    void tree_print(Node *node, int blk);
    void process_print(Node *node, int blk);
};


#endif // SRC_MGMT_H_