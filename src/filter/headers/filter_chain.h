#ifndef CHAT_SERVER_FILTER_CHAIN_H
#define CHAT_SERVER_FILTER_CHAIN_H

#include <initializer_list>
#include <deque>

using std::deque;
using std::shared_ptr;
using std::make_shared;

class filter_chain;

typedef void (*filter_func)(class server_connect &, shared_ptr<void>, filter_chain &);
typedef std::initializer_list<filter_func> filter_initializer;

class filter_chain {

public:
    filter_chain() = default;
    filter_chain(filter_initializer initializer): filters(initializer){}
    filter_chain(const filter_chain &) = default;
    filter_chain(filter_chain &&) = default;

    void add_filter(filter_func);
    void do_filter(server_connect &, shared_ptr<void> arg = shared_ptr<void>(nullptr));

    filter_chain& operator=(const filter_chain &) = default;
    filter_chain& operator=(filter_chain &&) = default;

private:
    deque<filter_func> filters;

};


#endif //CHAT_SERVER_FILTER_CHAIN_H
