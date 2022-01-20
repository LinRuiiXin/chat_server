#ifndef CHAT_SERVER_CONNECT_HOOK_H
#define CHAT_SERVER_CONNECT_HOOK_H

#include "server_connect.h"

class connect_hook {

public:
    virtual void on_active(server_connect &) = 0;
    virtual void* on_read_done(server_connect &, void *) = 0;
    virtual void on_closed(server_connect &) = 0;

    virtual ~connect_hook() = default;

private:

};


#endif //CHAT_SERVER_CONNECT_HOOK_H
