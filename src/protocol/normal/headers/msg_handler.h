#ifndef CHAT_SERVER_MSG_HANDLER_H
#define CHAT_SERVER_MSG_HANDLER_H

#include <string>
#include <src/protocol/normal/meta/base_req_msg.pb.h>
#include "../../../net/headers/server_connect.h"

using std::string;

class msg_handler {

public:
    explicit msg_handler(const string &_path): path(_path) {}
    string& get_path() { return path; }
    virtual string handler(server_connect &, base_req_msg &) = 0;
    virtual ~msg_handler() = default;

private:
    string path;

};

#endif //CHAT_SERVER_MSG_HANDLER_H
