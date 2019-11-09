#include "process_sched.h"
#include <iostream>
#include<algorithm>

using namespace std;
using namespace lyx_utlis;

void Sched::init() {
    Pcb *p1 = new Pcb;
    p1 -> init("A", 0, 3);
    ready_.push_back(*p1);

    Pcb *p2 = new Pcb;
    p2 -> init("B", 0, 2);
    ready_.push_back(*p2);

    Pcb *p3 = new Pcb;
    p3 -> init("C", 10, 4);
    ready_.push_back(*p3);

    Pcb *p4 = new Pcb;
    p4 -> init("D", 1, 2);
    ready_.push_back(*p4);

    Pcb *p5 = new Pcb;
    p5 -> init("E", 16, 4);
    ready_.push_back(*p5);

}


void Sched::calc(Pcb *p, List<Pcb> list) {
    if (p -> get_name() == list[0]->data_.get_name()) {
        p -> set_start(p -> get_arr());
        p -> set_fini(p -> get_start() + p -> get_bur());
        p -> set_T(p -> get_fini() - p ->get_arr());
        p -> set_W(p -> get_T() / p -> get_bur());
    } else {
        int r = list.find(p -> get_name());
        if (p -> get_arr() > list[r] -> pred_ -> data_.get_fini()) {
            p -> set_start(p -> get_arr());
        } else {
            p -> set_start(list[r] -> pred_ ->data_.get_fini());
        }
        p -> set_fini(p -> get_start() + p -> get_bur());
        p -> set_T(p -> get_fini() - p ->get_arr());
        p -> set_W(p -> get_T() / p -> get_bur());
    }
        
}

void Sched::FCFS() {
    ready_.sort("ARR ORDERED");
    running_.clear();
    runned_.clear();

    while(!ready_.empty()) {
        running_.push_back(ready_.first() ->data_);
        ready_.remove(ready_.first());
        runned_.push_back(running_.first() -> data_);
        running_.remove(running_.first());
    }

    for(int j = 0; j < runned_.size(); ++j) {
        calc(&(runned_[j] -> data_), runned_);
    }
}


void Sched::SJF() {
    ready_.sort("ARR ORDERED");
    running_.clear();
    runned_.clear();

    running_.push_back(ready_.first() -> data_);
    ready_.remove(ready_.first());
    runned_.push_back(running_.first() -> data_);
    running_.remove(running_.first());

    ready_.sort("SJF ORDERED");

    while(!ready_.empty()) {
        running_.push_back(ready_.first() ->data_);
        ready_.remove(ready_.first());
        runned_.push_back(running_.first() -> data_);
        running_.remove(running_.first());
    }

    for(int j = 0; j < runned_.size(); ++j) {
        calc(&(runned_[j] -> data_), runned_);
    }
}

void Sched::RR(int q) {
    
}

int main(int argc, char *argv[]) {
    Sched s;
    s.init();
    s.SJF();
    for (int i = 0; i < s.runned_.size(); ++i) {
        cout << "名字" << s.runned_[i] ->data_.get_name() << " "
            << "到达时间：" << s.runned_[i] ->data_.get_arr() << " "
            << "开始时间：" << s.runned_[i] ->data_.get_start() << " "
            << "服务时间：" << s.runned_[i] -> data_.get_bur() << " "
            << "结束时间：" << s.runned_[i] -> data_.get_fini() << " "
            << "周转时间：" << s.runned_[i] -> data_.get_T() << " "
            << "带权周转：" << s.runned_[i] -> data_.get_W();
            cout << endl;
    }
    // for (int i = 0; i < s.ready_.size(); ++i) {
    //     cout << s.ready_[i] ->data_.get_arr() << endl;
    // }
    return 0;
}