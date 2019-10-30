#ifndef SRC_MGMT_H_
#define SRC_MGMT_H_

#pragma once;

#include "mgmt.h"
#include "utlis.h"
#include <memory.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "fcb.h"
#include <time.h>
#include <stdio.h>
#include "block.h"
#include <string>
#include <cstring>
#include <math.h>

typedef uint16_t FatNode;

const std::string data_path = "../data/";

const std::string vir_file_name = "virtual.disk";

const std::string file_name = data_path + vir_file_name;

class Mgmt{
public:
    int path_ = 0;
    std::string str1 = "lyx";
    int fat_len_;
    FatNode *fat_ = nullptr;
    Block *block_ = nullptr;
    void set_fat_len(int len);
    void fat_init();
    void block_init();
    void vir_disk_init();
    void run();
    void command_parser();
    void open_vir_disk();
    void set_init();
    void vir_update();
    void test();
    void test1();
    void md();
    void set_path(int p);
    int get_path() const;
    void fcb_copy(Fcb &a, Fcb b);
    void mk();
    void rd();
    void del();
    void cd();
    void dir();
    void info();
};

#endif // SRC_MGMT_H_