#include <iostream>
#include <thread>
#include <src/meta/generic_msg.pb.h>
#include <unistd.h>
#include "net/headers/server_socket.h"
#include "filter/headers/filter_chain.h"

#define PORT 8080

using std::cout;
using std::endl;

void client_thread_runner();

void message_protocol_filter(server_connect &, shared_ptr<void>, filter_chain &);

int main() {
    std::thread client_thread(client_thread_runner);
    server_socket socket(PORT);
    socket.set_filter_chain({message_protocol_filter});
    socket.start();
    return 0;
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
    auto len = msg.ByteSizeLong() + 1;
    unique_ptr<char[]> buf = unique_ptr<char[]>(new char[len]);
    msg.SerializeToArray(buf.get(), len - 1);
    buf[msg.ByteSizeLong()] = EOF;
    if (write(socketfd, buf.get(), len) > 0)
        close(socketfd);
}

void message_protocol_filter(server_connect &connect, shared_ptr<void> arg, filter_chain &filters) {
    auto &in_buffer = connect.in();
    int counter = 1;
    for (auto it = in_buffer.begin(); it != in_buffer.end(); ++it) {
        if (*it == EOF) {
            unique_ptr<char[]> protocol_buffer = in_buffer.read(counter);
            auto g_msg = make_shared<generic_msg>();
            if (g_msg->ParseFromArray(protocol_buffer.get(), counter - 1)) {
                protocol_buffer.reset();
                printf("[message_protocol_filter] parse request body to generic_msg: {time_stamp: %llu}\n",
                       g_msg->timestamp());
                filters.do_filter(connect, g_msg);
            } else perror("[message_protocol_filter] invalid protocol");
            return;
        }
        ++counter;
    }
}
