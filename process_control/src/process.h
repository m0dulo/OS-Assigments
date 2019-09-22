#ifndef SRC_PROCESS_H_
#define SRC_PROCESS_H_

#include "pcb_header.h"
#include "list_header.h"
#include "memory_header.h"
#include "Argument_helper.h"
#include <tuple>


class Process {
private:
    List<Pcb> ready_;
    List<Pcb> running_;
    List<Pcb> blocked_;

    List<MemNode> used_;
    List<MemNode> free_;

    int frag_ = 1;

public:
    Process() = default;
    void init();
    std::tuple<bool, int, int> alloc(int size);
    void helper();
    void create();
    void round_robin();
    void block();
    void wake_up();
    void release();
    void show_pcb();
    void show_mem();
};


#endif // SRC_PROCESS_H_