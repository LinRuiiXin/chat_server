#include "headers/filter_chain.h"
#include <utility>

void filter_chain::add_filter(filter_func filter) {
    filters.push_front(filter);
}

void filter_chain::do_filter(server_connect &connect, shared_ptr<void> arg) {
    if(filters.empty()) return;
    auto filter_ptr = filters.front();
    filter_ptr(connect, std::move(arg), *this);
    filters.pop_front();
}