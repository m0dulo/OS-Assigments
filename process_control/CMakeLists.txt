cmake_minimum_required(VERSION 3.8)
project("process")
find_package(Threads)
# set(CMAKE_BUILD_TYPE Debug)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -w -msse3 -funroll-loops  -std=c++11 -O0 -pg" )
else()
    SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIC -funroll-loops -w -std=c++11 -Ofast -march=native" )
endif()
include_directories(src)
AUX_SOURCE_DIRECTORY(src SRCS)
add_executable (process ${SRCS})
target_link_libraries(process libglog.a ${CMAKE_THREAD_LIBS_INIT})
include_directories(src/basic/list)
include_directories(src/basic/pcb)
include_directories(src/basic/memory)
include_directories(src/third_party/Argument_helper)
