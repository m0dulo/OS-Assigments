#ifndef SRC_LYX_UTLIS_SPLIT_H_
#define SRC_LYX_UTLIS_SPLIT_H_

#pragma once;

#include <string>
#include <vector>
#include <sstream>

namespace lyx_utlis {

void split(const std::string &s, std::vector<std::string> &tokens, 
            const std::string &delimiter = " ") {
    tokens.clear();
    std::string::size_type last_pos = s.find_first_not_of(delimiter, 0);
    std::string::size_type pos = s.find_first_of(delimiter, last_pos);
    while (std::string::npos != pos || std::string::npos != last_pos) {
        tokens.push_back(s.substr(last_pos, pos - last_pos));
        last_pos = s.find_first_not_of(delimiter, pos);
        pos = s.find_first_of(delimiter, last_pos );
    }
}

// void split(const std::string& s, std::vector<std::string>& tokens,
//            const char delimiter = ' ') {
//     tokens.clear();
//     std::istringstream iss(s);
//     std::string temp;
//     while (std::getline(iss, temp, delimiter)) {
//         tokens.emplace_back(std::move(temp));
//     }
// }

}

#endif // SRC_LYX_UTLIS_SPLIT_H_