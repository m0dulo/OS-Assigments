#ifndef SRC_LYX_UTLIS_CURR_TIME_H_
#define SRC_LYX_UTLIS_CURR_TIME_H_

#pragma once;

#include <time.h>
#include <string>

#define FORMAT_MAX_SIZE 100

namespace lyx_utlis {

std:: string current_time() {
    time_t now = time(0);
    struct tm tstruct;
    char *curr_time = (char *)malloc(sizeof(char) * FORMAT_MAX_SIZE);
    tstruct = *localtime(&now);
    strftime(curr_time, sizeof(char) * FORMAT_MAX_SIZE, "%m-%d %X", &tstruct);
    return curr_time;
}

}

#endif // SRC_LYX_UTLIS_CURR_TIME_H_