#ifndef SRC_LYX_UTLIS_LOG_H_
#define SRC_LYX_UTLIS_LOG_H_

#pragma once;

#include <iostream>

namespace lyx_utlis {

template <class T>
void
_print(T arg) {
    cout << arg << " ";
}

template <class... Args>
void
log(Args... args) {
    int arr[] = {(_print(args), 0)...};
    cout << endl;
}

bool ensure(bool condition, const string &message) {
    if (condition) {
        log("|||测试成功");
    } else {
        log("***测试失败,", message);
    }
}

}

#endif // SRC_LYX_UTLIS_LOG_H_