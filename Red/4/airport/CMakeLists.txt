cmake_minimum_required (VERSION 2.8)

add_compile_options (-std=c++17 -O1)

set (TEST_RUNNER_PATH "~/Documents/ArtCC/test_runner")
set (PROFILE_PATH "~/Documents/ArtCC/profile")
set (SRCS src/airport_counter.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)

include_directories(${TEST_RUNNER_PATH} ${PROFILE_PATH})

add_executable(a.out ${SRCS})

