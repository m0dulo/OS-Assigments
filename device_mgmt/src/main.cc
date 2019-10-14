#include "mgmt.h"
#include <iostream>

#include <string>


using namespace std;

void Mgmt::helper() {
    cout<<"c.添加设备------------------命令:c"<<endl;
    cout<<"d.删除设备------------------命令:d"<<endl;	
    cout<<"a.申请设备------------------命令:a"<<endl;
    cout<<"r.归还设备------------------命令:r"<<endl;
    cout<<"i.申请一类设备(设备独立性)--命令:i"<<endl;
    cout<<"s.状态显示------------------命令:s"<<endl;
    cout<<"h.显示帮助------------------命令:h"<<endl;
}

void Mgmt::init() {
    Node *ch = new Node[2];
    for (int i = 0; i < 2; ++i) {
        string idx = to_string(i + 1);
        ch[i].set_rank(1);
        chct.push_back(ch[i]);
        chct[i]->data_.init("CH" + idx);
    }
    delete []ch;

    Node *co = new Node[3];
    for (int i = 0; i < 3; ++i) {
        string idx = to_string(i + 1);
        co[i].set_rank(2);
        coct.push_back(co[i]);
        coct[i]->data_.init("CO" + idx);
    }
    delete []co;

    Node *dc = new Node[4];
    for (int i = 0; i < 4; ++i) {
        dc[i].set_rank(3);
        dct.push_back(dc[i]);
    }
    delete []dc;
    dct[0]->data_.init("KB");
    dct[0]->data_.set_type('I');
    dct[1]->data_.init("MOUSE");
    dct[1]->data_.set_type('I');
    dct[2]->data_.init("PTR");
    dct[2]->data_.set_type('O');
    dct[3]->data_.init("MON");
    dct[3]->data_.set_type('O');

    coct[0] ->data_.set_parent(chct[0]->data_);
    coct[1] ->data_.set_parent(chct[1]->data_);
    coct[2] ->data_.set_parent(chct[1]->data_);

    chct[0]->data_.add_child(coct[0]->data_);
    chct[1]->data_.add_child(coct[1] ->data_);
    chct[1]->data_.add_child(coct[2] ->data_);

    dct[0] ->data_.set_parent(coct[0]->data_);
    dct[1] ->data_.set_parent(coct[0]->data_);
    dct[2] ->data_.set_parent(coct[1]->data_);
    dct[3] ->data_.set_parent(coct[2]-> data_);

    coct[0]->data_.add_child(dct[0] ->data_);
    coct[0]->data_.add_child(dct[1] ->data_);
    coct[1]->data_.add_child(dct[2] ->data_);
    coct[2]->data_.add_child(dct[3] ->data_);
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
        if (dc_name == dct[i] ->data_.node_name) {
            dc_flag = 1;
            break;
        }
    }
    if (dc_flag == 1) {
        cout << "设备已经存在！添加设备失败！" << endl;
        add_dc();
    } else if (dc_flag == 0) {
        Node *dc = new Node;
        dc ->init(dc_name);
        dc -> set_type(dc_type);
        dct.push_back(*dc);
        add_co();
    }

}

void Mgmt::add_co() {
    int flag = 0;
    char cmd;
    cout << "是否添加新的控制器？(Y/N)" << endl <<  "> ";
    cin >> cmd;
    if (cmd == 'Y'){
        cout << "请输入控制器名称：" << endl << "> ";
        string name;
        cin >> name;
        for(int i = 0; i < coct.size(); ++i) {
            if (name == coct[i]->data_.node_name) {
                flag = 1;
                break;
            }
        }
        if (flag == 1) {
            cout << "设备已经存在！添加设备失败！" << endl;
            add_co(); 
        } else if (flag == 0) {
            Node *co = new Node;
            co -> init(name);
            coct.push_back(*co);
            dct.back().set_parent(coct.back());
            coct.back().add_child(dct.back());
            // dct[dct.size() - 1]->data_.parent = &(coct[coct.size() - 1]->data_);
            string ch_name;
            cout << "请输入新设备要挂载的通道名称:" << endl << "> ";
            cin >> ch_name;
            int r = chct.find(ch_name);
            coct.back().set_parent(chct[r]->data_);
            chct[r]->data_.add_child(coct.back());
            //coct[coct.size() - 1]->data_.parent = &(chct[r]->data_);           
        }   
    } else if (cmd == 'N') {
        string n_name;
        cout << "请输入新添加设备要挂载的控制器名称:" << endl << "> ";
        cin >> n_name;
        int r = coct.find(n_name);
        dct.back().set_parent(coct[r]->data_);
        //dct[dct.size() - 1]->data_.parent = &(coct[r]->data_);
    }
}

void Mgmt::show() {
    cout << "|--------------------------------------通道----------------------------------------|" << endl;
    for (int i = 0; i < chct.size(); ++i) {
        Node node = chct[i] -> data_;
        cout << "\t" << "  通道名称:" << node.node_name << " ";
        cout << "连接的控制器名称: ";
         for (int j = 0; j < node.childs.size(); ++j) {
            cout << (node.childs[j]) -> node_name << " ";
        }
        cout << endl;
     }

     cout << "|----------------------------------------------------------------------------------|" << endl;
     cout << endl;

     cout << "|-------------------------------------控制器---------------------------------------|" << endl;
    for (int i = 0; i < coct.size(); ++i) {
        Node node = coct[i] -> data_;
        cout << "\t" << "  控制器名称:" << node.node_name << " ";
        cout << "连接的设备名称:";
        for (int j = 0; j < node.childs.size(); ++j) {
            cout << (node.childs[j]) -> node_name << " ";
        }
        cout << "\t"<<"所属通道:" << node.parent->node_name;
        cout << endl;
     }

     cout << "|----------------------------------------------------------------------------------|" << endl;
     cout << endl;
     cout << "|--------------------------------------设备----------------------------------------|" << endl;
    for (int i = 0; i < dct.size(); ++i) {
        Node node = dct[i] -> data_;
        cout << "\t" << "  设备名称:" << node.node_name << "(" << node.type << ")";
        cout << "\t" << "所属控制器:" << node.parent->node_name;
        cout << endl;
     }

     cout << "|----------------------------------------------------------------------------------|" << endl;
     cout << endl;
     
    

}

int main(int argc, char *argv[]) {
    Mgmt m;
    m.init();
    m.helper();
    cout << endl;
    char c;
    while(1) {
        cout << "请输入命令:" << endl;
        cout << "> ";
        cin >> c;
        switch(c) {
            case 'q':exit(0);break;
            case 'c':m.add_dc();break;
            case 's':m.show();break;
            case 'h':m.helper();cout << endl;break;
            default:cout << "命令错误，请重新输入！" << endl;continue;
        }
    }
    return 0;
}