#ifndef CHAT_SERVER_MSG_ROUTER_H
#define CHAT_SERVER_MSG_ROUTER_H

#include <map>
#include <initializer_list>
#include <src/protocol/normal/meta/base_resp_msg.pb.h>
#include "msg_handler.h"

typedef std::vector<std::shared_ptr<msg_handler>> handlers_initializer;
typedef std::map<std::string, std::shared_ptr<msg_handler>> handler_map;
typedef std::pair<std::string, std::shared_ptr<msg_handler>> handler_pair;

// 用于消息分发, 通过消息字段 target 全匹配消息处理器
// 注: 消息处理器的存储结构使用 static 修饰, 即独立于 msg_router 对象存在
class msg_router: public filter {

public:
    inline bool is_null_handler(handler_map::iterator it) { return it == handlers.end(); }
    inline bool handler_exist(string &path) { return handlers.find(path) != handlers.end(); }

    void set_handlers(const handlers_initializer&);
    base_resp_msg route(server_connect &, base_req_msg &);
    // 作为 server_socket 的一个拦截器, 用于处理消息, 并且调度 route 分发请求
    void do_filter(server_connect &, void *, filter_chain &) override;

private:
    handler_map handlers;

};



#endif //CHAT_SERVER_MSG_ROUTER_H
