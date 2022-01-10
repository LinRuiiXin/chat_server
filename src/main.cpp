#include <iostream>
#include "net/headers/server_socket.h"
#include "filter/headers/filter_chain.h"

using std::cout;
using std::endl;

void message_protocol_filter(server_connect &, shared_ptr<void>, filter_chain &);

int main() {
    server_socket socket(8080);
    socket.set_filter_chain({message_protocol_filter});
    socket.start();
    return 0;
}

void message_protocol_filter(server_connect &connect, shared_ptr<void> arg, filter_chain &filters) {
    cout << "message_protocol_filter invoke..." << endl;
    auto &in_buffer = connect.in();
    int counter = 1;
    for(auto it = in_buffer.begin(); it != in_buffer.end(); ++it) {
        if(*it == EOF) {
            unique_ptr<char[]> protocol_buffer = in_buffer.read(counter);
        }
        ++counter;
    }

}