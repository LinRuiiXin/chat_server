#include "headers/server_socket.h"
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <event.h>

//#define READ_BUF_SIZE 1024

using std::move;

typedef struct sockaddr *SAP;

void log_error_and_exit(const char *msg);

void tcp_accept_handler(int, short, void*); // NOLINT(readability-redundant-declaration)

//void tcp_read_write_handler(int, short, void*);

server_socket::server_socket(uint_32 _port): port(_port), started(false), ev_base(event_base_new()),
                                             accept_fd(-1), accept_event(nullptr), hooks() {
    if(ev_base == nullptr) log_error_and_exit("unable to init event-base\n");
}

void server_socket::set_filter_chain(filter_initializer _initializer) {
    filters = move(filter_chain(_initializer));
}

void server_socket::set_hooks(const hook_chain &_hook_chain) {
    hooks = _hook_chain;
}

// 开启服务
void server_socket::start() {
    if (!started) {
        accept_fd = open_tcp_socket();
        if(accept_fd == -1) log_error_and_exit("unable to open tcp socket\n");

        // 设置 accept_socket 监听连接事件并且在事件发生时回调 tcp_accept_handler
        accept_event = event_new(ev_base, accept_fd, EV_READ | EV_PERSIST, tcp_accept_handler, this);
        event_add(accept_event, nullptr);

        printf("server is already for accept server_connect\n");
        started = true;

        event_base_dispatch(ev_base); // 启动事件循环
    }
}

// 接收请求事件回调
void tcp_accept_handler(int socket_fd, short events, void *arg) {
    auto server_sk_ptr = (server_socket*) arg;

    struct sockaddr_in client{};
    socklen_t socklen = sizeof (client);

    evutil_socket_t client_sock_fd = accept(socket_fd, (SAP) &client, &socklen); // 接收一个连接，并且返回一个连接的 socket 文件描述符
    if(client_sock_fd < 0) return;
    server_sk_ptr->connect_map.insert(connect_pair(client_sock_fd, server_connect(*server_sk_ptr, client_sock_fd, server_sk_ptr->ev_base, server_sk_ptr->hooks)));
}

// 打开一个接收连接的 socket，设置为非阻塞，返回 socket 文件描述符
int server_socket::open_tcp_socket() {
    int errno_save;

    // 开启一个 socket 返回对应文件描述符
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) return -1;

    // 允许多次绑定同一个地址
    evutil_make_listen_socket_reuseable(socket_fd);

    struct sockaddr_in sin{};
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(port);

    if (bind(socket_fd, (SAP) &sin, sizeof(sin)) < 0) goto err; // 绑定端口与 socket_fd;
    if (listen(socket_fd, MAX_ACCEPT) < 0) goto err;            // 监听 socket
    printf("server socket opened with port: %d\n", port);

    evutil_make_socket_nonblocking(socket_fd); // 配置 socket 非阻塞

    return socket_fd;

    err: // 错误处理
        errno_save = errno;
        evutil_closesocket(socket_fd);
        errno = errno_save;
        return -1;
}

// 输出错误信息并且退出程序
void log_error_and_exit(const char *msg) {
    perror(msg);
    exit(-1);
}
