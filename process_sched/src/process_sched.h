#ifndef SRC_PROCESS_SCHED_H_
#define SRC_PROCESS_SCHED_H_

#pragma once;

#include "utlis.h"
#include "cpptoml.h"
#include "list_header.h"
#include "pcb.h"

class Sched {
public:
    int cnt = 0;
    List<Pcb> ready_;
    List<Pcb> running_;
    List<Pcb> runned_;
    Sched() = default;
    void init();
    void calc(Pcb *p, List<Pcb> list);
    void FCFS();
    void SJF();
    void RR(int q);
};
#endif // PROCESS_SCHED_H_