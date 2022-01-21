#include "headers/keep_alive_bootstrap.h"
#include "../normal/headers/normal_protocol.h"
#include "headers/keep_alive_router.h"

keep_alive_bootstrap::keep_alive_bootstrap(uint_32 _port): server_sock(_port) {
    server_sock.set_filter_chain({normal_protocol_filter, keep_alive_router::keep_alive_router_pre_filter});
}

keep_alive_bootstrap &keep_alive_bootstrap::register_handlers(const handlers_initializer& handler_i) {
    msg_router::set_handlers(handler_i);
    return *this;
}