/*
 * communicator.c
 *
 *  Created on: Oct 15, 2017
 *      Author: lgerber
 */
#include "communicator.h"

char* build_identities(list* name_list, uint16_t length_identities);
uint16_t calc_length_identities(list* name_list);
uint8_t get_number_identities(list* name_list);
int remove_identity(list *identity_list, char* identity);

void * com_loop(void* data){

	communicator *com = data;
	com->joined = 0;
	pdu* pdu_receive;
	pdu_receive = NULL;

	while(*com->bail_out == 0){

		// lock the cond mut
		pthread_mutex_lock(&cond_mutex);

		// check condition
		while(com->handler == NULL && *com->bail_out == 0){
			pthread_cond_wait(&cond_var, &cond_mutex);
		}
		pthread_mutex_unlock(&cond_mutex);


		if(*com->bail_out == 0){
			// receive and parse
			pdu_receive = parse_header(com->handler);
			if(com->handler->status == STATUS_RECEIVE_OK){
				pdu_receive->print(pdu_receive);
				com->handler->status = 0;

				// here we check for JOIN message
				if(pdu_receive->type == 12 && com->joined == 0){

					com->joined = 1;




					// add new client identity to list
					char * client_identity = malloc((pdu_receive->identity_length+1)*sizeof(char));
					strcpy(client_identity, pdu_receive->identity);
					list_insert(list_last(com->client_list), client_identity);
					com->client_name = malloc(pdu_receive->identity_length*sizeof(char));
					strcpy(com->client_name, pdu_receive->identity);

					// prepare and send participants to new user
					uint8_t number_identities = get_number_identities(com->client_list);
					uint16_t length_identities = calc_length_identities(com->client_list);
					char * identities = build_identities(com->client_list, length_identities);

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

						// lock handler for access and potential send
						pthread_mutex_lock(com->com_array[i].handler_lock);

						// check if io_handler is in use
						if(com->com_array[i].handler != NULL){

							// Don't send PJOIN to the joining client, though for testing at the moment
							// we send anyway
							if(i != com->thread_id){
								com->com_array[i].handler->send_pdu(com->com_array[i].handler, pdu_response);
								printf("\nsent on %d\n", i);
							}

						}
						pthread_mutex_unlock(com->com_array[i].handler_lock);
					}
					pdu_response->free_pdu(pdu_response);

				} else if(pdu_receive->type == 12 && com->joined == 1){
					printf("client sends second join while already joined\n");
					// we should kill the client

				}
				// here we check for QUIT message
				// todo
				// remove client identity from list

				// here we check for MESS message
				// todo

				pdu_receive->free_pdu(pdu_receive);
				pdu_receive = NULL;



			} else if (com->handler->status != STATUS_RECEIVE_EMPTY){
				// here we handle connections that were
				// terminated wihtout notice
				printf("We shoudld probably shut this one down\n");
				com->handler->close(com->handler);
				pthread_mutex_lock(com->handler_lock);
				free_tcp_server_communicator(com->handler);
				com->handler = NULL;
				pthread_mutex_unlock(com->handler_lock);
				com->joined = 0;
				remove_identity(com->client_list, com->client_name);
			}

		}

	}

	if(com->handler != NULL){
		free_tcp_server_communicator(com->handler);
	}

    return NULL;
}

uint8_t get_number_identities(list* name_list){
	int length = 0;
	uint16_t clients_count = 0;

	list_position current_position = list_first(name_list);

	// determine length
	if(!list_is_empty(name_list)){
		do{
			length += strlen((char*)list_inspect(current_position))+1;
			if(!list_is_end(name_list, current_position)){
				current_position = list_next(current_position);
				clients_count++;
			}
		} while (!list_is_end(name_list, current_position));
		clients_count++;
	}

	return clients_count;
}

uint16_t calc_length_identities(list* name_list){
	uint16_t length = 0;

	list_position current_position = list_first(name_list);

	// determine length
	if(!list_is_empty(name_list)){
		do{

			if(!list_is_end(name_list, current_position)){
				length += strlen((char*)list_inspect(current_position))+1;
				current_position = list_next(current_position);
			}
		} while (!list_is_end(name_list, current_position));
		length += strlen((char*)list_inspect(current_position))+1;
	}

	return length;

}


char* build_identities(list* name_list, uint16_t length_identities){
	uint16_t length = length_identities;
	int length_of_name;
	int str_position = 0;

	list_position current_position = list_first(name_list);


	// allocate memory, iterate and collect all names
	char* new_client = malloc(length * sizeof(char));
	current_position = list_first(name_list);


	if(!list_is_empty(name_list)){
		do{
			length_of_name = strlen((char *)list_inspect(current_position));
			memmove(&new_client[str_position], (char*) list_inspect(current_position), length_of_name);
			str_position += length_of_name+1;
			if(!list_is_end(name_list, current_position)){
				current_position = list_next(current_position);
			}

		} while (!list_is_end(name_list, current_position));
		length_of_name = strlen((char *)list_inspect(current_position));
		memmove(&new_client[str_position], (char*) list_inspect(current_position), length_of_name);
	}



	return new_client;
}

int remove_identity(list *identity_list, char* identity){

	list_position current_position = list_first(identity_list);

	if(!list_is_empty(identity_list)){
		do{
			if(strcmp(identity, (char*)list_inspect(current_position))==0){
				list_remove(identity_list, current_position);
				return 0;
			}

			if(!list_is_end(identity_list, current_position)){

				current_position = list_next(current_position);
			}
		} while (!list_is_end(identity_list, current_position));

		if(strcmp(identity, (char*)list_inspect(current_position))==0){
			list_remove(identity_list, current_position);
			return 0;
		}

	}

	return 0;
}

