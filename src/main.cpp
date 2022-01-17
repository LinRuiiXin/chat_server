#include <iostream>
#include <thread>
#include <utility>
#include <unistd.h>
#include "net/headers/server_socket.h"
#include "filter/headers/filter_chain.h"
#include "protocol/normal/headers/normal_protocol.h"
#include "util/headers/variant.h"

#define PORT 8080

using std::cout;
using std::endl;

void client_thread_runner();

int main() {
    std::thread client_thread(client_thread_runner);
    server_socket socket(PORT);
    socket.set_filter_chain({ normal_protocol_filter });
    socket.start();
    return 0;
}

//void client_thread_runner() {
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
//    if (socketfd < 0) {
//        perror("socket error");
//        exit(1);
//    }
//    struct sockaddr_in serveraddr{};
//    memset(&serveraddr, 0, sizeof(serveraddr));
//    serveraddr.sin_family = AF_INET;
//    serveraddr.sin_port = htons(PORT);
//    if (connect(socketfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
//        perror("connect error");
//        exit(1);
//    }
//    base_msg msg;
//    msg.set_timestamp(1641893011425);
//    chat_msg chat;
//    chat.set_from("LinRuiXin");
//    chat.set_to("LinRuiXin");
//    chat.set_content("HelloWorld");
//    msg.set_body(chat.SerializeAsString());
//    size_t msg_len = msg.ByteSizeLong();
//    size_t head_field_len = variant_len(variant_len(msg_len) + msg_len);
//    size_t total_len = msg_len + head_field_len;
//
//    unique_ptr<char[]> buffer = unique_ptr<char[]>(new char[total_len]);
//    serialize_variant(total_len, buffer.get());
//    msg.SerializeToArray(buffer.get() + head_field_len, msg_len); // NOLINT(cppcoreguidelines-narrowing-conversions)
//    write(socketfd, buffer.get(), total_len);
//    uint_8 resp_buf[1024];
//    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
//    uint_32 read_num = read(socketfd, resp_buf, sizeof(resp_buf));
//    msg.ParseFromArray(resp_buf, read_num); // NOLINT(cppcoreguidelines-narrowing-conversions)
//    printf("response from server: %llu\n", msg.timestamp());
//}
