/*
 * pleave_handler.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */
#include "../message_handlers/pleave_handler.h"

/**
 * send_pleave
 *
 * Function that sends out pleave messages
 * to all active clients. In case the server
 * itself announces quit, the flat server_message
 * should be set and no client_name will be added
 * to the messages.
 * @param pdu_receive, either quit message from client
 * or empty pdu in case of server message
 * @param com communicator struct, the communicator thread to
 * send the message
 * @param server_message int 0 = normal client quit, 1 = server message
 */
int send_pleave(pdu *pdu_receive, communicator *com, int server_message){
	pdu* pdu_response;
	// prepare and send PLEAVE to all other clients
	if(server_message){
		pdu_response = create_pleave(0);
		pdu_response->add_client_identity_timestamp(pdu_response, time(NULL), NULL);

	} else {
		pdu_response = create_pleave(pdu_receive->identity_length);
		pdu_response->add_client_identity_timestamp(pdu_response, time(NULL), com->client_name);
	}


	// cycle through all io_handlers
	for(int i = 0; i < NUMBER_HANDLERS; i++){
		pthread_mutex_lock(com->com_array[i].handler_lock);
		if(com->com_array[i].handler != NULL){
			if(i != com->thread_id){
				com->com_array[i].handler->send_pdu(com->com_array[i].handler, pdu_response);
				printf("\nsent on %d\n", i);
			}
		}
		pthread_mutex_unlock(com->com_array[i].handler_lock);
	}
	pdu_response->free_pdu(pdu_response);

	return 0;
}

