/*
 *  client_kicked.c
 *
 *
 *  Created on: Oct 1, 2017
 *  Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "client_kicked.h"


int client_kicked(communicator *com){

	if(com->client_name == NULL){
		return -1;
	}
	uint8_t identity_length = strlen(com->client_name);
	uint8_t length_kicked = 17;
	char* text = calloc(length_kicked + identity_length, sizeof(char));
	char* user = "user ";
	char* kicked = " was kicked\0";
	memcpy(text, user, 5);
	memcpy(text+5, com->client_name, identity_length);
	memcpy(text + (5+identity_length), kicked, 12);

	pdu *mess = create_mess(0, 0);
	mess->add_message(mess,
			identity_length + length_kicked,
			time(NULL),
			text);
	mess->set_checksum(mess);

	// cycle through all io_handlers
	for(int i = 0; i < NUMBER_HANDLERS; i++){
		pthread_mutex_lock(com->com_array[i].handler_lock);
		if(com->com_array[i].handler != NULL){
				com->com_array[i].handler->send_pdu(com->com_array[i].handler, mess);
				printf("\nsent on %d\n", i);
		}
		pthread_mutex_unlock(com->com_array[i].handler_lock);
	}
	free(text);
	mess->free_pdu(mess);

	return 0;


}

