cmake_minimum_required (VERSION 2.8)

add_compile_options (-std=c++17)

set (TEST_RUNNER_PATH "~/Documents/ArtCC/test_runner")
set (SRCS src/database.cpp src/node.cpp src/date.cpp src/token.cpp src/condition_parser.cpp src/condition_parser_test.cpp src/main.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)

include_directories(${TEST_RUNNER_PATH})

add_executable(a.out ${SRCS})

