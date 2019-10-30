#include "mgmt.h"
#include <iostream>
#include <glog/logging.h>
#include <math.h>
#include <cmath>
#include <bitset>
#include <stdio.h>


using namespace google;
using namespace std;


 
void Mgmt::set_block_size(int size) {
    this -> block_size_ = size;
}

int Mgmt::get_block_size() const {
    return (this -> block_size_);
}

void Mgmt::set_mem_size(int size) {
    this -> mem_size_ = size;
}

int Mgmt::get_mem_size() const {
    return (this -> mem_size_);
}

void Mgmt::init() {
    cout << "请输入内存大小(KB)：" << endl << ">";
    int mem_size;
    cin >> mem_size;
    set_mem_size(mem_size);
    cout << "请输入块大小(KB)：" << endl << ">";
    int block_size;
    cin >> block_size;
    set_block_size(block_size);
    int bit_len = mem_size_ / block_size;
    map_.set_btmp_len(bit_len);
    map_.init();
    map_.random();
    int size = map_.bit_len();
    //mem_.set(size);
    mem_.init(size);
    cout << "====已随机初始化位示图====" << endl;
    map_.show();
}

Pcb* Mgmt::create_pcb() {
    int size;
    cout << "请输入要创建Pcb的大小(Byte)：" << endl << ">";
    cin >> size;
    int page_len = ceil(size / (float)(block_size_ * 1024));
    Pcb *p = new Pcb;
    p ->set(size, page_len);
    cout << "创建成功！~" << "为该Pcb分配" << page_len << "页" << endl;
    return p;
}

void Mgmt::show_mem() {
    cout << "内存：" << endl;
    for (int i = mem_.size() - 1 ; i >= 0; i-- ) {
        cout << "|" << "  " << i << "  " << "|  " << mem_[i] ->data_ << "  |" << endl;
    }
}

void Mgmt::show_table(Pcb &p) {
    cout << "页表：" << endl;
    for (int i = 0; i < p.get_page_len(); i++) {
        cout << i << " |  " << p.page_table_[i].get_block() << "  |  " 
        << p.page_table_[i].get_flag() << "  |" << endl;
    }
}

std::tuple<int,int,int> Mgmt::log2ph(int log_addr) {
    double a = 2;
    int odd = log(block_size_ * 1024)/log(2);
    int mask = mask_map_[odd];
    int ph_addr_page = log_addr >> odd;
    int ph_addr_in = (log_addr & mask);
    return make_tuple(ph_addr_page, ph_addr_in, odd);
}

void Mgmt::fifo_alloc(Pcb &p) {
    cout << "请输入当前执行进程所要s访问的逻辑地址：" << endl << ">";
    int log_addr;
    int phy_addr;
    int block_num = -1;
    cin >> hex >> log_addr;
    auto ret = log2ph(log_addr);
    int page_id = get<0>(ret);
    int addr_in = get<1>(ret);
    if (page_id >= p.get_page_len()) {
        cout << "越界！请重新输入！" << endl;
        fifo_alloc(p);
    } else if (page_id < p.get_page_len()) {
        p.add_visit();
    }

    if (mem_.check(page_id) == false) {
        p.add_absent();
        if (mem_.is_top() == false) {
            auto idx = map_.bit_idx();
            int block = idx.at(0);
            phy_addr = block;
            map_.set(block,1);
            p.page_table_[page_id].set_flag(1);
            p.page_table_[page_id].set_block(block);
            block_num = block;
            for (int i = 0; i < mem_.size(); i++) {
                if (mem_[i] -> data_ == -1) {
                    mem_[i] -> data_ = page_id;
                    break;
                }
            }
        }
        else {
            map_.set(p.page_table_[mem_[0]->data_].get_block(),0);
            p.page_table_[mem_[0]->data_].set_flag(0);
            p.page_table_[mem_[0]->data_].set_block(-1);
            mem_.dequeue();
            mem_.enqueue(page_id);
            auto idx = map_.bit_idx();
            int block = idx.at(0);
            block_num = block;
            map_.set(block,1);
            p.page_table_[page_id].set_flag(1);
            p.page_table_[page_id].set_block(block);
        }
    } else {
        int idx = mem_.find(page_id);
        block_num = p.page_table_[page_id].get_block();
    }
    phy_addr = (block_num << get<2>(ret)) + addr_in;
    show_table(p);
    cout << "============" << endl;
    show_mem();
    cout << "============" << endl;
    printf("%x",log_addr);
    cout << "的物理地址为：";
    printf("%x",phy_addr);
    cout << endl;
    cout << "访问次数：" << p.get_visit() << endl
        <<"缺页次数：" << p.get_absent() << endl;
    cout << "缺页率：" << (p.get_absent() / (p.get_visit()*1.0)) * 100 << "%" << endl;
}

void Mgmt::lru_alloc(Pcb &p) {
    cout << "请输入当前执行进程所要访问的逻辑地址：" << endl << ">";
    int phy_addr;
    int log_addr;
    cin >> hex >> log_addr;
    int block_num = -1;
    auto ret = log2ph(log_addr);
    int page_id = get<0>(ret);
    int addr_in = get<1>(ret);
    if (page_id >= p.get_page_len()) {
        cout << "越界！请重新输入！" << endl;
        lru_alloc(p);
    } else if (page_id < p.get_page_len()) {
        p.add_visit();
    }

    if (mem_.check(page_id) == false) {
        p.add_absent();
        if (mem_.is_top() == false) {
            auto idx = map_.bit_idx();
            int block = idx.at(0);
            map_.set(block,1);
            block_num = block;
            p.page_table_[page_id].set_flag(1);
            p.page_table_[page_id].set_block(block);
            
            for (int i = 0; i < mem_.size(); i++) {
                if (mem_[i] -> data_ == -1) {
                    mem_[i] -> data_ = page_id;
                    break;
                }
            }
        }
        else {
            map_.set(p.page_table_[mem_[0]->data_].get_block(),0);
            p.page_table_[mem_[0]->data_].set_flag(0);
            p.page_table_[mem_[0]->data_].set_block(-1);
            mem_.dequeue();
            mem_.enqueue(page_id);
            auto idx = map_.bit_idx();
            int block = idx.at(0);
            map_.set(block,1);
            block_num = block;
            p.page_table_[page_id].set_flag(1);
            p.page_table_[page_id].set_block(block);
        }
    } else {
        int idx = mem_.find(page_id);
        int e = mem_[idx] -> data_;
        block_num = p.page_table_[page_id].get_block();
        mem_.remove(mem_[idx]);
        if (mem_.is_top() == false) {
             for (int i = 0; i < mem_.size(); i++) {
                if (mem_[i] -> data_ == -1) {
                    mem_[i] -> data_ = e;
                    break;
                }
             }
             mem_.push_back(-1);
        } else {
            mem_.push_back(e);
        }
    }

    phy_addr = (block_num << get<2>(ret)) + addr_in;
    show_table(p);
    cout << endl;
    // cout << hex << log_addr << "的物理地址为：";
    // cout << hex << phy_addr << endl;
    show_mem();
    cout << "============" << endl;
    printf("%x",log_addr);
    cout << "的物理地址为：";
    printf("%x",phy_addr);
    cout << endl;
    cout << "访问次数：" << p.get_visit() << endl
        <<"缺页次数：" << p.get_absent() << endl;
    cout << "缺页率：" << (p.get_absent() / (p.get_visit() * 1.0)) * 100 << "%" << endl;
}


int main(int argc, char *argv[]) {
    Mgmt m;
    m.init();
    m.show_mem();
    Pcb *p = m.create_pcb();
    m.show_table(*p);
    for (int i = 0; i < 20; i++) {
        m.fifo_alloc(*p);
    }

    return 0;
}