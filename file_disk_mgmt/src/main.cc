
#include "mgmt.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <fstream>

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
    fat_[0] = 0xFFFF;
    string date = current_time();
    char curr[] = ".";
    strcpy(block_[0].f[0].name_, curr);
    block_[0].f[0].type_ = 2;
    block_[0].f[0].first_block_ = 10;
    block_[0].f[0].size_ = 1024;
    strcpy(block_[0].f[0].date_time_, date.c_str());
    char up[] = "..";
    strcpy(block_[0].f[1].name_, up);
    block_[0].f[1].type_ = 2;
    block_[0].f[1].first_block_ = 10;
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


void Mgmt::run() {
    cout << "root:\\>" << endl;
    if (exists(file_name)) {
        return;
    } else {
        vir_disk_init();
    }
}



void Mgmt::open_vir_disk() {
    FILE *fp;
    if(!(fp = fopen(file_name.c_str(),"r"))){
        cout<<"未找到该文件,打开失败"<<endl;
        return ;
    }
    fread(fat_, sizeof(FatNode) * fat_len_, 1, fp);
    fread(block_, sizeof(Block) * fat_len_, 1, fp);
    cout << fat_[0] << endl;
    cout << block_[0].f[0].date_time_ << endl;
    cout << block_[0].f[0].size_ << endl;
    cout << block_[0].f[0].name_ << endl;
    cout << block_[0].f[0].first_block_ << endl;
    cout << block_[0].f[0].type_ << endl;
    cout << block_[0].f[1].date_time_ << endl;
    cout << block_[0].f[1].name_ << endl;
    cout << block_[0].f[1].size_ << endl;
    cout << block_[0].f[1].first_block_ << endl;
    cout << block_[0].f[1].type_ << endl;
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

void Mgmt::md() {
    char dir_name[5];
    cin >> dir_name;
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

void Mgmt::mk() {
    char name[5];
    int i;
    int size;
    int check = 0;
    cin >> name;
    cin >> size;
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
    cout << "first: " << first << endl;
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

void Mgmt::rd() {
    char name[5];
    cin >> name;
    int idx;
    int path = get_path();
    for (idx = 2; idx < 32; ++idx) {
        if (!strcmp(block_[path].f[idx].name_, name)) {
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
    block_[path].f[idx].type_ = 0;
    vir_update(); 
}

void Mgmt::del() {
    char name[5];
    cin >> name;
    int idx;
    int path = get_path();
    for (idx = 2; idx < 32; ++idx) {
        if (!strcmp(block_[path].f[idx].name_ , name))
            break;
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
    for (k = num; k < size; ++k) {
        memset(block_[k].f, 0, sizeof(Fcb));
    }
    memset(&(block_[path].f[idx]), 0, sizeof(Fcb));
    // block_[path].f[idx].type_ = 0;
    vir_update();
}

void Mgmt::cd() {
    char name[5];
    cin >> name;
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
        set_path(block_[path].f[0].first_block_);
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
            cout << block_[path].f[i].name_ << endl;
        }
    }
}

void Mgmt::info() {
    
}



void Mgmt::test() {
    char n[] = "upda";
    strcpy(block_[0].f[1].name_, n);
    vir_update();
}
void Mgmt::test1() {
    char n[] = "ada";
    strcpy(block_[0].f[0].name_, n);
    vir_update();
}

int main(int argc, char *argv[]) {
    Mgmt m;
    m.fat_init();
    m.block_init();
    //m.vir_disk_init();
    m.run();    //    
    m.open_vir_disk();
    // m.md();
    // m.mk();
    // m.dir();
    m.mk();
    m.mk();
    m.md();
    m.dir();
    m.cd();
    m.dir();
    m.cd();
    m.dir();
    //m.test();
    //m.open_vir_disk();
    //m.test1();
    //m.open_vir_disk();
    //m.md();
    
        // m.fat_init();
    // string s = "ABCd";
    // lyx_utlis::tolower(s);
    // lyx_utlis::toupper(s);
    // string date = current_time();
    // char a[13];
    // strcpy(a, date.c_str());
    // printf("%s\n",a);
    // Fcb *f = new Fcb;
    // cin.get(f -> name_, NAME_SIZE);
    // cout << f -> name_ << endl;
    // cin.get(test,8);
    // cout << test << endl;
    // cout << sizeof(test) << endl;
    // strcpy(m.block_[0].f[0].name_, "lyx");
    // cout << m.block_[0].f[0].name_ << endl;
}   