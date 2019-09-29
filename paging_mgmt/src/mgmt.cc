#include <iostream>
#include "list_header.h"
#include "bitmap.h"
#include "queue.h"

using namespace std;

int main(int argc, char *argv[]) {
    Queue<int> q;
    q.push_back(3);
    q.push_back(5);
    for (int i = 0; i < q.size(); i++) {
        cout << q[i] ->data_ << endl;
    }
    cout << q.size() << endl;
    q.dequeue();
    for (int i = 0; i < q.size(); i++) {
        cout << q[i] ->data_ << endl;
    }
    cout << q.size() << endl;
    cout << q.front() << endl;
    q.dequeue();
    cout << q.size() << endl;
    cout << q.front() << endl;

    // BitMap map;
    // map.init(5);
    // map.show();
    // map.set(5,0);
    // map.set(18,0);
    // map.set(0,0);
    // map.set(39,0);
    // //map.show();
    // if (map.check(31) == 0) {
    //     cout << "Y!" << endl;
    // }
    // cout << map.bit_len() << endl;
    // vector<int> idxs = map.bit_idx();
    // for (auto idx : idxs) {
    //     cout << idx << endl;
    // }

    return 0;
}

