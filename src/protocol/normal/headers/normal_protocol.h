#ifndef CHAT_SERVER_NORMAL_PROTOCOL_H
#define CHAT_SERVER_NORMAL_PROTOCOL_H

#include <src/protocol/normal/meta/base_req_msg.pb.h>
#include "../../../net/headers/server_connect.h"

void normal_protocol_filter(class server_connect &, void *, class filter_chain &);

#endif //CHAT_SERVER_NORMAL_PROTOCOL_H
