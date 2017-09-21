#include <stdio.h>
#include "pdu_parser.h"
#include "message_byte_array.h"


message_byte_array* pdu_reg_create_message(pdu_REG *self){

    message_byte_array* message = create_message_byte_array(8);
    message->add_uint8(message, self->type);
    message->add_uint8(message, self->server_name_length);
    message->add_uint16(message, self->tcp_port);
    message->add_chars(message, self->server_name, 4);


	return message;
}

