#include <iostream>
#include "list_header.h"
#include "bitmap.h"

using namespace std;

int main(int argc, char *argv[]) {
    BitMap map;
    map.init(6);
    map.set(5,0);
    map.set(18,0);
    map.set(0,0);
    map.set(39,0);
    map.show();
    cout << map.bit_len() << endl;
    vector<int> idxs = map.bit_idx();
    for (auto idx : idxs) {
        cout << idx << endl;
    }
    map.destory();
    return 0;
}

