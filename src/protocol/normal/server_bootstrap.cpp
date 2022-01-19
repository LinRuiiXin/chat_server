#include "headers/server_bootstrap.h"
#include "headers/normal_protocol.h"
#include "headers/msg_router.h"

server_bootstrap::server_bootstrap(uint_32 _port): server_sock(_port) {
    server_sock.set_filter_chain({normal_protocol_filter, msg_router::router_pre_filter});
}

server_bootstrap &server_bootstrap::register_handlers(const handlers_initializer& handler_i) {
    msg_router::set_handlers(handler_i);
    return *this;
}
