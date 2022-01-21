#include <src/protocol/normal/meta/base_req_msg.pb.h>
#include <src/protocol/normal/meta/base_resp_msg.pb.h>
#include "headers/msg_router.h"
#include "../../util/headers/timestamp.h"
#include "../../util/headers/variant.h"
#include "headers/status_code.h"
#include "util/headers/serialize.h"

void log_err_and_exit(const char *);
base_resp_msg resource_not_found_msg();

void msg_router::set_handlers(const handlers_initializer& initializer) {
    for(const auto& handler_ptr : initializer) {
        if(handler_exist(handler_ptr->get_path())) log_err_and_exit("existed two or more same path of handler\n");
        handlers.insert(handler_pair(handler_ptr->get_path(), handler_ptr));
    }
}

void msg_router::do_filter(server_connect &conn, void *arg, filter_chain &filters) {
    auto msg = (base_req_msg*) arg;
    base_resp_msg resp_msg = route(conn, *msg);
    serialize_buffer out_buffer = serialize_as_buffer(resp_msg);
    conn.write(out_buffer.buffer.get(), out_buffer.len);
}

base_resp_msg msg_router::route(server_connect &conn, base_req_msg &req_msg) {
    auto handler_it = handlers.find(req_msg.target());
    if(is_null_handler(handler_it)) { printf("[WARN] not found handler for: %s\n", req_msg.target().c_str()); return resource_not_found_msg(); }
    else {
        printf("[INFO] route into %s\n", req_msg.target().c_str());
        base_resp_msg resp_msg; auto handler_ptr = handler_it->second;

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