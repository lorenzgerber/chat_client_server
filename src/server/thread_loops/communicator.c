/*
 * commiunicator.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */
#include "communicator.h"

/**
 * com_loop
 *
 * This function is run in a separate
 * thread and contains the main client/server
 * communication code. It takes as argument a
 * pointer to a communicator struct.
 *
 * The whole function is contained in a while loop
 * that depends on a variable in the main server
 * struct. The function contains a conditional lock
 * to prevent busy waiting while there is no client
 * assigned to the current thread.
 *
 * When active, the function runs through several
 * stages of conditionals to handle JOIN, QUIT and
 * MESS pdu's.
 * @param data a communication struct
 */
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
					if(join_handler(pdu_receive, com) < 0){
						shutdown_connection(com);
						com->joined = 0;
					}

				// if client sends JOIN while already joined, kick out
				} else if(pdu_receive->type == 12 && com->joined == 1){
					printf("client sends second join while already joined\n");
					com->joined = 0;
					//send_pleave(pdu_receive, com, 1);
					client_kicked(com);
					shutdown_connection(com);


				} else if(pdu_receive->type == 11){
					// QUIT, close connection and send PLEAVE to all others
					com->joined = 0;
					send_pleave(pdu_receive, com, 0);
					shutdown_connection(com);


				} else if(pdu_receive->type == 10 && com->joined == 1){
					// here we check for MESS message
					if(pdu_receive->verify_checksum(pdu_receive)!=0){
						//Checksum incorrect
						printf("checksum incorrect!\n Shutting down client\n");
						com->joined = 0;
						client_kicked(com);
						//send_pleave(pdu_receive, com, 1);
						shutdown_connection(com);

					} else {
						mess_handler(pdu_receive, com);
					}
				} else {
					// unrecognized / wrong message. shutting down
					com->joined = 0;
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
				com->joined = 0;
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

/**
 * shutdown_connection
 *
 * Function to close down the current connection of
 * the communication thread. This function frees
 * up the resources of the thread. However the
 * thread keeps running and will go to sleep in
 * the conditional lock until it gets waken up
 * from the listener.
 * @param communicator struct of the connection to be closed
 */
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
	if(com->client_name != NULL){
		remove_identity(com->client_list, com->client_name);
	}
	// update number of clients
	uint8_t number_identities = get_number_identities(com->client_list);
	*com->number_of_clients = number_identities;
	pthread_mutex_unlock(com->client_list_lock);

	free(com->client_name);

	return 0;
}



