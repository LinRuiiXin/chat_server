cmake_minimum_required(VERSION 3.19)
project(chat_server)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/Cellar/libevent/2.1.12/include)
link_directories(/usr/local/Cellar/libevent/2.1.12/lib)

add_executable(chat_server src/main.cpp src/net/server_socket.cpp src/net/headers/server_socket.h src/net/server_connect.cpp src/net/headers/server_connect.h src/buffer/char_buffer.cpp src/buffer/headers/char_buffer.h src/buffer/char_buffer.cpp src/buffer/headers/char_buffer.h)

target_link_libraries(chat_server event)