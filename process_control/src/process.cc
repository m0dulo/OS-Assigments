/*
* Process Control
* Simulate operating system kernel management of processes and simple memory management.
* Memory Management Algorithm: Best Fit algorithm 
* Author: m0dulo
* Conact: cuckooegret@gmail.com
*/

#include "process.h"
#include <iostream>
#include <glog/logging.h>


using namespace google;
using namespace std;

const int SIZE = 0;
const int ADDR = 1;

void merge(List<MemNode> &free) {
    if (free.size() == 1)
        return;
    else {
        for (int i = 0; i < free.size() - 1; i++) {
            if(free[i] ->data_.get_end() == free[i + 1] -> data_.get_begin()) {
                free[i] ->data_.set(free[i] ->data_.get_begin(),                    
                                    free[i + 1] -> data_.get_end());
                free.remove(free[i + 1]);
            }
        }
    }
}

void Process::init() {
    cout << "请输入内存块的起始地址和大小：" << endl;
    MemNode *init_node(new MemNode);
    int begin, size;
    cin >> begin >> size;
    init_node -> set(begin, size + begin);
    free_.insertAsLast(*init_node);
}

void Process::create() {
    cout << "请输入创建的进程名和大小：" << endl;
    string name;
    int size;
    cin >> name >> size;
    auto ret = alloc(size);
    if (get<0>(ret) == false) {
        LOG(ERROR) << "可用内存不足，分配失败！"; 
        cerr << "可用内存不足，分配失败！" << endl;
    }
    else if(get<0>(ret) == true) {
        Pcb *p(new Pcb);
        p -> set(name, get<1>(ret), get<2>(ret));
        ready_.insertAsLast(*p);
        if (running_.empty()) {
            running_.insertAsLast(ready_[0] -> data_);
            ready_.remove(ready_[0]);
            cout << "分配成功！" << endl;
            LOG(INFO) << "分配成功！";
        }
    }
}

void Process::round_robin() {
    if ((running_.size() > 0) && (ready_.size() > 0)) {
        ready_.insertAsLast(running_[0] ->data_);
        running_.remove(running_[0]);
        running_.insertAsLast(ready_[0] -> data_);
        ready_.remove(ready_[0]);
        LOG(INFO) << "轮转成功！";
        cout << "轮转成功！" << endl;
    }
    else {
        LOG(ERROR) << "轮转失败！";
        cerr << "轮转失败！" << endl;
    }
}

void Process::block() {
    if (running_.size() > 0) {
        blocked_.insertAsLast(running_[0] -> data_);
        running_.remove(running_[0]);
        if (ready_.size() > 0) {
            running_.insertAsLast(ready_[0] -> data_);
            ready_.remove(ready_[0]);
        }
        LOG(INFO) << "阻塞成功！";
        cout << "阻塞成功！" << endl;
    }
    else {
        LOG(ERROR) << "阻塞失败！";
        cerr << "阻塞失败！" << endl;
    }
}

void Process::wake_up() {
    if (blocked_.size() > 0) {
        ready_.insertAsLast(blocked_[0] -> data_);
        blocked_.remove(blocked_[0]);
        if (running_.empty()) {
            running_.insertAsLast(ready_[0] -> data_);
            ready_.remove(ready_[0]);
        }
        cout << "唤醒成功！" << endl;
        LOG(INFO) << "唤醒成功！";
    }
    else {
        cerr << "唤醒失败！" << endl;
        LOG(ERROR) << "唤醒失败！";
    }
}



tuple<bool, int, int> Process::alloc(int size) {
    free_.sort(ADDR);
    merge(free_);
    free_.sort(SIZE);
    int iter = 0;
    for(; iter < free_.size(); iter++) {
        if (free_[iter] -> data_.get_size() - size > frag_) {
            MemNode *new_node(new MemNode);
            new_node -> set((free_[iter] -> data_.get_begin()), 
                    ((free_[iter] -> data_.get_begin()) + size));
            free_[iter] -> data_.cut(size);
            used_.insertAsLast(*new_node);
            return make_tuple(true, new_node -> get_begin(), size);
        }
        else if (free_[iter] -> data_.get_size() - size >= 0) {
            // MemNode *new_node(new MemNode);
            // new_node -> set((free_[iter] -> data_.get_begin()), 
            //         ((free_[iter] -> data_.get_begin()) + size));
            used_.insertAsLast(free_[iter] -> data_);
            auto link_node = free_[iter];
            int begin = link_node ->data_.get_begin();
            int size = link_node -> data_ .get_size();
            free_.remove(free_[iter]);
            return make_tuple(true, begin, size);
        }
    }
    if (iter == free_.size())
        return make_tuple(false,-2000000, 0);

    // used_.sort(ADDR);
    // merge(used_);
    // used_.sort(SIZE);

    free_.sort(ADDR);
    merge(free_);
    free_.sort(SIZE);
}

void Process::release() {
    if (running_.size() > 0) {
        int addr = running_[0] -> data_.get_addr();
        for (int i = 0; i < used_.size(); i++) {
            if (used_[i] -> data_.get_begin() == addr) {
                free_.insertAsLast(used_[i] -> data_);
                free_.sort(ADDR);
                merge(free_);
                free_.sort(SIZE);
                used_.remove(used_[i]);
                running_.remove(running_[0]);
                if (ready_.size() > 0) {
                    running_.insertAsLast(ready_[0] -> data_);
                    ready_.remove(ready_[0]);
                }
                return;
            }
        }
        cout << "终止成功！" << endl;
        LOG(INFO) << "终止成功！";
    }
    else {
        LOG(ERROR) << "终止失败！";
        cerr << "终止失败！" << endl;
    }
}




void Process::helper() {
    cout << "命令使用:" << endl
    << "1.创建:c" << endl
    << "2.进程调度 && 内存资源分配(最佳适应算法，自动)." << endl
    << "3.终止进程 && 回收内存资源:e 结束正在运行的进程" << endl
    << "4.时间片到:r 将运行的进程装入就绪队列尾部,将下一个自动调度过来." << endl
    << "5.阻塞:b 将正在执行的程序阻塞,即插入阻塞队列尾部." << endl
    << "6.唤醒:w 将阻塞队列中的一个进程调度到就绪队列上,即阻塞队列头到就绪队列尾." << endl
    << "7.查看:p 查看pcb调度情况" <<endl
    << "8.查看:m 查看内存使用情况" <<endl
    << "9.退出:q" << endl;
}

void Process::show_pcb() {
    cout << "Output Format: Name(begin, end)" << endl;
    cout << "Ready Queue: ";
    LOG(INFO)<< "Output Format: Name(begin, end)";
    LOG(INFO) << "Ready Queue: ";
    for (int i = 0; i < ready_.size(); i++) {
        cout << ready_[i] -> data_.get_name() << "("
             << ready_[i] -> data_.get_addr() << ", "
             << ready_[i] -> data_.get_size() + ready_[i] -> data_.get_addr() << ") ";
        LOG(INFO) << ready_[i] -> data_.get_name() << "("
             << ready_[i] -> data_.get_addr() << ", "
             << ready_[i] -> data_.get_size() + ready_[i] -> data_.get_addr() << ") ";
    }
    cout << endl;

    cout << "Running Queue: ";
    LOG(INFO) << "Running Queue: ";
    for (int i = 0; i < running_.size(); i++) {
        cout << running_[i] -> data_.get_name() << "("
             << running_[i] -> data_.get_addr() << ", "
             << running_[i] -> data_.get_size() + running_[i] -> data_.get_addr() << ") ";
        LOG(INFO) << running_[i] -> data_.get_name() << "("
             << running_[i] -> data_.get_addr() << ", "
             << running_[i] -> data_.get_size() + running_[i] -> data_.get_addr() << ") ";
    }
    cout << endl;

    cout << "Blocked Queue: ";
    LOG(INFO) << "Blocked Queue: ";
    for (int i = 0; i < blocked_.size(); i++) {
        cout << blocked_[i] -> data_.get_name() << "("
             << blocked_[i] -> data_.get_addr() << ", "
             << blocked_[i] -> data_.get_size() + blocked_[i] -> data_.get_addr() << ") ";
    }
    cout << endl;
}

void Process::show_mem() {

    free_.sort(ADDR);
    merge(free_);
    free_.sort(SIZE);
    cout << "Output Format: (begin, end) in ascending order of free area size" << endl;
    cout << "Free Memory: ";
    LOG(INFO) << "Output Format: (begin, end) in ascending order of free area size";
    LOG(INFO) << "Free Memory: ";
    for (int i = 0; i < free_.size(); i++) {
        cout << "(" << free_[i] -> data_.get_begin() << ", "
             << free_[i] -> data_.get_end() << ")";
        LOG(INFO) << "(" << free_[i] -> data_.get_begin() << ", "
             << free_[i] -> data_.get_end() << ")";
    }
    cout << endl;

    // used_.sort(ADDR);
    // merge(used_);
    // used_.sort(SIZE);

    cout << "Used Memory: ";
    for (int i = 0; i < used_.size(); i++) {
        cout << "(" << used_[i] -> data_.get_begin() << ", "
             << used_[i] -> data_.get_end() << ") ";
        LOG(INFO) << "(" << used_[i] -> data_.get_begin() << ", "
             << used_[i] -> data_.get_end() << ") ";
    }
    cout << endl;
}


int main(int argc, char *argv[]) {
    InitGoogleLogging(argv[0]);
    FLAGS_log_dir = "../log/";
    FLAGS_colorlogtostderr = true; 
    Process *p(new Process);
    bool b_help = false;
    dsr::Argument_helper ah;
    ah.new_flag("h", "help", "argument help", b_help);
    ah.process(argc, argv);
    char c;
    p -> init();
    if (b_help)
        p -> helper();
    while (1){
        cout << ">";
        cin >> c;
        switch(c) {
            case 'q':exit(0);
            case 'c':p -> create();break;
            case 'e':p -> release();break;
            case 'r':p -> round_robin();break;
            case 'b':p -> block();break;
            case 'w':p -> wake_up();break;
            case 'p':p -> show_pcb();break;
            case 'm':p -> show_mem();break;
            case 'h':p -> helper();break;
            default:continue;
        }
    }
    delete p;
    // Pcb *p(new Pcb())
    return 0;
}