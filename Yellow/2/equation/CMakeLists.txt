cmake_minimum_required (VERSION 2.8)

add_compile_options (-std=c++17)

set (PROJ_FILES_PATH "src")
set (TEST_RUNNER_PATH "/home/dimag1991/Documents/ArtCC/test_runner")
set (SRCS ${PROJ_FILES_PATH}/test_equation.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)

include_directories(${TEST_RUNNER_PATH})

add_executable (a.out ${SRCS})

