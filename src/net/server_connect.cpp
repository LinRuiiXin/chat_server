#include "headers/server_connect.h"
#include <unistd.h>

#include <utility>

#define DEFAULT_BUF_SIZE 512

void tcp_read_write_handler(int, short, void *);  // NOLINT(readability-redundant-declaration)

// 将客户端 socket 与 server_connect 绑定, 并且向 event_base 注册一个监听数据可读的事件
server_connect::server_connect(class server_socket &_server_sock, int _sock_fd, event_base *_ev_base, filter_chain _filter_chain)
        : server_sock(_server_sock), sock_fd(_sock_fd), in_buffer(), out_buffer(), filters(std::move(_filter_chain)) {

    evutil_make_socket_nonblocking(sock_fd);
    event = event_new(_ev_base, sock_fd, EV_READ | EV_WRITE | EV_PERSIST, tcp_read_write_handler, this);
    event_add(event, nullptr);
    printf("client %d connected\n", sock_fd);
}

// 当 server_connect 对象被移动时, 应将事件的回调函数所传参数改为新的对象
server_connect::server_connect(server_connect &&_r_c) noexcept :server_sock(_r_c.server_sock), sock_fd(_r_c.sock_fd),
                                                                in_buffer(move(_r_c.in_buffer)), out_buffer(move(_r_c.out_buffer)),
                                                                event(_r_c.event), filters(move(_r_c.filters)) {
    _r_c.event = nullptr;
    event_assign(event, event->ev_base, sock_fd, EV_READ | EV_WRITE | EV_PERSIST, tcp_read_write_handler, this);
}

void server_connect::write(void *out_ptr, uint_32 len) {
    out_buffer.write(out_ptr, len);
}

// 当 socket 可读或可写
void tcp_read_write_handler(int sock_fd, short events, void *arg) {
    auto connect_ptr = (server_connect*) arg;
    if(events & EV_READ)  tcp_read_handler(sock_fd, *connect_ptr);
    if(events & EV_WRITE) tcp_write_handler(sock_fd, *connect_ptr);
}

void tcp_read_handler(int sock_fd, server_connect &conn) {
    // 将 socket 的数据读入临时缓冲区, 随后写入 server_connect 内部的数据缓冲区
    printf("connect %d is already to read\n", sock_fd);
    char buffer[DEFAULT_BUF_SIZE];
    ssize_t len = read(sock_fd, buffer, DEFAULT_BUF_SIZE);

    if(len <= 0) {
        perror("some errors happened when reading\n");
        event_free(conn.event);
        close(sock_fd);
        return;
    }
    conn.in_buffer.write(buffer, len);
    //printf("received message from %d: %s\n", sock_fd, buffer);
    // 调度请求拦截器
    conn.filters.start_filter(conn);
}

void tcp_write_handler(int sock_fd, server_connect &conn) {
    auto &out_buffer = conn.out();
    if(out_buffer.size() != 0){
        uint_32 flag = write(sock_fd, out_buffer.inner_ptr(), out_buffer.size());
        out_buffer.remove_front(flag);
    }
}