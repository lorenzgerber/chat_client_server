/*
 * client_connect.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef CHAT_PDU_PARSER_GENERATOR_CLIENT_CONNECT_H
#define CHAT_PDU_PARSER_GENERATOR_CLIENT_CONNECT_H

#include "client.h"
#include "chat_loop.h"



list* request_chat_servers(current_user* u, list* server_list);

int join_server_in_list(current_user* user, char* input, list* servers);

int direct_connect(current_user* user, const char* input);

#endif //CHAT_PDU_PARSER_GENERATOR_CLIENT_CONNECT_H
