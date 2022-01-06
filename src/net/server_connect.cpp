#include "headers/server_connect.h"
#include <unistd.h>

#define DEFAULT_BUF_SIZE 1024
#define DEFAULT_TEMP_BUF_SIZE 512

void tcp_read_handler(int, short, void *);  // NOLINT(readability-redundant-declaration)

server_connect::server_connect(int _sock_fd, event_base *_ev_base): sock_fd(_sock_fd), in_buffer(DEFAULT_BUF_SIZE),
                                                                    out_buffer(DEFAULT_BUF_SIZE) {
    evutil_make_socket_nonblocking(sock_fd);
    event = event_new(_ev_base, sock_fd, EV_READ | EV_PERSIST, tcp_read_handler, this);
    event_add(event, nullptr);
    printf("client %d connected\n", sock_fd);
}

void tcp_read_handler(int sock_fd, short events, void *arg) {
    printf("connect %d is already to read\n", sock_fd);
    auto connect_ptr = (server_connect*) arg;

    char buffer[DEFAULT_TEMP_BUF_SIZE];
    ssize_t len = read(sock_fd, buffer, DEFAULT_TEMP_BUF_SIZE - 1);

    if(len <= 0) {
        perror("some errors happened when read\n");
        event_free(connect_ptr->event);
        close(sock_fd);
        return;
    }
    connect_ptr->in_buffer.write(buffer, len);
    printf("received message from %d: %s\n", sock_fd, buffer);
}
