cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

set(TEST_RUNNER_PATH "~/Documents/ArtCC/test_runner")

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${TEST_RUNNER_PATH})

protobuf_generate_cpp(
    PROTO_SRCS
    PROTO_HDRS
    protos/address.proto
    protos/company.proto
    protos/name.proto
    protos/phone.proto
    protos/provider.proto
    protos/signal.proto
    protos/url.proto
    protos/working_time.proto
)

add_executable(yellow_pages ${PROTO_SRCS} ${PROTO_HDRS} ${TEST_RUNNER_PATH}/test_runner.cpp src/test.cpp src/merge.cpp)
target_link_libraries(yellow_pages ${Protobuf_LIBRARIES})

