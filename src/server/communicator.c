/*
 * communicator.c
 *
 *  Created on: Oct 15, 2017
 *      Author: lgerber
 */
#include "communicator.h"


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

					// handling JOIN messages
					join_handler(pdu_receive, com);

				// if client sends JOIN while already joined, kick out
				} else if(pdu_receive->type == 12 && com->joined == 1){
					printf("client sends second join while already joined\n");
					//send_pleave(pdu_receive, com, 1);
					client_kicked(com);
					shutdown_connection(com);

				} else if(pdu_receive->type == 11){
					// QUIT, close connection and send PLEAVE to all others
					send_pleave(pdu_receive, com, 0);
					shutdown_connection(com);

				} else if(pdu_receive->type == 10 && com->joined == 1){
					// here we check for MESS message
					if(pdu_receive->verify_checksum(pdu_receive)!=0){
						//Checksum incorrect
						printf("checksum incorrect!\n Shutting down client\n");
						client_kicked(com);
						//send_pleave(pdu_receive, com, 1);
						shutdown_connection(com);
					} else {
						mess_handler(pdu_receive, com);
					}
				} else {
					// unrecognized / wrong message. shutting down
					client_kicked(com);
					//send_pleave(pdu_receive, com, 1);
					shutdown_connection(com);
				}

				pdu_receive->free_pdu(pdu_receive);
				pdu_receive = NULL;

			} else if (com->handler->status != STATUS_RECEIVE_EMPTY){
				// here we handle connections that were
				// terminated without notice
				printf("We should probably shut this one down\n");
				client_kicked(com);
				shutdown_connection(com);
			}
		}
	}


	if(com->handler != NULL){
		// send quit to client
		pdu* pdu_quit = create_quit();
		com->handler->send_pdu(com->handler, pdu_quit);
		pdu_quit->free_pdu(pdu_quit);

		// clean up
		free_tcp_server_communicator(com->handler);
		if(com->client_name != NULL){
			free(com->client_name);
		}
	}

    return NULL;
}


int shutdown_connection(communicator *com){
	// we should kill the client
	com->handler->close(com->handler);
	if(com->handler->buffer != NULL){
		free_message_byte_array(com->handler->buffer);
	}
	pthread_mutex_lock(com->handler_lock);
	free_tcp_server_communicator(com->handler);
	com->handler = NULL;
	pthread_mutex_unlock(com->handler_lock);
	com->joined = 0;

	// remove identity from participants list
	pthread_mutex_lock(com->client_list_lock);
	remove_identity(com->client_list, com->client_name);
	// update number of clients
	uint8_t number_identities = get_number_identities(com->client_list);
	*com->number_of_clients = number_identities;
	pthread_mutex_unlock(com->client_list_lock);

	free(com->client_name);

	return 0;
}



