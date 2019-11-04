#include "process_sched.h"
#include <iostream>

using namespace std;
using namespace lyx_utlis;

int main(int argc, char *argv[]) {
    string data;
    ArgumentParser ap;
    ap.new_named_string("load", "dataSample", "named_string", "inputSample", data);
    ap.process(argc, argv);
    auto config = cpptoml::parse_file(data);
    auto key1 = config-> get_qualified_as<double>("first-table.key1");
    cout <<  *key1 << endl;
    return 0;
}