#ifndef SRC_LYX_UTLIS_EXISTS_H_
#define SRC_LYX_UTLIS_EXISTS_H_

#include <string>
#include <sys/stat.h>

namespace lyx_utlis {

bool exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

}

#endif // SRC_LYX_UTLIS_EXISTS_H_