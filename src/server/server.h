/*
 * server.h
 *
 *  Created on: Oct 11, 2017
 *      Author: lgerber
 */

#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#include "../data_structures/linked_list.h"
#include "socket_templates.h"

typedef struct server {
	io_handler *client_array[255];
	list *client_list;
	queue *message_list;
}server;


#endif /* SRC_SERVER_SERVER_H_ */
