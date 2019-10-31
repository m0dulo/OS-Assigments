
#include "mgmt.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <limits.h>

using namespace std;
using namespace lyx_utlis;

#define RED "\033[31m"
#define WHITE "\033[37m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"  
#define YELLOW "\033[33m"
#define CYAN "\033[0;36m"

void Mgmt::vir_update() {
    FILE *fp = fopen(file_name.c_str(),"w");
    fwrite(fat_, sizeof(FatNode) * fat_len_, 1, fp);
    fwrite(block_, sizeof(Block) * fat_len_, 1, fp); 
}
void Mgmt::vir_disk_init() {
    bool fat_flag = false;
    bool block_flag = false;
    mkdir(data_path.c_str(), 0777);
    fat_[0] = UINT16_MAX;
    string date = current_time();
    char curr[] = "root";
    strcpy(block_[0].f[0].name_, curr);
    block_[0].f[0].type_ = 2;
    block_[0].f[0].first_block_ = 0;
    block_[0].f[0].size_ = 1024;
    strcpy(block_[0].f[0].date_time_, date.c_str());
    char up[] = "root";
    strcpy(block_[0].f[1].name_, up);
    block_[0].f[1].type_ = 2;
    block_[0].f[1].first_block_ = 0;
    block_[0].f[1].size_ = 1024;
    strcpy(block_[0].f[1].date_time_, date.c_str());
    set_path(0);
    vir_update();
}

void Mgmt::set_fat_len(int len) {
    this -> fat_len_ = (len / 2);
}

void Mgmt::block_init() {
    block_ = (Block*)malloc(sizeof(Block) * fat_len_);
    memset(this -> block_, 0x00, fat_len_);
}

void Mgmt::fat_init() {
    set_fat_len(16);
    fat_ = (FatNode*)malloc(sizeof(FatNode) * fat_len_);
    memset(this -> fat_, 0x00, this -> fat_len_);
}

void Mgmt::format() {
    memset(fat_, 0, sizeof(char) * 8208);
    vir_disk_init();
    open_vir_disk();
    set_path(0);
}

void Mgmt::open_vir_disk() {
    FILE *fp;
    if(!(fp = fopen(file_name.c_str(),"r"))){
        cout<<"未找到该文件,打开失败"<<endl;
        return ;
    }
    fread(fat_, sizeof(FatNode) * fat_len_, 1, fp);
    fread(block_, sizeof(Block) * fat_len_, 1, fp);
}

void Mgmt::set_path(int p) {
    path_ = p;
}

int Mgmt::get_path() const{
    return path_;
}

void Mgmt::fcb_copy(Fcb &a, Fcb b) {
    a.type_ = b.type_;
    a.size_ = b.size_;
    a.first_block_ = b.first_block_;
    strcpy(a.name_, b.name_);
    strcpy(a.date_time_, b.date_time_);
}

void Mgmt::md(const char *dir_name) {
    int fat_idx = 1;
    int i;
    for (;fat_idx < fat_len_; ++fat_idx) {
        if (fat_[fat_idx] == 0x0000)
            break;
    }
    if (fat_idx == fat_len_) {
        cout << "磁盘空间已经满！" << endl;
        return;
    }
    int path = get_path();
    for (i = 2; i < 32; ++i) {
        if (block_[path].f[i].type_ == 0) {
            break;
        }
    }
    if (i == 32) {
        cout<<"目录中文件达上限,无法创建"<<endl;
        return ;
    }
    fat_[fat_idx] = 0xffff;
    block_[path].f[i].type_ = 2;
    block_[path].f[i].size_ = 0;
    block_[path].f[i].first_block_ = fat_idx;
    string date = current_time();
    strcpy(block_[path].f[i].date_time_, date.c_str());
    strcpy(block_[path].f[i].name_, dir_name);
    fcb_copy(block_[fat_idx].f[0], block_[path].f[i]);
    fcb_copy(block_[fat_idx].f[1], block_[path].f[0]);
    vir_update();
}

void Mgmt::mk(const char *name, int size) {
    int i;
    int check = 0;
    string date;
    date = current_time();
    int block_num = ceil((size * 1.0 ) / 1024 );
    for (i = 1; i < fat_len_; ++i) {
        if(fat_[i] == 0x0000) {
            check++;
        }   
    }

    if (check < block_num) {
        cout << "磁盘空间已满！" << endl;
        return;
    }

    int first = 1;
    for (;first < fat_len_; ++first) {
        if (fat_[first] == 0) {
            break;
        }
    }

    int idx;
    int path = get_path();
    for (idx = 2; idx < 32; ++idx) {
        if (block_[path].f[idx].type_ == 0) {
            break;
        }
    }
    if (idx == 32) {
        cout<<"目录中文件达上限,无法创建"<<endl;
        return ;
    }

    if (block_num == 1) {
        fat_[first] = 0xffff;
        block_[first].f[0].size_ = size;
        strcpy(block_[first].f[0].name_,name);
        block_[first].f[0].type_ = 1;
        block_[first].f[0].first_block_ = first;
        strcpy(block_[first].f[0].date_time_, date.c_str()); 
    } else if (block_num > 1) {
        for (int j = first; j < (first + (block_num - 1)); ++j) {
            fat_[j] = j + 1;
            block_[j].f[0].size_= 1024;
            strcpy(block_[j].f[0].name_,name);
            block_[j].f[0].type_ = 1;
            block_[j].f[0].first_block_ = first;
            strcpy(block_[j].f[0].date_time_, date.c_str()); 
        }

        fat_[first + (block_num - 1)] = 0xFFFF;
        block_[first + (block_num - 1)].f[0].size_ = size - (block_num - 1) * 1024;
        strcpy(block_[first + (block_num - 1)].f[0].name_,name);
        block_[first + (block_num - 1)].f[0].type_ = 1;
        block_[first + (block_num - 1)].f[0].first_block_ = first;
        strcpy(block_[first + (block_num - 1)].f[0].date_time_, date.c_str()); 
    }
    block_[path].f[idx].type_ = 1;
    block_[path].f[idx].size_ = size;
    strcpy(block_[path].f[idx].name_, name);
    strcpy(block_[path].f[idx].date_time_, date.c_str());
    block_[path].f[idx].first_block_ = first;
    vir_update();
} 

void Mgmt::rd(const char *name) {
    int idx;
    int path = get_path();
    for (idx = 2; idx < 32; ++idx) {
        if (!strcmp(block_[path].f[idx].name_, name)) {
            if (block_[path].f[idx].type_ == 1) {
                cout << "此命令不可用于删除文件！" << endl;
                return;
            }
            else
                break;
        }
    }
    if (idx == 32) {
       cout<<"未找到该目录"<<endl;
       return ; 
    }

    int num = block_[path].f[idx].first_block_;
    for (int j = 2; j < 32; ++j) {
        if (block_[num].f[j].type_) {
            cout<<"不能删除非空子目录!"<<endl;
            return ;
        }
    }

    fat_[num] = 0;
    memset(&(block_[path].f[idx]), 0, sizeof(Fcb));
    memset(&(block_[num]), 0, sizeof(Block));
    vir_update(); 
}

void Mgmt::del(const char *name) {
    int idx;
    int path = get_path();
    for (idx = 2; idx < 32; ++idx) {
        if (!strcmp(block_[path].f[idx].name_ , name)) {
            if (block_[path].f[idx].type_ == 2) {
                cout << "此命令不可用于删除目录！" << endl;
                return;
            }
            else
                break;
        }
    }

    if (idx == 32) {
        cout<<"未找到该文件"<<endl;
        return ;
    }

    int num  = block_[path].f[idx].first_block_;
    int i;
    vector<int> idxs;
    for (i = num; i < fat_len_; ++i) {
        idxs.push_back(i);
        if (fat_[i] == 0xFFFF)
            break;
        if (fat_[i] != 0xFFFF)
            fat_[i] = 0;
    }
    fat_[i] = 0;
    int size = idxs.size();
    int k;
    for (k = num; k < (num + size); ++k) {
        memset(block_[k].f, 0, sizeof(Fcb));
    }
    memset(&(block_[path].f[idx]), 0, sizeof(Fcb));
    vir_update();
}

void Mgmt::cd(const char *name) {
    if (!strcmp(name, "root")) {
        set_path(0);
        return;
    }

    if (!strcmp(name, "\\")) {
        set_path(0);
        return;
    }

    if (!strcmp(name, ".")) {
        set_path(get_path());
        return;
    }

    if (!strcmp(name, "..")) {
        int path = get_path();
        set_path(block_[path].f[1].first_block_);
        return;
    }

    int idx;
    int path = get_path();
    for (idx = 1; idx < 32; ++idx) {
        if (!strcmp(block_[path].f[idx].name_, name)
            &&block_[path].f[idx].type_ == 2) {
                break;
            }
    }
    if (idx == 32) {
        cout<<"未找到该路径"<<endl;
        return ;
    }
    set_path(block_[path].f[idx].first_block_);
}

void Mgmt::find_dir(int path) {
    if (block_[path].f[1].type_ == 2 && block_[path].f[1].first_block_ > 0) {
        dir_name.push(block_[path].f[1].name_);
        find_dir(block_[path].f[1].first_block_);
    }
}

void Mgmt::dir() {
    int path = get_path();
    for (int i = 0; i < 32; i++) {
            if (block_[path].f[i].type_) {
                cout << block_[path].f[i].first_block_ << "\t";
                cout << block_[path].f[i].date_time_;
                if (block_[path].f[i].type_ == 2)
                    cout << "\t" << "<DIR>" << "\t\t";
                else 
                    cout << "\t\t" << block_[path].f[i].size_ << "\t";
                if (i == 0)
                    cout << "." << endl;
                else if (i == 1)
                    cout << ".." << endl;
                else if (i > 1)
                    cout << block_[path].f[i].name_ << endl;
            }
    }
}

void Mgmt::info(int addr) {
    mem_ = new char[8208];
    ifstream ifs(file_name, ios::binary);
    ifs.read(mem_, 8208);
    cout << RED;
    printf("Address");
    cout << "\t\t";
    for (int i = 0; i < 16; ++i) {
        printf("%02x ",i);
    }
    cout << "\t";
    cout << "Dump" << WHITE << endl;
    if ( ((513 - ((addr / 16) + 1) + 1)  * 1.0 / 16) < 1) {
        for (int i = (addr / 16); i < 513; ++i) {
            vector<char> ch;
            printf("%06x ", i * 16);
            printf("%04d", i * 16);
            cout << "\t";
            for (int j = 0; j < 16; ++j) {
                if ((mem_[i * 16 + j] >= 0x21 && mem_[i * 16 + j] <= 0x7e))
                    ch.push_back(mem_[i * 16 + j]);
                else
                    ch.push_back('.');
                unsigned char c = (unsigned char)mem_[i * 16 + j];
                printf("%02x ", c);
            }
            cout << "\t";
            for(auto i : ch) {
                cout << i;
            }
            cout << endl;
        }
    }
    else {
    for (int i = (addr / 16); i < (addr / 16) + 16; ++i) {
            vector<char> ch;
            printf("%06x ", i * 16);
            printf("%04d", i * 16);
            cout << "\t";
            for (int j = 0; j < 16; ++j) {
                if ((mem_[i * 16 + j] >= 0x21 && mem_[i * 16 + j] <= 0x7e))
                    ch.push_back(mem_[i * 16 + j]);
                else
                    ch.push_back('.');
                unsigned char c = (unsigned char)mem_[i * 16 + j];
                printf("%02x ", c);
            }
            cout << "\t";
            for(auto i : ch) {
                cout << i;
            }
            cout << endl;
        }
    }
    ifs.close();
    delete []mem_;
    mem_ = nullptr;
}

void Mgmt::tree(int path, int blk) {
    for (int i = 0; i < blk; ++i)
        cout << "   ";
    if (path == 0) 
        cout << "|—<" << RED << block_[path].f[0].name_ << WHITE <<  ">" << ":" << block_[path].f[0].first_block_ <<  endl;
    else if (path > 0) {
        if (block_[path].f[0].type_ == 1)
            cout << "|—<" << BLUE << block_[path].f[0].name_ << "(FILE)"<< ":" << block_[path].f[0].first_block_ << WHITE <<  ">" << endl;
        else if (block_[path].f[0].type_ == 2)
            cout << "|—<" << GREEN << block_[path].f[0].name_ << "(DIR)"<< ":" << block_[path].f[0].first_block_ << WHITE << endl;
    }

    for (int j = 2; j < 32; ++j) {
        if (block_[path].f[j].type_ == 1 || block_[path].f[j].type_ == 2) {
            tree(block_[path].f[j].first_block_, blk + 1);
        }
    }
}

void Mgmt::pwd() {
    cout << "PATH" << endl;
    cout << "-------" << endl;
    if (get_path() == 0)
        cout << "root\\";
    else if (get_path() > 0) {
        find_dir(get_path());
        cout << "root\\";
        while(!dir_name.empty()) {
            cout << dir_name.top() << "\\";
            dir_name.pop();
        }
        cout << block_[get_path()].f[0].name_ << "\\"<< endl;
    }
    cout << endl;
}


void Mgmt::check_disk() {
    vector<int> used;
    vector<int> free;
    for (int i = 0; i < 8; ++i) {
        if(fat_[i] == 0)
            free.push_back(i);
        else
            used.push_back(i);
    }
    cout << "占用的盘号为：";
    for (int i : used) {
        cout << i << " ";
    }
    cout << endl;
    cout << "空闲的盘号为：";
    for (int i : free) {
        cout << i << " ";
    }
    cout << endl;
    cout << "占用率：";
    printf("%0.2f",((float)used.size() / 8) * 100);
    cout << "%" << endl;

}

int Mgmt::run() {  
    if (get_path() == 0)
        cout << "root:\\> ";
    else if (get_path() > 0) {
        find_dir(get_path());
        cout << "root:\\";
        while(!dir_name.empty()) {
            cout << dir_name.top() << "\\";
            dir_name.pop();
        }
        cout << block_[get_path()].f[0].name_ << "> ";
    }
    string cmd;
    getline(cin, cmd);
    tolower(cmd);
    vector<string> tokens;
    tokens.clear();
    split(cmd, tokens, " ");
    if (tokens.at(0) == "format") {
        format();
        
    }
    else if (tokens.at(0) == "dir")
        dir();
    else if (tokens.at(0) == "md") {
        string dir_name;
        dir_name = tokens.at(1);    
        md(dir_name.c_str());
    }
    else if (tokens.at(0) == "rd") {
        string dir_name;
        dir_name = tokens.at(1);
        rd(dir_name.c_str());
    }
    else if (tokens.at(0) == "mk") {
        string name;
        name = tokens.at(1);
        const char *s = tokens.at(2).c_str();
        int size = atoi(s); 
        mk(name.c_str(), size);
    }
    else if (tokens.at(0) == "del") {
        string name;
        name = tokens.at(1);
        del(name.c_str());
    }
    else if (tokens.at(0) == "cd") {
        string name;
        name = tokens.at(1);
        cd(name.c_str());
    }

    else if (tokens.at(0) == "tree") {
        tree(get_path(),0);
    }
    else if (tokens.at(0) == "help") {
        helper();
    }
    else if (tokens.at(0) == "pwd") {
        pwd();
    }
    else if (tokens.at(0) == "info") {
        const char * s = tokens.at(1).c_str();
        int addr = atoi(s);
        info(addr);
    }
    else if (tokens.at(0) == "check") {
        check_disk();
    }
    else if (tokens.at(0) == "quit")
        return 0;
    else cout<<"错误的命令格式,请查看帮助!"<<endl;
    return 1;  
}

void Mgmt::helper() {
    cout << RED << "以下命令均支持大小写" << WHITE << endl;
    cout<< GREEN << "1.MD(创建空目录)              命令:MD(md) dir_name"<<endl;
    cout<<"2.CD(切换当前目录)            命令:CD(cd) dir_name"<<endl;
    cout<<"3.RD(删除空目录)              命令:RD(rd) dir_name"<<endl;
    cout<<"4.MK(创建文件)                命令:MK(mk) name size"<<endl;
    cout<<"5.DEL(删除文件)               命令:DEL(del) dir_name"<<endl;
    cout<<"6.DIR(列出目录信息)           命令:DIR(dir)"<<endl;	
    cout<<"7.PWD(显示工作全路径)         命令:PWD(pwd)"<<endl;
    cout<<"8.TREE(树形打印)              命令:TREE(tree))"<<endl;
    cout<<"9.FORMAT(格式化虚拟盘)        命令:FORMAT(format)"<<endl;
    cout<<"11.HELP(显示帮助)             命令:HELP(help)"<<endl;
    cout<<"12.INFO(输出虚拟盘信息)       命令:INFO(info) addr"<<endl;
    cout<<"13.CHECK(检查虚拟盘使用情况)   命令:QUIT(quit)"<< WHITE << endl;
    cout<<"14.QUIT(退出程序)             命令:QUIT(quit)"<< WHITE << endl;
}

void Mgmt::check() {
    fat_init();
    block_init();
    if (exists(file_name)) {
        open_vir_disk();
        return;
    } else {
        vir_disk_init();
    }
}

int main(int argc, char *argv[]) {
    cout << sizeof(Fcb) << endl;
    Mgmt m;
    m.check();
    while(m.run());
    return 0;
}   