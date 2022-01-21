#ifndef CHAT_SERVER_FILTER_H
#define CHAT_SERVER_FILTER_H

#include "../../net/headers/server_connect.h"
#include "filter_chain.h"

class filter {

public:
    virtual void do_filter(class server_connect &, void *, class filter_chain &) {};
    virtual ~filter() = default;

};


#endif //CHAT_SERVER_FILTER_H
