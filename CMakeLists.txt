cmake_minimum_required(VERSION 3.19)
project(chat_server)

FIND_PACKAGE(Protobuf REQUIRED)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/Cellar/libevent/2.1.12/include)
link_directories(/usr/local/Cellar/libevent/2.1.12/lib)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB PROTO_FILES "src/**/*.proto") # 查找所有 protobuf 源文件

SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(FIL ${PROTO_FILES})
    GET_FILENAME_COMPONENT(ABS_FIL ${FIL} ABSOLUTE)
    GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)

    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
    string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})

    LIST(APPEND PROTO_SRCS "${FILE_PATH}${FIL_WE}.pb.cc")
    LIST(APPEND PROTO_HDRS "${FILE_PATH}${FIL_WE}.pb.h")

    EXECUTE_PROCESS( # 编译 .proto 文件
            COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

ENDFOREACH()

FILE(GLOB PROTO_OUT_FILES "${CMAKE_CURRENT_BINARY_DIR}/src/**/*.pb.**") # 匹配项目目录中所有 **.pb.** 的文件
message("[protobuf] out files: " ${PROTO_OUT_FILES})

add_executable(chat_server ${PROTO_OUT_FILES} src/main.cpp src/net/server_socket.cpp src/net/headers/server_socket.h src/net/server_connect.cpp src/net/headers/server_connect.h src/buffer/char_buffer.cpp src/buffer/headers/char_buffer.h src/buffer/char_buffer.cpp src/buffer/headers/char_buffer.h src/filter/filter_chain.cpp src/filter/headers/filter_chain.h )

target_link_libraries(chat_server event ${PROTOBUF_LIBRARIES})