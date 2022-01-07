#include <iostream>
#include <utility>
#include "net/headers/server_socket.h"
#include "buffer/headers/char_buffer.h"

using std::move;

int main() {
    server_socket socket(8080);
    socket.start();
    return 0;
}