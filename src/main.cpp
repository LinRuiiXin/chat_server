#include <thread>
#include <sys/socket.h>
#include <unistd.h>
#include <src/protocol/normal/meta/a_string.pb.h>
#include "protocol/normal/headers/server_bootstrap.h"
#include "protocol/normal/handler/headers/generic_handler.h"
#include "protocol/normal/headers/msg_router.h"
#include "util/headers/timestamp.h"
#include "util/headers/variant.h"
#include "protocol/normal/util/headers/serialize.h"

#define PORT 8080

using namespace std;

void client_thread_runner();
handlers_initializer handlers();

int main() {
    thread client_thread(client_thread_runner);
    server_bootstrap bootstrap(PORT);
    bootstrap.register_handlers({make_shared<generic_handler>()});
    bootstrap.start();
    return 0;
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

    base_req_msg req_msg;
    req_msg.set_timestamp(current_timestamp());
    req_msg.set_target("");
    auto buffer_token = serialize_as_buffer(req_msg);
    write(socketfd, buffer_token.buffer.get(), buffer_token.len);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    uint_8 r_buffer[1024];
    read(socketfd, r_buffer, sizeof (r_buffer));
    uint_64 msg_len = parse_variant(r_buffer);
    auto resp = parse_from_bytes<base_resp_msg>(r_buffer, msg_len);
    a_string a_str;
    a_str.ParseFromString(resp.body());
    printf("[response msg] code: %d timestamp: %llu msg: %s\n", resp.code(), resp.timestamp(), a_str.content().c_str());
}
