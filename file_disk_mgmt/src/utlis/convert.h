#ifndef SRC_LYX_UTLIS_CONVERT_H_
#define SRC_LYX_UTLIS_CONVERT_H_

#pragma once;

#include <string>
#include <algorithm>

namespace lyx_utlis {
	
struct lower_convert {
   void operator()(char& c) { c = tolower((unsigned char)c); }
};

void tolower(std::string &str) {
    std::for_each(str.begin(), str.end(), lower_convert());
}

struct upper_convert {
   void operator()(char& c) { c = toupper((unsigned char)c); }
};

void toupper(std::string &str) {
    std::for_each(str.begin(), str.end(), upper_convert());
}

}

#endif // SRC_LYX_UTLIS_CONVERT_H_