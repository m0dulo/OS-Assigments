#ifndef SRC_BASIC_NODES_H_
#define SRC_BASIC_NODES_H_

#pragma once;

#include "pcb.h"
#include "queue.h"
#include "list_header.h"
#include <vector>

class Node {
private:
    std::string node_name;
    Pcb *process = nullptr;
    List<Pcb> waiting_list;
    int rank = 0;

public:
    Node *parent = nullptr;
    std::vector<Node *> childs;

    virtual void add_child(Node &n) {}
    virtual void set_parent(Node &n) {}

    void init(std::string n) {
        this -> node_name = n;
    }

    void set_rank(int r) {
        this -> rank = r;
    }

    std::string get_name() {
        return this -> node_name;
    }
};

class DcNode : public Node {
private:
    char type = '\0';
public:
    void set_parent(Node &n) {
        parent = &n;
    }

    void set_type(char t) {
        this -> type = t;
    }

    char get_type() {
        return this -> type;
    }
};

class CoNode : public Node {
public:
    void set_parent(Node &n) {
        parent = &n;
    }

    void add_child(Node &n) {
        Node *ptr = nullptr;
        this -> childs.push_back(ptr);
        this -> childs[childs.size() - 1]  = &n;
    }
};

class ChNode : public Node {
public:
    void add_child(Node &n) {
        Node *ptr = nullptr;
        this -> childs.push_back(ptr);
        this -> childs[childs.size() - 1]  = &n;
    }
};


#endif // SRC_BASIC_NODES_H_