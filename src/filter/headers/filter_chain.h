#ifndef CHAT_SERVER_FILTER_CHAIN_H
#define CHAT_SERVER_FILTER_CHAIN_H

#include <memory>
#include <deque>
#include "filter.h"
#include "../../net/headers/server_connect.h"

using std::deque;
using std::shared_ptr;
using std::make_shared;

typedef deque<shared_ptr<filter>> filter_vec;

class filter_chain {

public:
    filter_chain(const filter_vec &_filters = {}): filters(_filters) {}
    filter_chain(const filter_chain &) = default;
    filter_chain& operator=(filter_chain &&) = default;

    filter_chain(filter_vec &&_r_filters): filters(_r_filters) {}
    void start_filter(server_connect &, void *);
    void do_filter(server_connect &, void *);


private:
    filter_vec filters;

};

#endif //CHAT_SERVER_FILTER_CHAIN_H
