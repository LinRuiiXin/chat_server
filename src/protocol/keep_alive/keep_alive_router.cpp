#include <src/protocol/normal/meta/base_req_msg.pb.h>
#include "headers/keep_alive_router.h"
#include "../normal/headers/msg_router.h"
#include "../normal/util/headers/serialize.h"

void keep_alive_router::do_filter(server_connect &connect, void *arg, filter_chain &chain) {
    auto req_msg = (base_req_msg*) arg;
    auto resp_msg = route(conn, *req_msg);
    auto out_buffer = serialize_as_buffer(req_msg);
    conn.write(out_buffer.buffer.get(), out_buffer.len);
}
