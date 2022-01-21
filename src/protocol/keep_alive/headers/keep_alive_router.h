#ifndef CHAT_SERVER_KEEP_ALIVE_ROUTER_H
#define CHAT_SERVER_KEEP_ALIVE_ROUTER_H

#include "../../../net/headers/server_connect.h"
#include "../../normal/headers/msg_router.h"

class keep_alive_router: public msg_router {

public:
    void do_filter(server_connect &connect, void *pVoid, filter_chain &chain) override;

private:

};

#endif //CHAT_SERVER_KEEP_ALIVE_ROUTER_H
