#ifndef SRC_MGMT_H_
#define SRC_MGMT_H_

#pragma once;

#include "nodes.h"

class Mgmt {
private:
    List<ChNode> chct;
    List<CoNode> coct;
    List<DcNode> dct;
public:
    void init();
    void helper();
    void show();
    void add_dc();
    void add_co();
    void add_ch();
};


#endif // SRC_MGMT_H_