/*
 * join_handler.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */
#include "join_handler.h"

int join_handler(pdu* pdu_receive, communicator *com){
	// add new client identity to list
	char * client_identity = malloc((pdu_receive->identity_length+1)*sizeof(char));
	strcpy(client_identity, pdu_receive->identity);
	pthread_mutex_lock(com->client_list_lock);
	list_insert(list_last(com->client_list), client_identity);
	pthread_mutex_unlock(com->client_list_lock);
	com->client_name = malloc((pdu_receive->identity_length+1)*sizeof(char));
	strcpy(com->client_name, pdu_receive->identity);


	// prepare and send participants to new user

	pthread_mutex_lock(com->client_list_lock);
	uint8_t number_identities = get_number_identities(com->client_list);
	*com->number_of_clients = number_identities;
	uint16_t length_identities = calc_length_identities(com->client_list);
	char *identities = build_identities(com->client_list, length_identities);
	pthread_mutex_unlock(com->client_list_lock);

	pdu * pdu_participants = create_participants(number_identities, length_identities);
	pdu_participants->add_identities(pdu_participants, identities);
	com->handler->send_pdu(com->handler, pdu_participants);
	pdu_participants->free_pdu(pdu_participants);
	free(identities);

	// prepare and send PJOIN to all other clients
	pdu* pdu_response = create_pjoin(pdu_receive->identity_length);
	pdu_response->add_client_identity_timestamp(pdu_response, time(NULL), pdu_receive->identity);


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



