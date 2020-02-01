cmake_minimum_required (VERSION 2.8)

add_compile_options (-std=c++17)

set (TEST_RUNNER_PATH "~/Documents/ArtCC/test_runner")
set (PROFILE_PATH "~/Documents/ArtCC/profile")
set (SRCS_DESKTOP src/budget.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)
set (SRCS_MOBILE src/budget_mobile.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)
set (SRCS_LEGACY src/budget_legacy.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)

include_directories(${TEST_RUNNER_PATH} ${PROFILE_PATH})

add_executable(desktop ${SRCS_DESKTOP})
add_executable(mobile ${SRCS_MOBILE})
add_executable(legacy ${SRCS_LEGACY})
