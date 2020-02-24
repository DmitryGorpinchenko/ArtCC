cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)  # Испльзуем C++ 17

set (TEST_RUNNER_PATH "~/Documents/ArtCC/test_runner")
set (PROFILE_PATH "~/Documents/ArtCC/profile")
set (SRCS src/phone_book.cpp src/main.cpp ${TEST_RUNNER_PATH}/test_runner.cpp)

find_package(Protobuf REQUIRED) # Команда находит пути к компилятору protoc и библиотеке libprotobuf
include_directories(${Protobuf_INCLUDE_DIRS}) # Добавляем в include path пути к библиотеке protobuf
include_directories(${CMAKE_CURRENT_BINARY_DIR}) # В ${CMAKE_CURRENT_BINARY_DIR} будут файлы, сгенерированные компилятором protoс, путь к ним надо добавить в include path
include_directories(${TEST_RUNNER_PATH} ${PROFILE_PATH})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS src/contact.proto) # Здесь надо перечислить все ваши .proto-файлы
add_executable(main ${SRCS} ${PROTO_SRCS} ${PROTO_HDRS}) # Здесь надо перечислить все ваши .cpp-файлы, в том числе и сгенерированные protoc'ом
target_link_libraries(main ${Protobuf_LIBRARIES}) # компонуем наш исполняемый файл с библиотекой libprotobuf 

