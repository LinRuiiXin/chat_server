#include "headers/filter_chain.h"
#include <utility>

void filter_chain::add_filter(filter_func filter) {
    filters.push_front(filter);
}

// 从队首取出一个拦截器, 并调用
void filter_chain::do_filter(server_connect &connect, void *arg) {
    if(filters.empty()) return;
    auto filter_ptr = filters.front();
    filters.pop_front();
    filter_ptr(connect, arg, *this);
}