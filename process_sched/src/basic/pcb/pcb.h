#include <string>

class Pcb {
private:
    std::string name_;
    int arr_time_;
    int bur_time_;
    int start_time_;
    int fini_time_;
    int runned_time_;
    double T_;
    double W_;

public:
    Pcb() {
        name_ = "\0";
        arr_time_ = 0;
        bur_time_ = 0;
        fini_time_ = 0;
        runned_time_ = 0;
        T_ = 0;
        W_ = 0;
    }

    void init(std::string name, int arr, int bur) {
        name_ = name;
        arr_time_ = arr;
        bur_time_ = bur;
    }

    void set_start(int s) { start_time_ = s;}
    void set_fini(int f) { fini_time_ = f;}
    void set_runned(int r) { runned_time_ = r;}
    void set_T(double t) { T_ = t;}
    void set_W(double w) { W_ = w;}
    void run() { runned_time_++; }

    
    std::string get_name() { return name_;}
    int get_arr() {return arr_time_;}
    int get_bur() { return bur_time_;}
    int get_fini() { return fini_time_;}
    int get_start() {return start_time_;}
    int get_runned() { return runned_time_;}
    double get_T() {return T_;}
    double get_W() { return W_;}

    bool is_finished() {
        if (runned_time_ ==  bur_time_)
            return true;
        else
            return false;
    }
};