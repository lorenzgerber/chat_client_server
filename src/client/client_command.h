/*
 * client_command.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef CHAT_PDU_PARSER_GENERATOR_CLIENT_COMMAND_H
#define CHAT_PDU_PARSER_GENERATOR_CLIENT_COMMAND_H

#include "client.h"
#include <time.h>

int unix_to_localtime(uint32_t unix_time);

void print_main(void);

void print_usage_error(void);

void print_help(void);

int parse_arguments(int argc, char *argv[], current_user* u);

void get_list_to_user(pdu* slist, list* servers);

int set_name_server(current_user* user, const char* input);

#endif //CHAT_PDU_PARSER_GENERATOR_CLIENT_COMMAND_H
