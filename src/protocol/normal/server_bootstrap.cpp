#include "headers/server_bootstrap.h"
#include "headers/normal_protocol.h"
#include "headers/msg_router.h"

server_bootstrap::server_bootstrap(uint_32 _port): server_sock(_port), router(make_shared<msg_router>()) {
    server_sock.set_filter_chain(filter_chain({ make_shared<normal_protocol_filter>(), router }));
}

server_bootstrap& server_bootstrap::register_handlers(const handlers_initializer& handler_i) {
    router->set_handlers(handler_i);
    return *this;
}
