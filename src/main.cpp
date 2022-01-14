#include <iostream>
#include <thread>
#include <utility>
#include <src/meta/base_msg.pb.h>
#include <src/meta/chat_msg.pb.h>
#include <unistd.h>
#include "net/headers/server_socket.h"
#include "filter/headers/filter_chain.h"
#include "util/headers/variant.h"

#define PORT 8080

using std::cout;
using std::endl;

void client_thread_runner();

void message_protocol_filter(server_connect &, void *, filter_chain &);
void message_handler(server_connect &, void *, filter_chain &);

int main() {
    std::thread client_thread(client_thread_runner);
    server_socket socket(PORT);
    socket.set_filter_chain({message_protocol_filter, message_handler});
    socket.start();
    return 0;
}


// 网络数据经过的第一个拦截器, 主要用于编解码消息, 此处将一次报文规定为以 EOF 结束, 然后将消息反序列化为 generic_msg
void message_protocol_filter(server_connect &connect, void *, filter_chain &filters) {
    char_buffer &in_buffer = connect.in();
    const uint_8 *buf_ptr = in_buffer.inner_ptr();
    uint_64 msg_len = parse_variant(buf_ptr);
    if(in_buffer.size() != 0 && in_buffer.size() >= msg_len ) {
        auto msg_buffer = in_buffer.read(msg_len);
        uint_32 head_field_len = calculate_variant(msg_len);
        base_msg msg;
        msg.ParseFromArray(msg_buffer.get() + head_field_len, msg_len - head_field_len); // NOLINT(cppcoreguidelines-narrowing-conversions)
        printf("[message_protocol_filter] parse byte buffer to generic_msg: {timestamp : %llu}\n", msg.timestamp());
        filters.do_filter(connect, &msg);
    }
}

void message_handler(server_connect &connect, void *arg, filter_chain &filter) {
    auto *gen_msg = static_cast<base_msg*>(arg);
    chat_msg msg;
    msg.ParseFromString(gen_msg->body());
    printf("[message handler] from: %s to: %s content: %s\n", msg.from().c_str(), msg.to().c_str(), msg.content().c_str());
    base_msg resp_msg;
    resp_msg.set_timestamp(201750270118);
    string resp = resp_msg.SerializeAsString();
    connect.write((void *) resp.c_str(), resp.length());
    filter.do_filter(connect, gen_msg);
}

void client_thread_runner() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        perror("socket error");
        exit(1);
    }
    struct sockaddr_in serveraddr{};
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    if (connect(socketfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        perror("connect error");
        exit(1);
    }
    base_msg msg;
    msg.set_timestamp(1641893011425);
    chat_msg chat;
    chat.set_from("LinRuiXin");
    chat.set_to("LinRuiXin");
    chat.set_content("HelloWorld");
    msg.set_body(chat.SerializeAsString());
    size_t msg_len = msg.ByteSizeLong();
    size_t head_field_len = calculate_variant(calculate_variant(msg_len) + msg_len);
    size_t total_len = msg_len + head_field_len;

    unique_ptr<char[]> buffer = unique_ptr<char[]>(new char[total_len]);
    serialize_variant(total_len, buffer.get());
    msg.SerializeToArray(buffer.get() + head_field_len, msg_len); // NOLINT(cppcoreguidelines-narrowing-conversions)
    write(socketfd, buffer.get(), total_len);
    uint_8 resp_buf[1024];
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    uint_32 read_num = read(socketfd, resp_buf, sizeof(resp_buf));
    msg.ParseFromArray(resp_buf, read_num); // NOLINT(cppcoreguidelines-narrowing-conversions)
    printf("response from server: %llu\n", msg.timestamp());
}
