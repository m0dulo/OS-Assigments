#ifndef SRC_MGMT_H_
#define SRC_MGMT_H_

#pragma once;

#include "bitmap.h"
#include "list_header.h"
#include "pcb.h"
#include "queue.h"
#include <tuple>
#include <map>

#define F_MASK 0xfff
#define O_MASK 0x3ff
#define E_MASK 0x1fff
#define T_MASK 0x7ff

class Mgmt {
private:
    BitMap map_;
    Queue<int> mem_;
    int mem_size_;
    int block_size_;
    std::map<int,int> mask_map_;

    void set_mask() {
        mask_map_[10] = O_MASK;
        mask_map_[11] = T_MASK;
        mask_map_[12] = F_MASK;
        mask_map_[13] = E_MASK;
    }
    
public:
    int visit_ = 0;
    Mgmt() {set_mask();}
    void set_block_size(int size);
    int get_block_size() const;
    void set_mem_size(int size);
    int get_mem_size() const;
    void init();
    void helper();
    Pcb* create_pcb();
    void fifo_alloc(Pcb &p);
    void lru_alloc(Pcb &p);
    void show_mem();
    void show_table(Pcb &p);
    std::tuple<int,int,int> log2ph(int log_addr);
    // void round_robin();
    // void block();
    // void wake_up();
    // void release();
    // void show_pcb();
    // void show_mem();
};

#endif // SRC_MGMT_H_