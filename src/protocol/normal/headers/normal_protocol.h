#ifndef CHAT_SERVER_NORMAL_PROTOCOL_H
#define CHAT_SERVER_NORMAL_PROTOCOL_H

#include <src/protocol/normal/meta/base_req_msg.pb.h>
#include "../../../net/headers/server_connect.h"

class normal_protocol_filter: public filter {

public:
    void do_filter(server_connect &connect, void *pVoid, filter_chain &chain) override;

};

#endif //CHAT_SERVER_NORMAL_PROTOCOL_H
