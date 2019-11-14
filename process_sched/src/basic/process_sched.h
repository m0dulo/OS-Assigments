#ifndef SRC_SCHED_H_
#define SRC_SCHED_H_

#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

struct PCB
{
    char name[10];
    int size;              //申请内存大小
    int arrival_time;      //到达时间
    int burst_time;        //服务时间
    int start_time;        //开始时间
    int finished_time;    //完成时间
    int runned_time;        //周转时间
    double runned_time_value; //带权周转时间
    int priority;         //优先级

    int has_time;   ///时间片轮转已经执行的时间；
    bool tail;   ///时间片轮转的已经开始执行队列中的最后一个进程；

    vector<int> Max;             //最大需求矩阵
    vector<int> Work;            //剩余资源;
    vector<int> Allocation;      //已分配矩阵
    vector<int> Need;            //需求矩阵
    vector<int> WandA;           //Work + Allocation

    bool operator <(const struct PCB& b){return burst_time < b.burst_time;}
};

bool cmp1 (const PCB& a, const PCB& b){ return a.arrival_time < b.arrival_time; }

class Process
{
public :
    double ave_time;
    double ave_value;

    vector<PCB> process;
    vector<PCB> ordered_process;
    list<PCB> ready, running, blocked, finished, ready_runned;
    vector<PCB> vec;
    vector<int> Work,Worksum;
    list<PCB> first,second,third;///三级反馈队列;

    ///进程状态转换 链表操作
    void init_p();
    void time_p();
    void block_p();
    void wake_p();
    void finished_p();
    void dispatch_show(string str, int t);
    void finished_show();

    ///进程调度算法;
    void process_show();
    void Creat_PCB(fstream& in);

    void process_FCFS();

    void process_SJF();

    void Time_turn(int T);
    void Timeout(list<PCB>& A, list<PCB>& B, const int& T, int& Tt,int tt);

    void process_priority_static_notgrap();
    void process_priority_static_grap();
    void process_priority_dynamic_notgrap();
    void process_priority_dynamic_grap();

    void multilevel_feedback_FCFS();
    void multilevel_feedback_SJF();
    void multilevel_feedback_show(int tt);

    void dfs(int path, PCB p);
    void queue_show(int num);
    void Banker_algorithm();
};

void Process::Creat_PCB(fstream& in)
{
    PCB p;
    int n,x;
    //cout<<"Please input the name,size,arrival time and brust time:"<<endl;
    in >> p.name >> p.size >> p.arrival_time >> p.burst_time;
    //cout<<"Please input the size of resource:";
    in >> n;
    //cout<<"Please input the Max resource one by one."<<endl;
    for(int i=0; i<n; i++)in >> x, p.Max.emplace_back(x);
    //cout<<"Please input the Allocation one by one."<<endl;
    for(int i=0; i<n; i++)in >> x, p.Allocation.emplace_back(x);
    for(int i=0; i<n; i++){
        p.Need.emplace_back(p.Max[i] - p.Allocation[i]);
    }
    p.has_time = 0;
    p.tail = 0;
    p.priority = 0;
    p.start_time = p.finished_time = p.runned_time = p.runned_time_value = -1;
    process.emplace_back(p);
}

void Process::process_show()
{
    cout<<"  name  arrival time\tburst time\tstart time\tfinished time\trunned time\trunned time with value"<<endl;
    for (int i = 0; i < process.size(); i++){
        printf("%6s\t%8d\t%8d\t%8d\t%8d\t%8d\t%8.2f\n", process[i].name, process[i].arrival_time, process[i].burst_time,
        process[i].start_time, process[i].finished_time,process[i].runned_time, process[i].runned_time_value);
    }
    printf("average runned time：%.6f\naverage runned time with value:%.6f\n", ave_time, ave_value);
}
void Process::finished_show()
{
    cout<<"  name  arrival time\tburst time\tstart time\tfinished time\trunned time\trunned time with value"<<endl;
    for (auto it:finished){
        printf("%6s\t%8d\t%8d\t%8d\t%8d\t%8d\t%8.2f\n", it.name, it.arrival_time, it.burst_time,
        it.start_time, it.finished_time,it.runned_time, it.runned_time_value);
    }
    printf("average runned time：%.6f\naverage runned time with value:%.6f\n", ave_time, ave_value);
}
void Process::init_p()
{
    ready_runned.clear();
    ready.clear();
    running.clear();
    blocked.clear();
    finished.clear();
    ordered_process.clear();
    ordered_process.assign(process.begin(),process.end());
}
void Process::time_p()
{
    PCB tmp = running.front();
    ready.push_back(tmp);
    running.pop_front();
}
void Process::block_p()
{
    PCB tmp = running.front();
    blocked.push_back(tmp);
    running.pop_front();
}
void Process::wake_p()
{
    PCB tmp = blocked.front();
    ready.push_back(tmp);
    blocked.pop_front();
}
void Process::finished_p()
{
    PCB tmp = running.front();
    finished.push_back(tmp);
    running.pop_front();
}

void Process::dispatch_show(string str, int t)
{
    system("clear");
    cout<<"\n\n\n\t\t"<<str<<endl;
    cout<<"\t\tready: "; for(auto it:ready_runned) cout<<it.name<<"("<<it.has_time<<";"<<it.priority<<") ";
    cout<<"/ ";
    for(auto it:ready) cout<<it.name<<"("<<it.has_time<<";"<<it.priority<<") "; cout<<endl;
    cout<<"\t\trunning: "; for(auto it:running) cout<<it.name<<"("<<it.has_time<<";"<<it.priority<<") "; cout<<endl;
    cout<<"\t\tblocked: "; for(auto it:blocked) cout<<it.name<<"("<<it.has_time<<";"<<it.priority<<") "; cout<<endl;
    cout<<"\t\tfinished: "; for(auto it:finished) cout<<it.name<<"("<<it.has_time<<";"<<it.priority<<") "; cout<<endl;
    cout<<"\n\t\t******time="<<t<<"******"<<endl;
}

void Process::process_FCFS()
{
    getchar();
    init_p();
    sort(ordered_process.begin(), ordered_process.end(), cmp1);
    for(auto it:ordered_process){
        ready.push_back(it);
    }
    double a = 0, b = 0;
    for (int tt=0; ;tt++){
        if(running.size() == 1){
            PCB tmp = running.front();
            if(tmp.has_time == tmp.burst_time){
                tmp.finished_time = tt;
                tmp.runned_time = tt - tmp.arrival_time;
                tmp.runned_time_value = tmp.runned_time*1.0 / tmp.burst_time;
                a += tmp.runned_time;
                b += tmp.runned_time_value;
                finished.push_back(tmp);
                running.pop_back();
            }
        }
        ///执行一个进程;
        PCB u;
        if(ready.size() && running.empty()) {
            u = ready.front();
            if(u.arrival_time <= tt){
                u.start_time = tt;
                //u.has_time++;
                running.push_back(u);
                ready.pop_front();
            }
        }
        if(running.size() == 1) {
            PCB& tmp = running.front();
            tmp.has_time++;
        }

        dispatch_show(">>FCFS<<", tt);
        usleep(1000);
        if(ready.empty() && running.empty()) break;
    }
    ave_time = a / process.size();
    ave_value = b / process.size();
    finished.sort([](const PCB& a, const PCB& b){
        return a.arrival_time < b.arrival_time;
    });
    finished_show();
}

void Process::process_SJF()
{
    getchar();
    init_p();
    sort(ordered_process.begin(), ordered_process.end(), cmp1);
    for(auto it:ordered_process){
        ready.push_back(it);
    }
    double a = 0, b = 0;
    for (int tt=0; ;tt++){
        ///选择可执行的文件;
        if(ready.size()){
            for(list<PCB>::iterator it = ready.begin();it!=ready.end();){
                PCB re = *it;
                if(re.arrival_time > tt) break;
                ready_runned.push_back(re);
                it++;
                ready.pop_front();
            }
            ready_runned.sort();
        }

        ///释放完成的进程;
        if(running.size() == 1){
            PCB tmp = running.front();
            if(tmp.has_time == tmp.burst_time){
                tmp.finished_time = tt;
                tmp.runned_time = tt - tmp.arrival_time;
                tmp.runned_time_value = tmp.runned_time*1.0 / tmp.burst_time;
                a += tmp.runned_time;
                b += tmp.runned_time_value;
                finished.push_back(tmp);
                running.pop_back();
            }
        }
        ///执行一个进程;
        if(running.empty() && ready_runned.size()){
            PCB run = ready_runned.front();
            run.start_time = tt;
            running.push_back(run);
            ready_runned.pop_front();
        }
        if(running.size() == 1) {
            PCB& tmp = running.front();
            tmp.has_time++;
            //cout<<"has_time: "<<running.front().has_time<<endl;
        }

        dispatch_show(">>SJF<<", tt);
        usleep(1000);
        if(ready.empty() && running.empty()) break;
    }
    ave_time = a / process.size();
    ave_value = b / process.size();
    finished.sort([](const PCB& a, const PCB& b){
        return a.arrival_time < b.arrival_time;
    });
    finished_show();
}

void Process::process_priority_static_notgrap()
{
    cout<<"Please input the priority of the process:"<<endl;
    for(auto &it:process){
        cout<<it.name<<":";
        cin>>it.priority;
    }

    getchar();
    init_p();
    sort(ordered_process.begin(), ordered_process.end(), cmp1);
    for(auto it:ordered_process){
        ready.push_back(it);
    }
    double a = 0, b = 0;
    for (int tt=0; ;tt++){
        ///选择可执行的文件;
        if(ready.size()){
            for(list<PCB>::iterator it = ready.begin();it!=ready.end();){
                PCB re = *it;
                if(re.arrival_time > tt) break;
                ready_runned.push_back(re);
                it++;
                ready.pop_front();
            }
            ready_runned.sort([](const PCB& a, const PCB& b){
                return a.priority < b.priority;
            });
        }

        ///释放完成的进程;
        if(running.size() == 1){
            PCB tmp = running.front();
            if(tmp.has_time == tmp.burst_time){
                tmp.finished_time = tt;
                tmp.runned_time = tt - tmp.arrival_time;
                tmp.runned_time_value = tmp.runned_time*1.0 / tmp.burst_time;
                a += tmp.runned_time;
                b += tmp.runned_time_value;
                finished.push_back(tmp);
                running.pop_back();
            }
        }
        ///执行一个进程;
        if(running.empty() && ready_runned.size()){
            PCB run = ready_runned.front();
            run.start_time = tt;
            running.push_back(run);
            ready_runned.pop_front();
        }
        if(running.size() == 1) {
            PCB& tmp = running.front();
            tmp.has_time++;
        }

        dispatch_show(">>priority_static_notgrap<<", tt);
        usleep(1000);
        if(ready.empty() && running.empty()) break;
    }
    ave_time = a / process.size();
    ave_value = b / process.size();
    finished.sort([](const PCB& a, const PCB& b){
        return a.arrival_time < b.arrival_time;
    });
    cout<<"priority"<<endl;
    for(auto it:process){
        cout<<it.name<<":"<<it.priority<<endl;
    }
    finished_show();
}
void Process::Time_turn(int T)
{
    getchar();
    init_p();
    sort(ordered_process.begin(), ordered_process.end(), cmp1);
    for(auto it:ordered_process){
        ready.push_back(it);
    }
    int Tt;

    double a = 0, b = 0;
    for (int tt=0; ;tt++){
        if(running.size() == 1){
            PCB& run = running.front();
            ///拿走完成进程、时间片到进程;
            if(run.has_time == run.burst_time){
                run.finished_time = tt;
                run.runned_time = tt - run.arrival_time;
                run.runned_time_value = run.runned_time*1.0 / run.burst_time;
                a += run.runned_time;
                b += run.runned_time_value;
                finished.push_back(run);
                running.pop_front();
            }
            else if(Tt == T){
                ready_runned.push_back(run);
                running.pop_front();
            }
            ///当一个队列周期结束 并且时间片到了 或 进程完成，考虑新的进程进入时间片执行队列
            if(running.empty() && run.tail && ready.size() && ready.front().arrival_time <= tt){
                //int burst = 0x3f3f3f3f;
                PCB& rr = ready.front();
                /*
                for(auto it:ready){
                    if(it.arrival_time <= tt){
                        if(burst > it.burst_time){
                            burst = it.burst_time;
                            rr = it;
                        }
                    }
                    else break;
                }
                */
                if(rr.arrival_time <= tt){
                    rr.start_time = tt;
                    rr.tail = 1;
                    run.tail = 0;
                    running.push_front(rr);
                    Tt = 0;
                    ready.pop_front();
                }
            }
            else if(running.empty() && ready_runned.size()) {
                running.push_front(ready_runned.front());
                ready_runned.pop_front();
                Tt=0;
            }
        }
        else{
            ///初始时间片执行队列处理
            if(ready.size() && ready.front().arrival_time <= tt){
                PCB& tp = ready.front();
                tp.start_time = tt;
                tp.tail = 1;
                Tt = 0;
                ready.pop_front();
                running.push_back(tp);
            }
        }
        ///执行
        if(running.size() == 1){
            running.front().has_time++;
            Tt++;
        }

        dispatch_show(">>RR<<", tt);
        usleep(1000);
        if(ready.empty() && ready_runned.empty() && running.empty()) break;
    }
    ave_time = a / process.size();
    ave_value = b / process.size();
    finished.sort([](const PCB& a, const PCB& b){
        return a.arrival_time < b.arrival_time;
    });
    finished_show();
}
void Process::Timeout(list<PCB>& A, list<PCB>& B,const int& T, int& Tt, int tt)
{
    if(running.size() == 1){
        PCB& run = running.front();
        ///拿走完成进程、时间片到的进程;
        if(run.has_time == run.burst_time){
            run.finished_time = tt;
            run.runned_time = tt - run.arrival_time;
            run.runned_time_value = run.runned_time*1.0 / run.burst_time;
            ave_time += run.runned_time;
            ave_value += run.runned_time_value;
            finished.push_back(run);
            running.pop_front();
        }
        else if(Tt == T){
            B.push_back(run);
            running.pop_front();
        }
        ///当一个队列周期结束 并且时间片到了 或 进程完成，考虑新的进程进入时间片执行队列
        if(running.empty() && A.size() && A.front().arrival_time <= tt){
            PCB& rr = A.front();
            rr.start_time = tt;
            running.push_front(rr);
            Tt = 0;
            A.pop_front();
        }
        else if(running.empty() && B.size() && B.front().arrival_time <= tt){
            PCB& rr = B.front();
            rr.start_time = tt;
            running.push_front(rr);
            Tt = 0;
            B.pop_front();
        }
    }
    else{
        ///初始时间片执行队列处理
        if(A.size() && A.front().arrival_time <= tt){
            PCB& tp = A.front();
            tp.start_time = tt;
            Tt = 0;
            A.pop_front();
            running.push_back(tp);
        }
    }
    ///执行
    if(running.size() == 1){
        running.front().has_time++;
        Tt++;
    }
}

void Process::process_priority_static_grap()
{
    cout<<"Please input the priority of the process:"<<endl;
    for(auto &it:process){
        cout<<it.name<<":";
        cin>>it.priority;
    }

    getchar();
    init_p();
    sort(ordered_process.begin(), ordered_process.end(), cmp1);
    for(auto it:ordered_process){
        ready.push_back(it);
    }
    double a = 0, b = 0;
    for (int tt=0; ;tt++){
        cout<<"time="<<tt<<endl;
        ///选择可执行的文件;
        if(ready.size()){
            for(list<PCB>::iterator it = ready.begin();it!=ready.end();){
                PCB re = *it;
                if(re.arrival_time > tt) break;
                ready_runned.push_back(re);
                it++;
                ready.pop_front();
            }
            ready_runned.sort([](const PCB& a, const PCB& b){
                return a.priority < b.priority;
            });
        }

        ///释放完成的进程; 或抢占进程;
        if(running.size() == 1){
            PCB tmp = running.front();
            if(tmp.has_time == tmp.burst_time){
                tmp.finished_time = tt;
                tmp.runned_time = tt - tmp.arrival_time;
                tmp.runned_time_value = tmp.runned_time*1.0 / tmp.burst_time;
                a += tmp.runned_time;
                b += tmp.runned_time_value;
                finished.push_back(tmp);
                running.pop_back();
            }
            else{
                ready_runned.push_front(tmp);
                running.pop_front();
            }
        }
        ///执行一个进程;
        if(running.empty() && ready_runned.size()){
            ready_runned.sort([](const PCB& a, const PCB& b){
                return a.priority < b.priority;
            });
            PCB& run = ready_runned.front();
            run.start_time = tt;
            running.push_back(run);
            ready_runned.pop_front();
        }
        if(running.size() == 1) {
            PCB& tmp = running.front();
            tmp.has_time++;
        }

        dispatch_show(">>priority_static_grap<<", tt);
        usleep(1000);
        if(ready.empty() && running.empty()) break;
    }
    ave_time = a / process.size();
    ave_value = b / process.size();
    finished.sort([](const PCB& a, const PCB& b){
        return a.arrival_time < b.arrival_time;
    });
    cout<<"priority"<<endl;
    for(auto it:process){
        cout<<it.name<<":"<<it.priority<<endl;
    }
    finished_show();
}
void Process::process_priority_dynamic_notgrap()
{
    cout<<"Please input the priority of the process:"<<endl;
    for(auto &it:process){
        cout<<it.name<<":";
        cin>>it.priority;
    }

    getchar();
    init_p();
    sort(ordered_process.begin(), ordered_process.end(), cmp1);
    for(auto it:ordered_process){
        ready.push_back(it);
    }
    double a = 0, b = 0;
    for (int tt=0; ;tt++){
        ///选择可执行的文件;
        if(ready.size()){
            for(list<PCB>::iterator it = ready.begin();it!=ready.end();){
                PCB re = *it;
                if(re.arrival_time > tt) break;
                ready_runned.push_back(re);
                it++;
                ready.pop_front();
            }
            ready_runned.sort([](const PCB& a, const PCB& b){
                return a.priority < b.priority;
            });
        }

        ///释放完成的进程;
        if(running.size() == 1){
            PCB tmp = running.front();
            if(tmp.has_time == tmp.burst_time){
                tmp.finished_time = tt;
                tmp.runned_time = tt - tmp.arrival_time;
                tmp.runned_time_value = tmp.runned_time*1.0 / tmp.burst_time;
                a += tmp.runned_time;
                b += tmp.runned_time_value;
                finished.push_back(tmp);
                running.pop_back();
            }
        }
        ///执行一个进程;
        if(running.empty() && ready_runned.size()){
            PCB run = ready_runned.front();
            run.start_time = tt;
            running.push_back(run);
            ready_runned.pop_front();
        }
        if(running.size() == 1) {
            PCB& tmp = running.front();
            tmp.priority++;
            tmp.has_time++;
        }

        dispatch_show(">>priority_dynamic_notgrap<<", tt);
        usleep(1000);
        if(ready.empty() && running.empty()) break;
    }
    ave_time = a / process.size();
    ave_value = b / process.size();
    finished.sort([](const PCB& a, const PCB& b){
        return a.arrival_time < b.arrival_time;
    });
    cout<<"priority"<<endl;
    for(auto it:process){
        cout<<it.name<<":"<<it.priority<<endl;
    }
    finished_show();
}
void Process::process_priority_dynamic_grap()
{
    cout<<"Please input the priority of the process:"<<endl;
    for(auto &it:process){
        cout<<it.name<<":";
        cin>>it.priority;
    }

    getchar();
    init_p();
    sort(ordered_process.begin(), ordered_process.end(), cmp1);
    for(auto it:ordered_process){
        ready.push_back(it);
    }
    double a = 0, b = 0;
    for (int tt=0; ;tt++){
        cout<<"time="<<tt<<endl;
        ///选择可执行的文件;
        if(ready.size()){
            for(list<PCB>::iterator it = ready.begin();it!=ready.end();){
                PCB re = *it;
                if(re.arrival_time > tt) break;
                ready_runned.push_back(re);
                it++;
                ready.pop_front();
            }
            ready_runned.sort([](const PCB& a, const PCB& b){
                return a.priority < b.priority;
            });
        }

        ///释放完成的进程; 或抢占进程;
        if(running.size() == 1){
            PCB tmp = running.front();
            if(tmp.has_time == tmp.burst_time){
                tmp.finished_time = tt;
                tmp.runned_time = tt - tmp.arrival_time;
                tmp.runned_time_value = tmp.runned_time*1.0 / tmp.burst_time;
                a += tmp.runned_time;
                b += tmp.runned_time_value;
                finished.push_back(tmp);
                running.pop_back();
            }
            else{
                ready_runned.push_front(tmp);
                running.pop_front();
            }
        }
        ///执行一个进程;
        if(running.empty() && ready_runned.size()){
            ready_runned.sort([](const PCB& a, const PCB& b){
                return a.priority < b.priority;
            });
            PCB& run = ready_runned.front();
            run.start_time = tt;
            running.push_back(run);
            ready_runned.pop_front();
        }
        if(running.size() == 1) {
            PCB& tmp = running.front();
            tmp.priority++;
            tmp.has_time++;
        }

        dispatch_show(">>priority_dynamic_grap<<", tt);
        usleep(1000);
        if(ready.empty() && running.empty()) break;
    }
    ave_time = a / process.size();
    ave_value = b / process.size();
    finished.sort([](const PCB& a, const PCB& b){
        return a.arrival_time < b.arrival_time;
    });
    cout<<"priority"<<endl;
    for(auto it:process){
        cout<<it.name<<":"<<it.priority<<endl;
    }
    finished_show();
}

void Process::multilevel_feedback_FCFS()
{
    getchar();
    init_p();
    sort(ordered_process.begin(), ordered_process.end(), cmp1);
    for(auto it:ordered_process){
        first.push_back(it);
    }
    int Tt;
    const int T1 = 1<<0, T2 = 1<<1, T3 = 1<<2;
    ave_time = 0;
    ave_value = 0;
    int flag = 1;
    for(int tt=0; ;tt++){
        if(first.size()){
            Timeout(first, second, T1, Tt, tt);
            flag = 1;
        }
        else if(second.size()){
            if(flag == 1) Timeout(first, second, T1, Tt, tt);
            else Timeout(second, third, T2, Tt, tt);
            flag = 2;
        }
        else if(third.size()){
            if(flag == 1)  Timeout(first, second, T1, Tt, tt);
            else if(flag == 2)  Timeout(second, third, T2, Tt, tt);
            else Timeout(third, third, T3, Tt, tt);
            flag = 3;
        }
        else{
            if(flag == 1)  Timeout(first, second, T1, Tt, tt);
            else if(flag == 2)  Timeout(second, third, T2, Tt, tt);
            else Timeout(third, third, T3, Tt, tt);
        }
        multilevel_feedback_show(tt);
        usleep(1000);
        if(first.empty() && second.empty() && second.empty() && running.empty()) break;
    }
    ave_time = ave_time / process.size();
    ave_value = ave_value / process.size();
    finished.sort([](const PCB& a, const PCB& b){
        return a.arrival_time < b.arrival_time;
    });
    finished_show();
}

void Process::multilevel_feedback_SJF()
{

}

void Process::multilevel_feedback_show(int tt)
{
    system("clear");
    cout<<"\n\n\n\t\t>>multilevel_feedback<<"<<endl;
    cout<<"\t\tready:"<<endl;
    cout<<"\t\t\tfirst->"; for(auto it:first) cout<<it.name<<"("<<it.has_time<<") "; cout<<endl;
    cout<<"\t\t\tsecond->"; for(auto it:second) cout<<it.name<<"("<<it.has_time<<") "; cout<<endl;
    cout<<"\t\t\tthird:->"; for(auto it:third) cout<<it.name<<"("<<it.has_time<<") "; cout<<endl;
    cout<<"\t\trunning:"; for(auto it:running) cout<<it.name<<"("<<it.has_time<<") "; cout<<endl;
    cout<<"\t\tblocked:"; for(auto it:blocked) cout<<it.name<<"("<<it.has_time<<") "; cout<<endl;
    cout<<"\t\tfinished:"; for(auto it:finished) cout<<it.name<<"("<<it.has_time<<") "; cout<<endl<<endl;
    cout<<"\n\n\t\t******time="<<tt<<"******"<<endl;
}
int Banker_algorithm_num;
void Process::dfs(int path, PCB p)
{
    //cout<<"path="<<path<<endl;
    vec[path] = p;
    if(path == process.size()-1) { queue_show(++Banker_algorithm_num); return; }
    bool vis[process.size()];
    memset(vis,0,sizeof vis);
    for(int i=0;i<process.size();i++){
        //cout<<process[i].name<<endl;
        if(process[i].tail == 0){
            bool flag = 1;
            for(int j=0;j<process[i].Need.size();j++){
                if(process[i].Need[j] > Worksum[j] ) {flag = 0; break;}
            }
            if(flag){
                process[i].tail = 1;
                for(int j=0;j<Work.size();j++){
                    process[i].Work.push_back(Worksum[j]);
                    process[i].WandA.push_back(Worksum[j] + process[i].Allocation[j]);
                    Worksum[j] = process[i].WandA[j];
                }
                dfs(path+1,process[i]);

                process[i].tail = 0;
                for(int j=0;j<Work.size();j++){
                    Worksum[j] -= process[i].Allocation[j];
                    process[i].Work.pop_back();
                    process[i].WandA.pop_back();
                }
            }
        }
    }
}
void Process::Banker_algorithm()
{
    Banker_algorithm_num = 0;
    for(int i=0;i<Work.size();i++) Worksum.push_back(0);
    vec.assign(process.begin(),process.end());
    for(int i=0;i<process.size();i++) {
        process[i].tail = 0;
    }
    int path = 0;

    for(int i=0;i<process.size();i++){
        if(process[i].tail == 0){
            bool flag = 1;
            for(int j=0;j<process[i].Need.size();j++){
                if(process[i].Need[j] > Work[j] ) {flag = 0; break;}
            }
            if(flag){
                //cout<<process[i].name<<endl;
                process[i].tail = 1;
                for(int j=0;j<Work.size();j++){
                    process[i].Work.push_back(Work[j]);
                    process[i].WandA.push_back( Work[j] + process[i].Allocation[j] );
                    Worksum[j] = process[i].WandA[j];
                }
                //cout<<"inter dfs"<<endl;
                dfs(path,process[i]);
                process[i].tail = 0;
                for(int j=0;j<Work.size();j++){
                    process[i].Work.pop_back();
                    process[i].WandA.pop_back();
                    Worksum[j] = 0;
                }
            }
        }
    }
    for(int i=0;i<process.size();i++) process[i].tail = 0;
    if(!Banker_algorithm_num) cout<<"There is no safe queue."<<endl;
}
void Process::queue_show(int num)
{
    cout<<"\n\n\n\t  >>Banker_algorithm<<"<<endl;
    cout<<"\t  ******number "<<num<<"******"<<endl;
    cout << "\nname\tWork\t  Allocation\t  Need\t  W+A\t" << endl;
    for(int i=0;i<vec.size();i++){
        printf("%4s /",vec[i].name);
        for(int j=0;j<vec[i].Work.size(); j++) printf("%2d ",vec[i].Work[j]); cout<<"/";
        for(int j=0;j<vec[i].Allocation.size(); j++) printf("%2d ",vec[i].Allocation[j]); cout<<"/";
        for(int j=0;j<vec[i].Need.size(); j++) printf("%2d ",vec[i].Need[j]); cout<<"/";
        for(int j=0;j<vec[i].WandA.size(); j++) printf("%2d ",vec[i].WandA[j]);
        cout<<endl<<endl;
    }
}

#endif // SRC_SCHED_H_
