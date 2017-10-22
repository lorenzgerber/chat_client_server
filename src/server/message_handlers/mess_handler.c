/*
 * mess_handler.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "../message_handlers/mess_handler.h"

/**
 * mess_handler
 *
 * This function composes the MESS pdu to be
 * sent to all clients upon reception of a MESS pdu
 * from one client.
 * @param pdu_receive pdu, the incoming message to react upon
 * @param com communicator struct of the communicator thread
 * that received the message
 */
int mess_handler(pdu* pdu_receive, communicator *com){

	pdu *mess = create_mess(pdu_receive->identity_length, 0);
	mess->add_message(mess,
			pdu_receive->message_length,
			time(NULL),
			pdu_receive->message);
	mess->add_identity(mess, com->client_name);
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
	mess->free_pdu(mess);

	return 0;
}

