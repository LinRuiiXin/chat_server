#ifndef CHAT_SERVER_SERVER_SOCKET_H
#define CHAT_SERVER_SERVER_SOCKET_H

#include "server_connect.h"
#include <memory>
#include <event.h>
#include <map>

#define MAX_ACCEPT 65535

typedef unsigned int uint_32;
typedef std::map<int, server_connect> connect_map;

using std::unique_ptr;

class server_socket {

public:
    explicit server_socket(uint_32 _port = 80);
    void start();
    virtual ~server_socket() = default;

    server_socket(const server_socket &) = delete;
    server_socket(server_socket &&) = delete;
    server_socket& operator=(const server_socket &) = delete;

private:
    volatile bool started;              // 启动状态
    struct event_base *const ev_base;   // lib-event 事件循环
    const uint_32 port;                 // 监听端口
    int accept_fd;                      // 接收连接 socket 文件描述符
    struct event *accept_event;         // 接收连接 event

    int open_tcp_socket();

};


#endif //CHAT_SERVER_SERVER_SOCKET_H
