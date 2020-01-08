cmake_minimum_required (VERSION 2.8)

add_compile_options (-std=c++17)

set (SRCS src/main.cpp src/query.cpp src/responses.cpp src/bus_manager.cpp)

add_executable(a.out ${SRCS})

