#ifndef CHAT_SERVER_SERVER_SOCKET_H
#define CHAT_SERVER_SERVER_SOCKET_H

#include "server_connect.h"
#include "../../filter/headers/filter_chain.h"
#include <memory>
#include <event.h>
#include <map>

#define MAX_ACCEPT 65535

using std::unique_ptr;
using std::shared_ptr;

typedef unsigned int uint_32;
typedef std::map<int, class server_connect> connect_map;
typedef std::pair<int, class server_connect> connect_pair;

// 该类作为一个服务接收 tcp 连接, 可以对该类进行配置
class server_socket {

    friend void tcp_accept_handler(int, short, void*);

public:
    explicit server_socket(uint_32 _port = 80);
    void set_filter_chain(filter_initializer); // 设置连接事件拦截器
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
    connect_map connect_map{};          // 管理所有连接的 map
    filter_chain filters;               // 连接过滤拦截器

    int open_tcp_socket();

};


#endif //CHAT_SERVER_SERVER_SOCKET_H
