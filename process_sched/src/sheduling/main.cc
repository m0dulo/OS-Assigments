#include "process_sched.h"
#include <iostream>
#include<algorithm>

using namespace std;
using namespace lyx_utlis;

#define RED "\033[31m"
#define WHITE "\033[37m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define YELLOW "\033[33m"
#define CYAN "\033[0;36m"

void Sched::init() {
    clear();
    Pcb *p1 = new Pcb;
    p1 -> init("A", 0, 4, 1);
    ready_.push_back(*p1);

    Pcb *p2 = new Pcb;
    p2 -> init("B", 1, 3, 1);
    ready_.push_back(*p2);

    Pcb *p3 = new Pcb;
    p3 -> init("C", 2, 5, 1);
    ready_.push_back(*p3);

    Pcb *p4 = new Pcb;
    p4 -> init("D", 3, 2, 1);
    ready_.push_back(*p4);

    Pcb *p5 = new Pcb;
    p5 -> init("E", 17, 4, 1);
    ready_.push_back(*p5);
}

void Sched::clear() {
    ready_.clear();
    running_.clear();
    runned_.clear();
    task_.clear();
    show_.clear();
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
    clear();
    init();
    ready_.sort("ARR ORDERED");
    while(!ready_.empty()) {
        running_.push_back(ready_.first() ->data_);
        ready_.remove(ready_.first());
        runned_.push_back(running_.first() -> data_);
        running_.remove(running_.first());
    }

    for(int j = 0; j < runned_.size(); ++j) {
        calc(&(runned_[j] -> data_), runned_);
    }

    show("FCFS");
}


void Sched::SJF() {
    clear();
    init();
    ready_.sort("ARR ORDERED");
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

    show("SJF");
}

void Sched::RR() {    
    clear();
    init();
    int step_ = 0;
    cout << ready_.size() <<  endl;
    int q;
    cout << "请输入时间片大小:" << endl;
    cout << "> ";
    cin >> q;
    ready_.sort("ARR ORDERED");
    
    while (!ready_.empty()) {
        cout << "step = " << step_ << endl;
        if ((step_ < ready_.first()->data_.get_arr()) && (ready_.size() > 1)) {
            running_.push_back(ready_.first()->succ_->data_);
            ready_.remove(ready_[1]);
        } else {
            running_.push_back(ready_.first()->data_);
            ready_.remove(ready_.first());
        }
        for (int i = 0; i < q; ++i) {
            if (running_.first()->data_.is_finished()) break;
            else {
                step_++;
                if ((i == 0) && (!show_.check(running_.first()->data_.get_name()))) {
                    running_.first()->data_.set_start(step_);
                }
                if (step_ < running_.first()->data_.get_arr())
                    show_.push_back("*");
                else {
                    running_.first()->data_.run();
                    show_.push_back(running_.first()->data_.get_name());
                }
            }
        }
        if (!running_.first()->data_.is_finished()) {
            ready_.push_back(running_.first()->data_);
            running_.remove(running_.first());
        }
        else {
            running_.first()->data_.set_fini(step_);
            running_.first()->data_.set_T(running_.first()->data_.get_fini()-running_.first()->data_.get_arr());
            running_.first()->data_.set_W(running_.first()->data_.get_T() / running_.first()->data_.get_bur());
            runned_.push_back(running_.first()->data_);
            running_.remove(running_.first());
        }
    }
    cout << ready_.size() << endl;
    for (int i = 0; i < runned_.size(); ++i) {
        cout << "name: " << runned_[i]->data_.get_name() << " "
            << "runned: " << runned_[i]->data_.get_runned() << endl;
    }
    show("RR");
}

void Sched::PS() {
    clear();
    init();

}

void Sched::show(std::string n) {
    int i = 0;
    if (n == "RR") {
        for (;i < show_.size(); ++i) {
            cout << " "<< WHITE << i  << " "<< RED << show_[i]->data_ << WHITE;
        }
        cout << i << endl;
    }
    printf("┌──────┬────────┬────────┬────────┬────────┬────────┬────────────┐\n");
    printf("│进程名│到达时间│开始时间│服务时间│完成时间│周转时间│带权周转时间│\n");
    for(int i = 0; i < runned_.size(); ++i){
        printf("├──────┼────────┼────────┼────────┼────────┼────────┼────────────┤\n");
        printf("│%6s│%8d│%8d│%8d│%8d│%8.2f│%12.2f│\n",
                runned_[i]->data_.get_name().c_str(),
                runned_[i]->data_.get_arr(),
                runned_[i]->data_.get_start(),
                runned_[i]->data_.get_bur(),
                runned_[i]->data_.get_fini(),
                runned_[i]->data_.get_T(),
                runned_[i]->data_.get_W());
    }
    printf("└──────┴────────┴────────┴────────┴────────┴────────┴────────────┘\n");
    float sum_T = 0;
    float sum_W = 0;
    for(int i = 0; i < runned_.size(); ++i) {
        sum_T += runned_[i]->data_.get_T();
        sum_W += runned_[i]->data_.get_W();
    }
    printf("平均周转时间: %.2f\n", sum_T/runned_.size());
    printf("平均周转时间: %.2f\n", sum_W/runned_.size());
}

void Sched::helper() {
    cout << "0.显示帮助(helper)" << endl;
    cout << "1.先来先服务(FCFS)" << endl;
    cout << "2.短作业优先(SJF)" << endl;
    cout << "3.时间片轮转(RR)" << endl;
    cout << "4.动态优先抢占(Preemptive Scheduling)" << endl;
    cout << "5.退出" << endl;
}

int main(int argc, char *argv[]) {
    Sched *s = new Sched;
    char cmd;
     while (1){
        cout << ">> ";
        cin >> cmd;
        switch(cmd) {
            case '0':s -> helper();break;
            case '1':s -> FCFS();break;
            case '2':s -> SJF();break;
            case '3':s -> RR();break;
            case '4':s -> PS();break;
            case '5':exit(0);
            default:continue;
        }
    }
    delete s;
    return 0;
}