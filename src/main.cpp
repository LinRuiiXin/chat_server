#include <iostream>
#include <utility>
#include "net/header/server_socket.h"

using std::move;

int main() {
    server_socket socket(8080);
    socket.start();
    return 0;
}
