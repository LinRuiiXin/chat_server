#ifndef CHAT_SERVER_SERVER_CONNECT_H
#define CHAT_SERVER_SERVER_CONNECT_H

#include <event.h>
#include <algorithm>
#include <vector>
#include "../../filter/headers/filter_chain.h"
#include "../../buffer/headers/byte_buffer.h"
#include "../../filter/headers/hook_chain.h"

class server_socket;

using std::move;

// 该类将一个连接封装, 用于负责一个连接的拦截链调度, 读写事件
class server_connect final {

//    friend void tcp_read_write_handler(int, short, void *);
    friend void tcp_read_handler(int, server_connect &);
    friend void tcp_write_handler(int, server_connect &);

public:
    server_connect(class server_socket &, int, event_base *);
    server_connect(server_connect &&) noexcept;
    void write(const void *, uint_32); // 向 socket 写数据
    byte_buffer& in() { return in_buffer; }
    byte_buffer& out() { return out_buffer; }

    server_connect(const server_connect &) = delete;
    server_connect &operator=(server_connect &) = delete;

private:
    server_socket &server_sock;
    const int sock_fd;
    struct event *event;
    byte_buffer in_buffer;
    byte_buffer out_buffer;

};

#endif //CHAT_SERVER_SERVER_CONNECT_H
