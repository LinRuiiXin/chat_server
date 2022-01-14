#ifndef CHAT_SERVER_FILTER_CHAIN_H
#define CHAT_SERVER_FILTER_CHAIN_H

#include <initializer_list>
#include <deque>

using std::deque;
using std::shared_ptr;
using std::make_shared;

class filter_chain;

// 函数接收三个参数 server_connect 表示当前处理事件对应的连接, shared_ptr 允许你将当前处理结果传递给下一个拦截器
// 在函数结束之前, 除非遇到了异常, 否则你应该手动调用 filter_chain.do_filter() 函数以确保之后的拦截器被调用
typedef void (*filter_func)(class server_connect &, void*, filter_chain &);
typedef std::initializer_list<filter_func> filter_initializer;

class filter_chain {

public:
    filter_chain() = default;
    filter_chain(filter_initializer initializer): filters(initializer){}
    filter_chain(const filter_chain &) = default;
    filter_chain(filter_chain &&) = default;

    void add_filter(filter_func);
    void do_filter(server_connect &, void *arg = nullptr);

    filter_chain& operator=(const filter_chain &) = default;
    filter_chain& operator=(filter_chain &&) = default;

private:
    deque<filter_func> filters;

};


#endif //CHAT_SERVER_FILTER_CHAIN_H
