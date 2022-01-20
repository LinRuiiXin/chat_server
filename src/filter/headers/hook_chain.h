#ifndef CHAT_SERVER_HOOK_CHAIN_H
#define CHAT_SERVER_HOOK_CHAIN_H

#include <utility>
#include <vector>
#include "../../net/headers/connect_hook.h"

typedef std::vector<std::shared_ptr<connect_hook>> hook_vec;

class hook_chain: public connect_hook {

public:
    explicit hook_chain(hook_vec _hooks): hooks(_hooks) {}
    explicit hook_chain(hook_vec &&_r_hooks): hooks(_r_hooks) {}

    void on_active(server_connect &) override;
    void *on_read_done(server_connect &, void *) override;
    void on_closed(server_connect &) override;


private:
    hook_vec hooks;

};


#endif //CHAT_SERVER_HOOK_CHAIN_H
