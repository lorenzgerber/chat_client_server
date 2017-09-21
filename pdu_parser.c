#include <stdio.h>
#include "pdu_parser.h"
#include "message_byte_array.h"


message_byte_array* pdu_reg_create_message(pdu_REG *self){
	//todo

    message_byte_array* message = create_message_byte_array(12);
    message->add_uint8(message, self->type);
    message->add_uint8(message, self->server_name_length);
    message->add_uint16(message, self->tcp_port);
    message->add_chars(message, self->server_name, 4);


	return message;
}

message_byte_array* pdu_alive_create_message(pdu_ALIVE *self){
	message_byte_array* message = create_message_byte_array(4);
	message->add_uint8(message, self->type);
	message->add_uint8(message, self->number_clients);
	message->add_uint16(message, self->id_number);
	return message;
}

message_byte_array* pdu_ack_create_message(pdu_ACK *self){
	message_byte_array* message = create_message_byte_array(4);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message, self->id_number);
	return message;
}

message_byte_array* pdu_notreg_create_message(pdu_NOTREG *self){
	message_byte_array* message = create_message_byte_array(4);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message, self->id_number);
	return message;
}

message_byte_array* pdu_getlist_create_message(pdu_GETLIST *self){
	message_byte_array* message = create_message_byte_array(4);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message,0);
	return message;
}

message_byte_array* pdu_slist_create_message(pdu_SLIST *self){

	// build the Header
	message_byte_array* message = create_message_byte_array(4);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message, self->number_servers);

	// Iterate over the server list
	for (int i = 0; i < self->number_servers; i++){

		// iterate over the TCP address array
		for(int j = 0; j < 4; j++){
			message->add_uint8(message, self->current_servers[i]->address[j]);
		}

		// Port, number of clients, server name length, and server name
		message->add_uint16(message, self->current_servers[i]->port);
		message->add_uint8(message, self->current_servers[i]->number_clients);
		message->add_uint8(message, self->current_servers[i]->name_length);
		message->add_chars(message, self->current_servers[i]->name, self->current_servers[i]->name_length);

		// calculate and apply padding for server name
		int padding = 4 - (self->current_servers[i]->name_length % 4);

		for(int k = 0; k < padding; k++){
			message->add_uint8(message, 0);
		}
	}

	return message;
}

message_byte_array* pdu_join_create_message(pdu_JOIN *self){
	message_byte_array* message = create_message_byte_array(4);
	message->add_uint8(message, self->type);
	message->add_uint8(message, self->identity_length);
	message->add_uint16(message, 0);
	message->add_chars(message, self->identity, self->identity_length);

	int padding = 4 -(self->identity_length % 4);
	for (int i = 0; i < padding; i++){
		message->add_uint8(message, 0);
	}

	return message;
}

message_byte_array* pdu_participants_create_message(pdu_PARTICIPANTS *self){
	message_byte_array* message = create_message_byte_array(4);
	//todo
	return message;
}

message_byte_array* pdu_quit_create_message(pdu_QUIT *self){
	message_byte_array* message = create_message_byte_array(4);
	//todo
	return message;
}

message_byte_array* pdu_mess_create_message(pdu_MESS *self){
	message_byte_array* message = create_message_byte_array(4);
	//todo
	return message;
}

message_byte_array* pdu_pjoin_create_message(pdu_PJOIN *self){
	message_byte_array* message = create_message_byte_array(4);
	//todo
	return message;
}

message_byte_array* pdu_pleave_create_message(pdu_PLEAVE *self){
	message_byte_array* message = create_message_byte_array(4);
	//todo
	return message;
}
