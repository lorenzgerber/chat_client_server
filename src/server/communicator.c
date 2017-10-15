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

					// prepare and send participants to new user
					// todo
					// add new client identity to list


					// here we prepare the pjoined message
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
							//if(i != com->thread_id){
								com->com_array[i].handler->send_pdu(com->com_array[i].handler, pdu_response);
								printf("\nsent on %d\n", i);
							//}

						}
						pthread_mutex_unlock(com->com_array[i].handler_lock);
					}
					pdu_response->free_pdu(pdu_response);

				} else if(pdu_receive->type == 12 && com->joined == 1){
					printf("client sends second join while already joined\n");
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
			}

		}

	}

	if(com->handler != NULL){
		free_tcp_server_communicator(com->handler);
	}

    return NULL;
}

