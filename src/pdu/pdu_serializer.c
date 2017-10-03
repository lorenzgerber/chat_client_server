#include "pdu_serializer.h"



message_byte_array* reg_create_message(pdu *self){
	int length = get_length_reg(self);
    message_byte_array* message = create_message_byte_array(length);
    message->add_uint8(message, self->type);
    message->add_uint8(message, self->server_name_length);
    message->add_uint16(message, self->tcp_port);
    message->add_chars(message, self->server_name, self->server_name_length);
    int padding = calc_word_padding(self->server_name_length);
    for(int i = 0; i < padding; i++){
    	message->add_uint8(message, 0);
    }


	return message;
}


message_byte_array* alive_create_message(pdu *self){
	int length = get_length_alive(self);
	message_byte_array* message = create_message_byte_array(length);
	message->add_uint8(message, self->type);
	message->add_uint8(message, self->number_clients);
	message->add_uint16(message, self->id_number);
	return message;
}



message_byte_array* ack_create_message(pdu *self){
	int length = get_length_ack(self);
	message_byte_array* message = create_message_byte_array(length);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message, self->id_number);
	return message;
}




message_byte_array* notreg_create_message(pdu *self){
	int length = get_length_notreg(self);
	message_byte_array* message = create_message_byte_array(length);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message, self->id_number);
	return message;
}



message_byte_array* getlist_create_message(pdu *self){
	message_byte_array* message = create_message_byte_array(4);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message,0);
	return message;
}



message_byte_array* slist_create_message(pdu *self){
	int length = get_length_slist(self);

	// build the Header
	message_byte_array* message = create_message_byte_array(length);
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



message_byte_array* join_create_message(pdu *self){
	int length = get_length_join(self);
	message_byte_array* message = create_message_byte_array(length);
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


message_byte_array* participants_create_message(pdu *self){
	int length = get_length_participants(self);
	message_byte_array* message = create_message_byte_array(length);
	message->add_uint8(message, self->type);
	message->add_uint8(message, self->number_identities);
	message->add_uint16(message, self->length);
	for (int i = 0; i < self->number_identities;i++){
		message->add_chars(message, self->identities[i], strlen(self->identities[i])+1);
	}
	int padding = calc_word_padding(self->length);
	for (int i = 0; i < padding; i++){
		message->add_uint8(message, 0);
	}
	return message;
}






message_byte_array* pdu_quit_create_message(pdu_QUIT *self){
	int length = get_length_QUIT(self);
	message_byte_array* message = create_message_byte_array(length);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message,0);
	return message;
}

message_byte_array* quit_create_message(pdu *self){
	int length = get_length_quit(self);
	message_byte_array* message = create_message_byte_array(length);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint16(message,0);
	return message;
}





message_byte_array* pdu_mess_create_message(pdu_MESS *self){
	int length = get_length_MESS(self);
	message_byte_array* message = create_message_byte_array(length);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint8(message, self->identity_length);
	message->add_uint8(message, self->checksum);
	message->add_uint16(message, self->message_length);
	message->add_uint16(message, 0);
	message->add_uint32(message, self->time_stamp);
	message->add_chars(message, self->message, self->message_length);

	int padding = calc_word_padding(self->message_length);
	for(int i = 0; i < padding; i++){
		message->add_uint8(message, 0);
	}

	if(self->identity_length > 0){
		message->add_chars(message, self->client_identity, self->identity_length);
		int padding = calc_word_padding(self->identity_length);
		for(int i = 0; i < padding; i++){
			message->add_uint8(message, 0);
		}
	}
	return message;
}

message_byte_array* mess_create_message(pdu *self){
	int length = get_length_mess(self);
	message_byte_array* message = create_message_byte_array(length);
	message->add_uint8(message, self->type);
	message->add_uint8(message, 0);
	message->add_uint8(message, self->identity_length);
	message->add_uint8(message, self->checksum);
	message->add_uint16(message, self->message_length);
	message->add_uint16(message, 0);
	message->add_uint32(message, self->time_stamp);
	message->add_chars(message, self->message, self->message_length);

	int padding = calc_word_padding(self->message_length);
	for(int i = 0; i < padding; i++){
		message->add_uint8(message, 0);
	}

	if(self->identity_length > 0){
		message->add_chars(message, self->identity, self->identity_length);
		int padding = calc_word_padding(self->identity_length);
		for(int i = 0; i < padding; i++){
			message->add_uint8(message, 0);
		}
	}
	return message;
}




message_byte_array* pdu_pjoin_create_message(pdu_PJOIN *self){
	int length = get_length_PJOIN(self);
	message_byte_array* message = create_message_byte_array(length);
    message->add_uint8(message, self->type);
	message->add_uint8(message, self->identity_length);
	message->add_uint16(message, 0);
	message->add_uint32(message, self->time_stamp);
	message->add_chars(message, self->client_identity, self->identity_length);
	int padding = calc_word_padding(self->identity_length);
	for(int i = 0; i < padding; i++){
		message->add_uint8(message, 0);
	}
	return message;
}

message_byte_array* pjoin_create_message(pdu *self){
	int length = get_length_pjoin(self);
	message_byte_array* message = create_message_byte_array(length);
    message->add_uint8(message, self->type);
	message->add_uint8(message, self->identity_length);
	message->add_uint16(message, 0);
	message->add_uint32(message, self->time_stamp);
	message->add_chars(message, self->identity, self->identity_length);
	int padding = calc_word_padding(self->identity_length);
	for(int i = 0; i < padding; i++){
		message->add_uint8(message, 0);
	}
	return message;
}





message_byte_array* pdu_pleave_create_message(pdu_PLEAVE *self){
	int length = get_length_PLEAVE(self);
	message_byte_array* message = create_message_byte_array(length);
    message->add_uint8(message, self->type);
	message->add_uint8(message, self->identity_length);
	message->add_uint16(message, 0);
	message->add_uint32(message, self->time_stamp);
	message->add_chars(message, self->client_identity, self->identity_length);
	int padding = calc_word_padding(self->identity_length);
	for(int i = 0; i < padding; i++){
		message->add_uint8(message, 0);
	}
	return message;
}

message_byte_array* pleave_create_message(pdu *self){
	int length = get_length_pleave(self);
	message_byte_array* message = create_message_byte_array(length);
    message->add_uint8(message, self->type);
	message->add_uint8(message, self->identity_length);
	message->add_uint16(message, 0);
	message->add_uint32(message, self->time_stamp);
	message->add_chars(message, self->identity, self->identity_length);
	int padding = calc_word_padding(self->identity_length);
	for(int i = 0; i < padding; i++){
		message->add_uint8(message, 0);
	}
	return message;
}

