#ifndef CHAT_SERVER_AUTO_CONVERT_HANDLER_H
#define CHAT_SERVER_AUTO_CONVERT_HANDLER_H

#include "../../headers/msg_handler.h"

// 该模版类将消息中的 body 字段反序列化为派生类所指定的类型 accept_t, 并将返回参数类型 return_t 序列化
// 使得派生类可以不用重复 序列化/反序列化 的过程
// accept_t 与 return_t 仅适用于 google-protobuf3 生成的类
template <typename accept_t, typename return_t>
class auto_convert_handler: public msg_handler {

public:
    explicit auto_convert_handler(const string &_path): msg_handler(_path){}

    string handler(server_connection &connect, base_req_msg &msg) override {
        accept_t accept_type;
        if(!msg.body().empty()) accept_type.ParseFromString(msg.body());
        auto return_type = do_handler(connect, msg, accept_type);
        return move(return_type.SerializeAsString());
    }

    virtual return_t do_handler(server_connection &, base_req_msg &, accept_t &) = 0;

private:

};

#endif //CHAT_SERVER_AUTO_CONVERT_HANDLER_H
