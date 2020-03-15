cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

set (TEST_RUNNER_PATH "~/Documents/ArtCC/test_runner")
set (SRCS src/database.cpp src/route_map.cpp src/router.cpp src/renderer.cpp src/geo.cpp src/json.cpp src/svg.cpp src/main.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${TEST_RUNNER_PATH})

protobuf_generate_cpp(
    PROTO_SRCS
    PROTO_HDRS
    src/transport_catalog.proto
)

add_executable(a.out ${SRCS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(a.out ${Protobuf_LIBRARIES})

