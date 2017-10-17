/*
 * pleave_handler.c
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */
#include "pleave_handler.h"


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

