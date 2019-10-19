#ifndef SRC_BASIC_NODES_H_
#define SRC_BASIC_NODES_H_

#pragma once;

#include "pcb.h"
#include "queue.h"
#include "list_header.h"
#include <vector>
#include <iostream>

class Node {
private:
    std::string node_name;
    int rank = 0;

public:
    Pcb *process = nullptr;
    Queue<Pcb*> waiting_list;
    void init(std::string n) {
        this -> node_name = n;
    }

    void set_rank(int r) {
        this -> rank = r;
    }

    std::string get_name() {
        return this -> node_name;
    }

    int waiting_size() {
        return waiting_list.size();
    }

    void show_waiting() {
        if (waiting_list.size() == 0) {
            std::cout << "none" << std::endl;
        }else {
            for (int i = 0; i < waiting_list.size(); ++i) {
                std::cout << waiting_list[i] ->data_-> get_name() << " ";
            }
            std::cout << std::endl;
        }
    }

    void show_process() {
        if (process == nullptr) {
            std::cout << "none" << std::endl;
        }else {
            std::cout << process ->get_name() << std::endl;
        }
    }

};

class CoNode;
class DcNode;

class ChNode : public Node {
public:
    std::vector<CoNode*> childs;

    void add_child(CoNode &n) {
        CoNode *ptr = nullptr;
        childs.push_back(ptr);
        childs[childs.size() - 1]  = &n;
    }
};

class CoNode : public Node {
public:
    ChNode *parent = nullptr;
    std::vector<DcNode *> childs;

    void add_child(DcNode &n) {
        DcNode *ptr = nullptr;
        childs.push_back(ptr);
        childs[childs.size() - 1]  = &n;
    }

    void set_parent(ChNode &n) {
        parent = &n;
    }

};

class DcNode : public Node {
private:
    char type = '\0';
public:
    CoNode *parent = nullptr;

    void set_type(char t) {
        type = t;
    }

    char get_type() {
        return type;
    }

    void set_parent(CoNode &n) {
        parent = &n;
    }
};






#endif // SRC_BASIC_NODES_H_