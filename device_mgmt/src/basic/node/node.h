#ifndef SRC_BASIC_NODE_H_
#define SRC_BASIC_NODE_H_

#pragma once;

#include "pcb.h"
#include "queue.h"

struct Node {
    std::string node_name;
    Pcb *process;
    Queue<Pcb> waiting_list;
    Node *parent;
    char type;
};

List<Node> chct;
List<Node> coct;
List<Node> dct;

#endif // SRC_BASIC_NODE_H_