#ifndef CHAT_SERVER_SERVER_CONNECT_H
#define CHAT_SERVER_SERVER_CONNECT_H

#include <event.h>
#include <algorithm>
#include "../../filter/headers/filter_chain.h"
#include "../../buffer/headers/char_buffer.h"

using std::move;

// 该类将一个连接封装, 用于负责一个连接的拦截链调度, 读写事件
class server_connect final {

    friend void tcp_read_handler(int, short, void *);

public:
    server_connect(class server_socket &, int, event_base *, filter_chain);
    server_connect(server_connect &&) noexcept;
    char_buffer& in() { return in_buffer; }
    char_buffer& out() { return out_buffer; }

    server_connect(const server_connect &) = delete;
    server_connect &operator=(server_connect &) = delete;

private:
    server_socket &server_sock;
    const int sock_fd;
    struct event *event;
    char_buffer in_buffer;
    char_buffer out_buffer;
    filter_chain filters;

};

#endif //CHAT_SERVER_SERVER_CONNECT_H
