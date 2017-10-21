/*
 * listener.c
 *
 *  Created on: Oct 15, 2017
 *      Author: lgerber
 */

#include "listener.h"



void * listen_loop(void* data){

	server *server = data;
	char* address = server->our_host;
	server->listener = create_tcp_server_listener(address, server->our_port);
	io_handler *new_com;

	// listener
	while(*server->bail_out != 1){
		int assigned = 0;
		new_com = NULL;
		new_com = server->listener->listen(server->listener);

		// implement here transfer of io_handler to com_array
		if(*server->bail_out != 1){

			for(int i = 0; i < NUMBER_HANDLERS && assigned == 0; i++){
				pthread_mutex_lock(&server->com_mutex[i]);
				if(server->com_array[i].handler == NULL){
					server->com_array[i].handler = new_com;
					assigned = 1;
				}
				pthread_mutex_unlock(&server->com_mutex[i]);
			}

			// if iterated over whole array and still not assigned
			if(assigned == 0){
				printf("no more free client slots on server\n");
				new_com->close(new_com);
			}

			// after transfer - signal all threads
			pthread_cond_broadcast(&cond_var);

		}
	}

	// clean up in case of shutdown
	free_tcp_server_listener(server->listener);
	free_tcp_server_communicator(new_com);
	printf("stop listening\n");
	return NULL;
}
