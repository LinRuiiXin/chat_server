#include "headers/hook_chain.h"


void hook_chain::on_active(server_connect &conn) {
    for(const auto &hook_ptr : hooks) hook_ptr->on_active(conn);
}

void* hook_chain::on_read_done(server_connect &conn, void *arg) {
    void *arg_temp = nullptr;
    for(const auto &hook_ptr : hooks) arg_temp = hook_ptr->on_read_done(conn, arg_temp);
    return nullptr;
}

void hook_chain::on_closed(server_connect &conn) {
    for(const auto &hook_ptr : hooks) hook_ptr->on_closed(conn);
}
