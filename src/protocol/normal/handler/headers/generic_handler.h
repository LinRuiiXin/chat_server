#ifndef CHAT_SERVER_GENERIC_HANDLER_H
#define CHAT_SERVER_GENERIC_HANDLER_H

#include "../../headers/msg_handler.h"

class generic_handler: public msg_handler {

public:
    generic_handler(): msg_handler("/"){};
    string handler(server_connect &connect, base_req_msg &msg) override;

private:

};


#endif //CHAT_SERVER_GENERIC_HANDLER_H
