/*
 * client.h
 *
 *  Created on: Oct 11, 2017
 *      Author: lgerber
 */

#ifndef SRC_CLIENT_CLIENT_H_
#define SRC_CLIENT_CLIENT_H_
#include <stdio.h>
#include <socket_templates.h>
#include <socket_creator.h>
#include <linked_list.h>

typedef struct chat_server{
	char server_name[255];
	char address[255];
	uint16_t port;
}chat_server;

typedef struct current_user{
	chat_server* join_server;
    char identity[255];
    int server_type; //0 name server, 1 chatserver
}current_user;

typedef struct client{
	int myId;
} client;

void get_list_to_user(pdu* slist, list* servers);

void print_welcome(void);

list* request_chat_servers();

int chat_loop(current_user *server);

#endif /* SRC_CLIENT_CLIENT_H_ */
