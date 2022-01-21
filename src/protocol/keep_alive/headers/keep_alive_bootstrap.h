#ifndef CHAT_SERVER_KEEP_ALIVE_BOOTSTRAP_H
#define CHAT_SERVER_KEEP_ALIVE_BOOTSTRAP_H

#include "../../generic/headers/size_types.h"
#include "../../../net/headers/server_socket.h"
#include "../../normal/headers/msg_router.h"

#define DEF_PORT 8080

class keep_alive_bootstrap {

public :
    explicit keep_alive_bootstrap(uint_32 = DEF_PORT);
    keep_alive_bootstrap &register_handlers(const handlers_initializer&);

    keep_alive_bootstrap(const keep_alive_bootstrap &) = delete;
    keep_alive_bootstrap(keep_alive_bootstrap &&) = delete;
    keep_alive_bootstrap& operator=(const keep_alive_bootstrap &) = delete;
    keep_alive_bootstrap& operator=(keep_alive_bootstrap &&) = delete;

private:
    server_socket server_sock;

};


#endif //CHAT_SERVER_KEEP_ALIVE_BOOTSTRAP_H
