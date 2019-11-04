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
#include <stack>

typedef uint16_t FatNode;

const std::string data_path = "../data/";

const std::string vir_file_name = "virtual.disk";

const std::string file_name = data_path + vir_file_name;

class Mgmt{
public:
    char *mem_ = nullptr;
    int path_ = 0;
    int fat_len_;
    FatNode *fat_ = nullptr;
    Block *block_ = nullptr;
    std::stack<char *> dir_name;
    void set_fat_len(int len);
    void fat_init();
    void block_init();
    void vir_disk_init();
    int run();
    void command_parser();
    void open_vir_disk();
    void set_init();
    void vir_update();
    void test();
    void test1();
    void md(const char *dir_name);
    void set_path(int p);
    int get_path() const;
    void fcb_copy(Fcb &a, Fcb b);
    void mk(const char *file_name, int size);
    void rd(const char *dir_name);
    void del(const char *file_name);
    void cd(const char *dir_name);
    void dir();
    void info(int addr);
    void find_dir(int path);
    void format();
    void check();
    void tree(int path, int blk);
    void helper();
    void pwd();
    void check_disk();
};

#endif // SRC_MGMT_H_