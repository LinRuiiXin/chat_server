#include "headers/generic_handler.h"
#include <src/protocol/normal/meta/a_string.pb.h>

string generic_handler::handler(server_connect &connect, base_req_msg &msg) {
    a_string a_str;
    a_str.set_content("GENERIC HANDLER");
    return a_str.SerializeAsString();
}
