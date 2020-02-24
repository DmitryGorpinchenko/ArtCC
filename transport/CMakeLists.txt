cmake_minimum_required (VERSION 2.8)

add_compile_options (-std=c++17 -O2)

set (TEST_RUNNER_PATH "~/Documents/ArtCC/test_runner")
set (PROFILE_PATH "~/Documents/ArtCC/profile")
set (SRCS src/database.cpp src/route_map.cpp src/router.cpp src/renderer.cpp src/geo.cpp src/json.cpp src/svg.cpp src/main.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)

include_directories(${TEST_RUNNER_PATH} ${PROFILE_PATH})

add_executable(a.out ${SRCS})
target_link_libraries (a.out pthread)
