//
// Created by knig on 2017-10-01.
//

#ifndef CHAT_PDU_PARSER_GENERATOR_SOCKET_ABSTRACTION_H
#define CHAT_PDU_PARSER_GENERATOR_SOCKET_ABSTRACTION_H

#include <stdint.h>
#include "message_byte_array.h"

typedef struct io_handler{

    message_byte_array io_buffer;

    //functionpointer init  build connection function
    //functionpointer read n words
    //functionpointer write
}io_handler;
#endif //CHAT_PDU_PARSER_GENERATOR_SOCKET_ABSTRACTION_H
