#ifndef CHAT_SERVER_SERVER_CONNECT_H
#define CHAT_SERVER_SERVER_CONNECT_H

#include "../../buffer/headers/char_buffer.h"
#include "server_socket.h"
#include <event.h>
#include <memory>

using std::move;

class server_connect final {

    friend void tcp_read_handler(int, short, void *);

public:
    explicit server_connect(server_socket &, int, event_base *);
    server_connect(server_connect &&) noexcept;

    server_connect(const server_connect &) = delete;
    server_connect &operator=(server_connect &) = delete;

private:
    server_socket &server_sock;
    const int sock_fd;
    struct event *event;
    char_buffer in_buffer;
    char_buffer out_buffer;

};

#endif //CHAT_SERVER_SERVER_CONNECT_H
