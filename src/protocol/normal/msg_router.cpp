#include <src/protocol/normal/meta/base_req_msg.pb.h>
#include <src/protocol/normal/meta/base_resp_msg.pb.h>
#include "headers/msg_router.h"
#include "../../util/headers/timestamp.h"
#include "../../util/headers/variant.h"
#include "headers/status_code.h"

void log_err_and_exit(const char *);
base_resp_msg resource_not_found_msg();

void msg_router::set_handlers(handlers_initializer initializer) {
    for(auto &it : initializer) {
        auto handler_ptr = move(it);
        if(handler_exist(handler_ptr->get_path())) log_err_and_exit("existed two or more same path of handler\n");
        handlers.insert(handler_pair(handler_ptr->get_path(), move(handler_ptr)));
    }
}

void msg_router::router_pre_filter(server_connect &conn, void *arg, filter_chain &filters) {
    auto msg = (base_req_msg*) arg;
    base_resp_msg resp_msg = route(conn, *msg);

    uint_64 msg_len = resp_msg.ByteSizeLong();
    uint_64 head_f_len = variant_len(msg_len + variant_len(msg_len));
    uint_64 full_len = msg_len + head_f_len;
    auto *out_buffer = new uint_8[full_len];
    serialize_variant(full_len, out_buffer);
    resp_msg.SerializeToArray(out_buffer + head_f_len, (int) msg_len);

    conn.write(out_buffer, full_len);
}

base_resp_msg msg_router::route(server_connect &conn, base_req_msg &req_msg) {
    auto handler_it = handlers.find(req_msg.target());
    if(is_null_handler(handler_it)) return resource_not_found_msg();
    else {
        base_resp_msg resp_msg; auto &handler_ptr = handler_it->second;

        string resp_body = handler_ptr->handler(conn, req_msg);

        resp_msg.set_code(SUCCESS);
        resp_msg.set_timestamp(current_timestamp());
        resp_msg.set_body(move(resp_body));
        return move(resp_msg);
    }
}

base_resp_msg resource_not_found_msg() {
    base_resp_msg msg;
    msg.set_timestamp(current_timestamp());
    msg.set_code(RESOURCE_NOT_FOUND);
    return move(msg);
}

void log_err_and_exit(const char *msg) {
    perror(msg);
    exit(-1);
}