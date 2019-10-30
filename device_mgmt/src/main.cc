#include "mgmt.h"
#include <iostream>

#include <string>

#define RED "\033[31m"
#define WHITE "\033[37m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"  
#define YELLOW "\033[33m"
#define CYAN "\033[0;36m"

using namespace std;

void Mgmt::helper() {
    cout << endl;
    cout <<"\t++++++++++++++*HELPER*++++++++++++++"<<endl;
    cout <<"\t===========*AUTHOR:m0dulo*=========="<<endl;
    cout<<"\t|c.添加设备------------------命令:c|"<<endl;
    cout<<"\t|d.删除设备------------------命令:d|"<<endl;	
    cout<<"\t|a.申请设备------------------命令:a|"<<endl;
    cout<<"\t|r.释放设备------------------命令:r|"<<endl;
    cout<<"\t|i.申请一类设备(设备独立性)--命令:i|"<<endl;
    cout<<"\t|t.撤销进程------------------命令:t|"<<endl;
    cout<<"\t|k.进程状态------------------命令:k|"<<endl;
    cout<<"\t|s.分配状态------------------命令:s|"<<endl;
    cout<<"\t|p.树形打印------------------命令:p|"<<endl;
    cout<<"\t|h.显示帮助------------------命令:h|"<<endl;
    cout<<"\t|q.退出程序------------------命令:q|"<<endl;
    cout <<"\t===================================="<<endl;
}

void Mgmt::init() {
    cpu -> init("CPU");
    ChNode *ch = new ChNode[2];
    for (int i = 0; i < 2; ++i) {
        string idx = to_string(i + 1);
        ch[i].set_rank(1);
        chct.push_back(&ch[i]);
        chct[i]->data_->init("CH" + idx);
    }
    delete []ch;

    CoNode *co = new CoNode[3];
    for (int i = 0; i < 3; ++i) {
        string idx = to_string(i + 1);
        co[i].set_rank(2);
        coct.push_back(&co[i]);
        coct[i]->data_->init("CO" + idx);
    }
    delete []co;

    DcNode *dc = new DcNode[4];
    for (int i = 0; i < 4; ++i) {
        dc[i].set_rank(3);
        dct.push_back(&dc[i]);
    }
    delete []dc;
    dct[0]->data_->init("KB");
    dct[0]->data_->set_type('I');
    dct[1]->data_->init("MOUSE");
    dct[1]->data_->set_type('I');
    dct[2]->data_->init("PTR");
    dct[2]->data_->set_type('O');
    dct[3]->data_->init("MOR");
    dct[3]->data_->set_type('O');

    coct[0] ->data_->set_parent(*chct[0]->data_);
    chct[0]->data_->add_child(*coct[0]->data_);

    coct[1] ->data_->set_parent(*chct[1]->data_);
    chct[1]->data_->add_child(*coct[1] ->data_);

    coct[2] ->data_->set_parent(*chct[1]->data_);
    chct[1]->data_->add_child(*coct[2] ->data_);

    dct[0] ->data_->set_parent(*coct[0]->data_);
    coct[0]->data_->add_child(*dct[0] ->data_);

    dct[1] ->data_->set_parent(*coct[0]->data_);
    coct[0]->data_->add_child(*dct[1] ->data_);

    dct[2] ->data_->set_parent(*coct[1]->data_);
    coct[1]->data_->add_child(*dct[2] ->data_);
    
    dct[3] ->data_->set_parent(*coct[2]-> data_);
    coct[2]->data_->add_child(*dct[3] ->data_);

    cpu ->add_child(*chct[0] -> data_);
    cpu -> add_child(*chct[1] -> data_);
}

void Mgmt::add_dc() {
    int dc_flag = 0;
    cout << "请输入设备名称：" << endl << "> ";
    string dc_name;
    char dc_type;
    string ch_name;
    string co_name;
    cin >> dc_name;
    cout << "请输入设备属性(I/O)：" << endl << "> ";
    cin >> dc_type;
    for (int i = 0; i < dct.size(); ++i) {
        if (dc_name == dct[i] ->data_->get_name()) {
            dc_flag = 1;
            break;
        }
    }
    if (dc_flag == 1) {
        cout << RED << "设备已经存在！添加设备失败！" << WHITE << endl;
        add_dc();
    } else if (dc_flag == 0) {
        DcNode *dc = new DcNode;
        dc ->init(dc_name);
        dc -> set_type(dc_type);
        dct.push_back(dc);
        add_co();
    }

}

void Mgmt::add_co() {
    bool flag = 0;
    char cmd;
    cout << "是否添加新的控制器？(Y/N)" << endl <<  "> ";
    cin >> cmd;
    if (cmd == 'Y'){
        cout << "请输入控制器名称：" << endl << "> ";
        string name;
        cin >> name;
        flag = coct.check(name);
        if (flag == true) {
            cout << endl << RED << "设备已经存在！添加设备失败！" << WHITE << endl;
            add_co(); 
        } else if (flag == false) {
            CoNode *co = new CoNode;
            co -> init(name);
            coct.push_back(co);
            dct.back()->set_parent(*coct.back());
            coct.back()->add_child(*dct.back());

            string ch_name;
            cout << "请输入新设备要挂载的通道名称:" << endl << "> ";
            cin >> ch_name;
            int r = chct.find(ch_name);
            coct.back()->set_parent(*chct[r]->data_);
            chct[r]->data_->add_child(*coct.back());   
            cout << endl << GREEN << "添加新设备成功！" << WHITE << endl << endl;    
        }   
    } else if (cmd == 'N') {
        string n_name;
        cout << endl << "请输入新添加设备要挂载的控制器名称:" << endl << "> ";
        cin >> n_name;
        int r = coct.find(n_name);
        dct.back()->set_parent(*coct[r]->data_);
        coct[r]->data_->add_child(*dct.back());
        cout << endl << GREEN << "添加新设备成功！" << WHITE << endl << endl;
    }
}

void Mgmt::apply(string dc_name) {
    cout << "请输入进程名:" << endl << "> ";
    string pcb_name;
    cin >> pcb_name;
    Pcb *pcb = new Pcb;
    pcb -> init(pcb_name);
    if(!ready.check(pcb_name)) {
        ready.push_back(pcb);
    }
    int r = dct.find(dc_name);
    if (dct[r] ->data_ -> process == nullptr) {
        dct[r] ->data_ -> process = pcb;
        int d = ready.find(pcb -> get_name());
        Pcb *block = ready[d] -> data_;
        ready.remove(ready[d]);
        pcb ->set_bolcking();
        if (!blocking.check(block ->get_name()))
            blocking.push_back(block);
        cout << GREEN << "分配成功！" << WHITE << endl;
        if (dct[r] -> data_ -> parent -> process == nullptr) {
            dct[r] -> data_ -> parent -> process = block;
            if (!blocking.check(block ->get_name()))
                blocking.push_back(block);
            if (dct[r] -> data_ -> parent -> parent -> process == nullptr) {
                dct[r] -> data_ -> parent -> parent -> process = block;
                if (!blocking.check(block ->get_name()))
                    blocking.push_back(block);
            }else{
                dct[r] -> data_ -> parent -> parent -> waiting_list.push_back(block);
                if (!blocking.check(block ->get_name()))
                    blocking.push_back(block);
            }
        } else {
            dct[r] -> data_ -> parent -> waiting_list.push_back(block);
            if (!blocking.check(block ->get_name()))
                blocking.push_back(block);
        }
    } else {
        dct[r] ->data_ -> waiting_list.push_back(pcb);
        int d = ready.find(pcb -> get_name());
        Pcb *block = ready[d] -> data_;
        ready.remove(ready[d]);
        pcb ->set_bolcking();
        blocking.push_back(block);
        cout << RED << "分配失败！" << WHITE << endl;
    }
}

void Mgmt::release(string dc_name) {
    if (!dct.check(dc_name)) {
        cout << RED << "没有这个设备，释放设备失败！" << WHITE << endl;
        return;
    }
    string pro_name;
    int r = dct.find(dc_name);
    int c = 0;
    bool b_assigned = false;
    for (; c < dct.size(); ++c) {
        if (c == r) continue;
        if (dct[c] -> data_ -> process != nullptr) {
            if (dct[r] -> data_ -> process -> get_name() == dct[c] -> data_ -> process -> get_name()) {
                b_assigned = true;
                break;
            }
        }
    }
    if (dct[r] -> data_ -> waiting_list.size() == 0) {
        if (!b_assigned) {
            dct[r] -> data_ -> process -> set_ready();
            int d = blocking.find(dct[r] ->data_-> process ->get_name());
            blocking.remove(blocking[d]);
            if (!ready.check(dct[r] -> data_ -> process -> get_name()) || ready.size() == 0) {
                ready.push_back(dct[r] -> data_ -> process);
            }
        }
        pro_name = dct[r] -> data_ -> process -> get_name();
        dct[r] -> data_ -> process = nullptr;
    } else {
        if (!b_assigned) {
            dct[r] -> data_ -> process -> set_ready();
            int d = blocking.find(dct[r] ->data_-> process ->get_name());
            blocking.remove(blocking[d]);
            if (!ready.check(dct[r] -> data_ -> process -> get_name()) || ready.size() == 0) {
                ready.push_back(dct[r] -> data_ -> process);
            }
        }
        pro_name = dct[r] -> data_ -> process -> get_name();
        dct[r] -> data_ -> process = nullptr;
        dct[r] -> data_ -> process = dct[r] -> data_ -> waiting_list.first() -> data_;
        dct[r] -> data_ -> waiting_list.dequeue();

        if (dct[r] -> data_ -> parent -> process == nullptr) {
            dct[r] -> data_ -> parent -> process = dct[r] -> data_ -> process;
        } else {
            dct[r] -> data_ ->parent -> waiting_list.enqueue(dct[r] -> data_ -> process);
        }
    }
    if (dct[r] -> data_ -> parent -> waiting_list.check(pro_name)) {
                int rr = dct[r] -> data_ -> parent -> waiting_list.find(pro_name);
                dct[r] -> data_ -> parent -> waiting_list.remove(dct[r] -> data_ -> parent -> waiting_list[rr]);
    }

    if (dct[r] -> data_ -> parent -> process -> get_name() == pro_name) {
        if (dct[r] -> data_ -> parent -> waiting_list.size() == 0) {
            dct[r] -> data_ -> parent -> process = nullptr;
        } else {
            dct[r] -> data_ -> parent -> process = nullptr;
            dct[r] -> data_ -> parent -> process = dct[r] -> data_ -> parent -> waiting_list.first() -> data_;
            dct[r] -> data_ -> parent -> waiting_list.dequeue();

            if (dct[r] -> data_ -> parent -> parent -> process == nullptr) {
                dct[r] -> data_ -> parent -> parent -> process = dct[r] -> data_ -> parent -> process;
            } else {
                dct[r] -> data_ ->parent -> parent -> waiting_list.enqueue(dct[r] -> data_ -> parent -> process);
            }
        }
        if (dct[r] -> data_ -> parent -> parent -> waiting_list.check(pro_name)) {
                    int rr = dct[r] -> data_ -> parent -> parent -> waiting_list.find(pro_name);
                    dct[r] -> data_ -> parent -> parent -> waiting_list.remove(dct[r] -> data_ -> parent -> parent -> waiting_list[rr]);
        }
        if(dct[r] -> data_ -> parent -> parent -> process -> get_name() == pro_name) {
            if (dct[r] -> data_ -> parent -> parent -> waiting_list.size() == 0) {
                dct[r] -> data_ -> parent -> parent -> process = nullptr;
            } else {
                dct[r] -> data_ -> parent -> parent -> process = nullptr;
                dct[r] -> data_ -> parent -> parent -> process = dct[r] -> data_ -> parent -> parent -> waiting_list.first() -> data_;
                dct[r] -> data_ -> parent -> parent -> waiting_list.dequeue(); 
            }

            cout << GREEN << "回收" << dc_name << "成功！" << WHITE << endl;            
        } else {
            cout << GREEN << "回收" << dc_name << "成功！" << WHITE << endl;  
        }
    } else {
        cout << GREEN << "回收" << dc_name << "成功！" << WHITE << endl;  
    }

}

void Mgmt::dele_dc() {
    string n;
    cout << "请输入要删除的设备名:" << endl << "> ";
    cin >> n;
    int r = dct.find(n);
    if (dct[r] -> data_ -> process != nullptr) {
        cout << RED << "设备正在被使用，删除失败！" << WHITE << endl;
        return;
    }

    if (dct[r] -> data_ -> process == nullptr) {
        if (dct[r] -> data_ -> parent -> childs.size() == 1) {
            if (dct[r] -> data_ -> parent -> process == nullptr) {
                int k = coct.find(dct[r] -> data_ -> parent -> get_name());
                dct.remove(dct[r]);
                int idx = 0;
                for (auto it : coct[k] -> data_ -> parent -> childs) {
                    idx++;
                    if (it -> get_name() == coct[k] -> data_ -> get_name()) {
                        break;
                    } 
                }
                coct[k] -> data_ -> parent -> childs.erase(coct[k] -> data_ -> parent -> childs.begin() + (idx - 1));
                coct.remove(coct[k]);
                cout << GREEN << "删除设备成功！" << WHITE << endl;
            }
        } else {
            int idx = 0;
            for (auto it :  dct[r] -> data_ -> parent -> childs) {
                idx++;
                if (it -> get_name() == dct[r] -> data_ -> get_name()) {
                    break;
                } 
            }
            dct[r] -> data_ -> parent -> childs.erase(dct[r] -> data_ -> parent -> childs.begin() + (idx - 1));
            dct.remove(dct[r]);
            cout << GREEN << "删除设备成功！" << WHITE << endl;
        }
    }
}

void Mgmt::apply_for_the_same_type() {
    cout << "请输入要申请的设备类型:" << endl << "> ";
    char type;
    cin >> type;
    int rank = 0;
    for(; rank < dct.size(); ++rank) {
        if (dct[rank] -> data_ -> get_type() == type && dct[rank] -> data_ -> process == nullptr) {
            break;
        }
    }
    if (rank == dct.size()) {
        cout << RED << "没有空闲设备！"  << WHITE << endl;
        return;
    }
    apply(dct[rank] -> data_ -> get_name());
}

void Mgmt::terminate() {
    cout << "请输入想要撤销的进程名" << endl << "> ";
    string name;
    cin >> name;
    for (int i =0; i < dct.size(); ++i) {
        if (dct[i] -> data_ -> process != nullptr) {
            if (dct[i] -> data_ -> process -> get_name() == name) {
                release(dct[i] -> data_ -> get_name());
            }
        }
    }
    if (ready.check(name)) {
        int r = ready.find(name);
        ready.remove(ready[r]);
        cout << GREEN << "撤销进程成功！" << WHITE << endl;
    }
}

void Mgmt::show_allocation() {
    cout << endl;
    cout << YELLOW << "|-------------------------------------" << RED << "通 道" << YELLOW << "----------------------------------------|" << WHITE << endl;
    for (int i = 0; i < chct.size(); ++i) {
        ChNode node = *chct[i] -> data_;
        cout << "\t通道名称: " << node.get_name() << endl;
        cout << "\t连接的控制器名称: ";
         for (int j = 0; j < node.childs.size(); ++j) {
            cout << (node.childs[j]) -> get_name() << " ";
        }
        cout << endl;
        cout << "\t占用进程: ";
        node.show_process();
        cout << "\t等待进程: ";
        node.show_waiting();
        cout << YELLOW << "|----------------------------------------------------------------------------------|" << WHITE << endl;
     }
     cout << endl << endl;

     cout << YELLOW << "|-------------------------------------" << RED << "控制器" << YELLOW << "---------------------------------------|" << WHITE << endl;
    for (int i = 0; i < coct.size(); ++i) {
        CoNode node = *coct[i] -> data_;
        cout << "\t控制器名称: " << node.get_name()<< endl;
        cout << "\t连接的设备名称: ";
        for (int j = 0; j < node.childs.size(); ++j) {
            cout << (node.childs[j]) -> get_name() << " ";
        }
        cout << endl;
        cout << "\t所属通道: " << node.parent-> get_name();
        cout << endl;
        cout << "\t占用进程: ";
        node.show_process();
        cout << "\t等待进程: ";
        node.show_waiting();
        cout << YELLOW << "|----------------------------------------------------------------------------------|" << WHITE << endl;
     }
     cout << endl << endl;

    cout << YELLOW << "|-------------------------------------" << RED << "设 备" << YELLOW << "----------------------------------------|" << WHITE << endl;
    for (int i = 0; i < dct.size(); ++i) {
        DcNode node = *dct[i] -> data_;
        cout << "\t设备名称: " << node.get_name() << "("  << GREEN << node.get_type() << WHITE << ")" << endl;
        cout << "\t所属控制器: " << node.parent->get_name();
        cout << endl;
        cout << "\t占用进程: ";
        node.show_process();
        cout << "\t等待进程: ";
        node.show_waiting();
        cout << YELLOW << "|----------------------------------------------------------------------------------|" << WHITE << endl;
     }
     cout << endl << endl;
}

void Mgmt::show_pcb_state() {
    cout << endl;
    cout << YELLOW << "|---------------" << RED << "就绪进程" << YELLOW << "-----------------|" << WHITE << endl;
    if (ready.size() == 0) {
        cout << "\t" << "NONE!" << endl;
    } else {
        for (int i =0 ; i < ready.size(); ++i) {
            cout << "\t" << ready[i] -> data_ -> get_name() << " ";
        }
        cout << endl;
    }
    cout << YELLOW << "|----------------------------------------|" << WHITE << endl;
    cout << endl << endl;
    cout << YELLOW << "|---------------" << RED << "阻塞进程" << YELLOW << "-----------------|" << WHITE << endl;
    if (blocking.size() == 0) {
        cout << "\t" << "NONE!" << endl;
    } else {
        cout << "\t";
        for (int i =0 ; i < blocking.size(); ++i) {
            cout << blocking[i] -> data_ -> get_name() << " ";
        }
        cout << endl;
    }
    cout << YELLOW << "|----------------------------------------|" << WHITE << endl;
    cout << endl;
}

void Mgmt::process_print(Node *node, int blk) {
    cout << "   ";
    if (node -> process == nullptr) {
        cout << "|—<" << "none" << ">" << endl;
    } else if (node -> process != nullptr) {
        cout << "|—<" << node -> process ->get_name() << ">" << endl;
    }
}

void Mgmt::tree_print(Node *node, int blk) {
    for (int i = 0; i < blk; ++i) {
        cout << "   ";
    }
    switch(node -> get_rank()) {
        case 0:cout << "|—<" << node ->get_name() << ">" << endl;break;
        case 1:cout << "|—<" << RED << "通道 " << node ->get_name() << WHITE << ">" << endl;break;
        case 2:cout << "|—<" << GREEN << "控制器 "  << node ->get_name() << WHITE << ">"  << endl;break;
        case 3:cout << "|—<" << CYAN << "设备 " << node ->get_name() << WHITE << ">" << endl;break;
    }
    if (node -> get_rank() != 0) {
        for (int i = 0; i < blk + 1; ++i)
            cout << "   ";
        if (node -> process == nullptr) {
            cout << "|—<" << YELLOW << "占用进程 " << "none" << WHITE ">" << endl;
        } else if (node -> process != nullptr) {
            cout << "|—<" << YELLOW << "占用进程 " << node -> process -> get_name() << WHITE ">" << endl;
        }
        for (int i = 0; i < blk + 1; ++i)
            cout << "   ";
        if (node -> waiting_list.size() == 0) {
            cout << "|—<" << YELLOW << "等待进程 " << "none" << WHITE ">" << endl;
        } else if (node -> waiting_list.size() > 0) {
            cout << "|—<" << YELLOW << "等待进程 ";
            for (int r = 0; r < node -> waiting_list.size(); ++r) {
                cout <<  node -> waiting_list[r] -> data_ -> get_name() << " ";
            }
            cout << WHITE << ">" <<  endl;
        }
    }
    for (int j = 0; j < node -> childs.size(); ++j) {
        tree_print(node -> childs[j], blk + 1);
    }
}

int main(int argc, char *argv[]) {
    Mgmt m;
    m.init();
    m.helper();
    cout << endl;
    char c;
    string dc_name;
    string n;
    while(1) {
        cout << endl << GREEN <<"m0dulo@DEVICE-MGMT-HLJU-OS" << WHITE << ":" << BLUE << "~" << WHITE;
        cout << "$ ";
        cin >> c;
        switch(c) {
            case 'p':m.tree_print(m.cpu,0);break;
            case 't':m.terminate();break;
            case 'd':m.dele_dc();break;
            case 'i':m.apply_for_the_same_type();break;
            case 'r': {
                cout << "请输入要释放的设备名:" << endl << "> ";
                cin >> n;
                m.release(n);
                break;
            }
            case 'a':{
                cout << "请输入所申请的设备名:" << endl << "> ";
                cin >> dc_name;
                m.apply(dc_name);
                break;
            }
            case 'q':exit(0);break;
            case 'c':m.add_dc();break;
            case 's':m.show_allocation();break;
            case 'h':m.helper();cout << endl;break; 
            case 'k':m.show_pcb_state();break;
            default:cout << "命令错误，请重新输入！" << endl;continue;
        }
    }
    return 0;
}