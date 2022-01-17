// 常规的请求/响应协议
#include "headers/normal_protocol.h"
#include "../../util/headers/variant.h"

// 从缓冲区反序列化数据 @param2: 消息总长度
base_req_msg parse_from_buffer(byte_buffer&, uint_32);

void normal_protocol_filter(server_connect &connect, void *arg, filter_chain &filters) {
    byte_buffer &in_buf = connect.in();
    uint_64 full_msg_len = parse_variant(in_buf.inner_ptr());
    if(in_buf.size() != 0 && in_buf.size() >= full_msg_len) {
        base_req_msg msg = parse_from_buffer(in_buf, full_msg_len);
        if(msg.target().empty()) msg.set_target("/");
        filters.do_filter(connect, &msg);
    }
}

base_req_msg parse_from_buffer(byte_buffer &buffer, uint_32 full_len) {
    base_req_msg msg;
    uint_64 head_f_len  = variant_len(full_len);
    msg.ParseFromArray(buffer.inner_ptr() + head_f_len, (int) (full_len - head_f_len));
    return move(msg);
}

