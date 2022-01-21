#include "headers/hook_chain.h"


void hook_chain::on_active(server_connect &conn) {
    for(const auto &hook_ptr : hooks) hook_ptr->on_active(conn);
}

void hook_chain::on_closed(server_connect &conn) {
    for(const auto &hook_ptr : hooks) hook_ptr->on_closed(conn);
}
