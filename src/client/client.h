/*
 * client.h
 *
 *  Created on: Oct 11, 2017
 *      Author: lgerber
 */

#ifndef SRC_CLIENT_CLIENT_H_
#define SRC_CLIENT_CLIENT_H_

#define TYPE_CHAT_SERVER 1
#define TYPE_NAME_SERVER 0

#define JOIN_INITIAL -2
#define JOIN_FAIL -1
#define JOIN_SUCCESS 0

#define NAME_SERVER "itchy.cs.umu.se"
#define NAME_SERVER_PORT 1337

#include <stdio.h>
#include <ctype.h>
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
	chat_server* name_server;
    char identity[255];
    int server_type;
	int join_status;
}current_user;

typedef struct client{
	int myId;
} client;

void get_list_to_user(pdu* slist, list* servers);

void print_main(void);

list* request_chat_servers(current_user* u, list* server_list);

int chat_loop(current_user *server);

int parse_arguments(int argc, char *argv[], current_user* u);

void print_usage_error(void);

void print_help(void);

int join_server_in_list(current_user* user, char* input, list* servers);

int direct_connect(current_user* user, const char* input);

int set_name_server(current_user* user, const char* input);

#endif /* SRC_CLIENT_CLIENT_H_ */
