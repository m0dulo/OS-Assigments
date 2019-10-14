#ifndef SRC_MGMT_H_
#define SRC_MGMT_H_

#pragma once;

#include "node.h"

class Mgmt {
public:
    List<Node> chct;
    List<Node> coct;
    List<Node> dct;

    void init();
    void helper();
    void show();
    void add_dc();
    void add_co();
    void add_ch();
};


#endif // SRC_MGMT_H_