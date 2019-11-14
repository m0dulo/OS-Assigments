#include "process_sched.h"
#include <bits/stdc++.h>

using namespace std;

void menu()
{
    cout<<"\n\n\n\t\t\t>>>operation system5.0<<<\n\n";
    cout<<"\t\t\t1.refresh data\n\n";
    cout<<"\t\t\t***scheduling algorithm***\n";
    cout<<"\t\t\t2.FCFS algorithm\n";
    cout<<"\t\t\t3.SJF algorithm\n";
    cout<<"\t\t\t4.Times out\n";
    cout<<"\t\t\t5.priority\n";
    cout<<"\t\t\t6.multilevel feedback\n\n";
    cout<<"\t\t\t***Avoid deadlock***\n";
    cout<<"\t\t\t7.Banker algorithm\n\n";
    cout<<"\t\t\t0.exit\n"<<endl;
    cout<<"\n\n>>";
}

void solve(Process& p)
{
    p.process.clear();
    //p.ordered_process.clear();
    /*  F:\\OS\\os5\\  */
    string filename, file = "" ;///测试样例所在文件夹的绝对路径;
    cout<<"Please input the input filename:";
    cin>>filename;
    filename = file + filename;
    cout<<filename<<endl;
    fstream in(filename);
    int num;
    //cout<<"Please input the number of data:";
    in>>num;
    for(int i=0;i<num;i++) p.Creat_PCB(in);
    //cout<<"Please input the work source number:";
    int n,x; in>>n;
    for(int i=0;i<n;i++) in>>x, p.Work.push_back(x);
    p.ave_time = p.ave_value = 0;
}

int main() {
    Process process;
    int choice;
    while(1)
    {
       menu();
       cin>>choice;
       switch(choice)
       {
           case 1:
               solve(process);
               cout<<"Initial data is as follow:"<<endl;
               process.process_show();
               break;
           case 2:
               system("clear");
               process.process_FCFS();
               cout<<"Press any key to continue......"<<endl;
               getchar();
               break;
           case 3:
               system("clear");
               process.process_SJF();
               cout<<"Press any key to continue......"<<endl;
               getchar();
               break;
           case 4:
               system("clear");
               int T;
               cout<<"Please input the times:";
               cin>>T;
               cout<<"Press any key to continue......"<<endl;
               getchar();
               process.Time_turn(T);
               cout<<"Press any key to continue......"<<endl;
               getchar();
               break;
           case 5:
               system("clear");
               cout<<"\n\n\n\t\t\t    >>>priority<<<\n\n";
               cout<<"\t\t\t1.static and not grap\n";
               cout<<"\t\t\t2.static and grap\n";
               cout<<"\t\t\t3.dynamic and not grap\n";
               cout<<"\t\t\t4.dynamic and grap\n";
               cout<<"\t\t\t0.back\n";
               int n;cin>>n;
               if(n==1) process.process_priority_static_notgrap();
               else if(n==2) process.process_priority_static_grap();
               else if(n==3) process.process_priority_dynamic_notgrap();
               else if(n==4) process.process_priority_dynamic_grap();
               else if(n==0) {}
               else cout<<"defult!"<<endl;
               cout<<"Press any key to continue......"<<endl;
               getchar();
               break;
           case 6:
               system("clear");
               process.multilevel_feedback_FCFS();
               cout<<"Press any key to continue......"<<endl;
               getchar();
               break;
           case 7:
               //system("clear");
               process.Banker_algorithm();
               cout<<"Press any key to continue......"<<endl;
               getchar();
               break;
           case 0:
               exit(0);
           default:
               cout<<"Default!\n";
       }
    }
    return 0;
}
/*
1.txt
mf.txt
pri.txt
*/





