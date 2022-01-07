#include "headers/server_connect.h"
#include <unistd.h>

#define DEFAULT_BUF_SIZE 512

void tcp_read_handler(int, short, void *);  // NOLINT(readability-redundant-declaration)

server_connect::server_connect(server_socket &_server_sock, int _sock_fd, event_base *_ev_base)
        : server_sock(_server_sock), sock_fd(_sock_fd), in_buffer(), out_buffer() {

    evutil_make_socket_nonblocking(sock_fd);
    event = event_new(_ev_base, sock_fd, EV_READ | EV_PERSIST, tcp_read_handler, this);
    event_add(event, nullptr);
    printf("client %d connected\n", sock_fd);
}

server_connect::server_connect(server_connect &&_r_c) noexcept :server_sock(_r_c.server_sock), sock_fd(_r_c.sock_fd),
                                                                in_buffer(move(_r_c.in_buffer)), out_buffer(move(_r_c.out_buffer)), event(_r_c.event) {
    _r_c.event = nullptr;
    event_assign(event, event->ev_base, sock_fd, EV_READ | EV_PERSIST, tcp_read_handler, this);
}

void tcp_read_handler(int sock_fd, short events, void *arg) {
    printf("connect %d is already to read\n", sock_fd);
    auto connect_ptr = (server_connect*) arg;

    char buffer[DEFAULT_BUF_SIZE];
    ssize_t len = read(sock_fd, buffer, DEFAULT_BUF_SIZE);

    if(len <= 0) {
        perror("some errors happened when reading\n");
        event_free(connect_ptr->event);
        close(sock_fd);
        return;
    }
    connect_ptr->in_buffer.write(buffer, len);
    printf("received message from %d: %s\n", sock_fd, buffer);
}
