// 常规的请求/响应协议
#include "headers/normal_protocol.h"
#include "../../util/headers/variant.h"
#include "util/headers/serialize.h"

void normal_protocol_filter::do_filter(server_connect &connect, void *arg, filter_chain &filters) {
    byte_buffer &in_buf = connect.in();
    uint_64 full_msg_len = parse_variant(in_buf.inner_ptr());
    if(in_buf.size() != 0 && in_buf.size() >= full_msg_len) {
        auto msg = parse_from_bytes<base_req_msg>(in_buf.inner_ptr(), full_msg_len);
        if(msg.target().empty()) msg.set_target("/");
        filters.do_filter(connect, &msg);
    }
}
