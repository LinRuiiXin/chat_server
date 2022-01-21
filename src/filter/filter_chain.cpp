#include "headers/filter_chain.h"

void filter_chain::start_filter(server_connect &conn, void *arg) {
    filter_chain chain_cp = *this;
    chain_cp.do_filter(conn, arg);
}

void filter_chain::do_filter(server_connect &conn, void *arg) {
    if(filters.empty()) return;
    auto filter_ptr = filters.front();
    filters.pop_front();
    filter_ptr->do_filter(conn, arg, *this);
}
