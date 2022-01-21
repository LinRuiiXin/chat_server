#ifndef CHAT_SERVER_CONNECT_HOOK_H
#define CHAT_SERVER_CONNECT_HOOK_H

#include "../../net/headers/server_connect.h"

class server_connect;

// 连接生命钩子
class connect_hook {

public:
    virtual void on_active(server_connect &) {}; // 当连接可用
    virtual void on_read_done(server_connect &, void *) {}; // 当读事件完成
    virtual void on_closed(server_connect &) {}; // 当连接关闭

    virtual ~connect_hook() = default;

private:

};


#endif //CHAT_SERVER_CONNECT_HOOK_H
