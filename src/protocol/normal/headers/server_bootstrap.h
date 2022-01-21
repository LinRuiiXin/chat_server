#ifndef CHAT_SERVER_SERVER_BOOTSTRAP_H
#define CHAT_SERVER_SERVER_BOOTSTRAP_H

#include "../../../net/headers/server_socket.h"
#include "../../../generic/headers/size_types.h"
#include "msg_router.h"

class server_bootstrap {

public:
    explicit server_bootstrap(uint_32 _port = 8080);
    server_bootstrap& register_handlers(const handlers_initializer&);
    void start() { server_sock.start(); };

    server_bootstrap(const server_bootstrap &) = delete;
    server_bootstrap(server_bootstrap &&) = delete;
    server_bootstrap& operator=(const server_bootstrap &) = delete;
    server_bootstrap& operator=(server_bootstrap &&) = delete;

private:
    server_socket server_sock;
    shared_ptr<msg_router> router;

};


#endif //CHAT_SERVER_SERVER_BOOTSTRAP_H
