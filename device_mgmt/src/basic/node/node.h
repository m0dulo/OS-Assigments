#ifndef SRC_BASIC_NODE_H_
#define SRC_BASIC_NODE_H_

#pragma once;

#include "pcb.h"
#include "queue.h"
#include "list_header.h"
#include <vector>
struct Child {

};

class Node {
public:
    std::string node_name;
    Pcb *process = nullptr;
    List<Pcb> waiting_list;
    Node *parent = nullptr;
    std::vector<Node *> childs;
    char type = '\0';
    int rank = 0;

    void add_child(Node &n) {
        Node *ptr = nullptr;
        this -> childs.push_back(ptr);
        this -> childs[childs.size() - 1]  = &n;
    }

    void set_parent(Node &n) {
        parent = (&n);
    }

    void init(std::string n) {
        this -> node_name = n;
    }

    void set_type(char t) {
        this -> type = t;
    }

    void set_rank(int r) {
        this -> rank = r;
    }
};

// List<Node> chct;
// List<Node> coct;
// List<Node> dct;

#endif // SRC_BASIC_NODE_H_