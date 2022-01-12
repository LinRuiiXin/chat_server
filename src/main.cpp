#include <iostream>
#include <thread>
#include <src/meta/generic_msg.pb.h>
#include <unistd.h>
#include "net/headers/server_socket.h"
#include "filter/headers/filter_chain.h"
#include "util/headers/variant.h"

#define PORT 8080

using std::cout;
using std::endl;

void client_thread_runner();

uint_64 get_generic_msg_len(char_buffer &);
void message_protocol_filter(server_connect &, shared_ptr<void>, filter_chain &);

int main() {
    std::thread client_thread(client_thread_runner);
    server_socket socket(PORT);
    socket.set_filter_chain({message_protocol_filter});
    socket.start();
    return 0;
}


// 网络数据经过的第一个拦截器, 主要用于编解码消息, 此处将一次报文规定为以 EOF 结束, 然后将消息反序列化为 generic_msg
void message_protocol_filter(server_connect &connect, shared_ptr<void> arg, filter_chain &filters) {
    char_buffer &in_buffer = connect.in();
    uint_64 msg_len = get_generic_msg_len(in_buffer);
    if(in_buffer.size() != 0 && msg_len >= in_buffer.size()) {
        unique_ptr<char[]> msg_buffer = in_buffer.read(msg_len);
        uint_32 head_field_len = calculate_variant(msg_len);
        auto msg = make_shared<generic_msg>();
        msg->ParseFromArray(msg_buffer.get() + head_field_len, msg_len - head_field_len); // NOLINT(cppcoreguidelines-narrowing-conversions)
        filters.do_filter(connect, msg);
    }
}

uint_64 get_generic_msg_len(char_buffer &buffer) {
    uint_64 res = 0; uint_32 count = 0;
    for(auto buf_ptr = buffer.begin(); buf_ptr != buffer.end() && append(&res, *buf_ptr, count); ++buf_ptr, ++count){}
    return res;
}

void client_thread_runner() {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        perror("socket error");
        exit(1);
    }
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    if (connect(socketfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) {
        perror("connect error");
        exit(1);
    }
    generic_msg msg;
    msg.set_timestamp(1641893011425);
    size_t msg_len = msg.ByteSizeLong();
    size_t head_field_len = calculate_variant(calculate_variant(msg_len) + msg_len);
    size_t total_len = msg_len + head_field_len;

    unique_ptr<char[]> buffer = unique_ptr<char[]>(new char[total_len]);
    serialize_variant(total_len, buffer.get());
    msg.SerializeToArray(buffer.get() + head_field_len, msg_len); // NOLINT(cppcoreguidelines-narrowing-conversions)
    write(socketfd, buffer.get(), total_len);
}

//int main(){
//    uint_8 buffer[3]{0};
//    serialize_variant(23333, buffer);
//    char_buffer c_buffer;
//    c_buffer.write(reinterpret_cast<const char *>(buffer), 3);
//    cout << get_generic_msg_len(c_buffer) << endl;
//    return 0;
//}
