/*
 * client.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_CLIENT_CLIENT_H_
#define SRC_CLIENT_CLIENT_H_

#define TYPE_CHAT_SERVER 1
#define TYPE_NAME_SERVER 0

#define JOIN_STATUS_INITIAL -2
#define JOIN_STATUS_QUIT -1
#define JOIN_STATUS_CONTINUE 0

#define ACTIVE 0
#define DONE 1
#define DONE_EXIT 2

#define NAME_SERVER "itchy.cs.umu.se"
#define NAME_SERVER_PORT 1337

#include <stdio.h>
#include <ctype.h>

#include "chat_loop.h"
#include "socket_templates.h"
#include "socket_creator.h"
#include "linked_list.h"

typedef struct current_user{
	struct chat_server* join_server;
	struct chat_server* name_server;
	char identity[255];
	int server_type;
	int join_status;
} current_user;

typedef struct chat_server{
	char server_name[255];
	char address[255];
	uint16_t port;
}chat_server;


typedef struct client{
	int myId;
} client;

typedef struct threadarg{
    io_handler *com;
    int* status;
} threadarg;

#include "client_command.h"
#include "client_connect.h"

int chat_loop(current_user *server);

#endif /* SRC_CLIENT_CLIENT_H_ */
