/*
 * cmd_args.c
 *
 *  Created on: Oct 17, 2017
 *      Author: lgerber
 */

#include "cmd_args.h"


void server_usage_error(void){
    fprintf(stderr, "\nInvalid arguments.\n");
    fprintf(stderr, "Correct usage: server PORT SERVERNAME NAMESERVER_HOST NAMESERVER_PORT");
}

void invalid_port(void){
	fprintf(stderr, "\nInvalid port.\n");
}

int check_cmd_args(int argc, char*argv[]){

	char *next;

	if(argc < 5){
		server_usage_error();
		exit(EXIT_FAILURE);
	}

	// Check whether port args are numbers
	for(int i = 1; i < 5; i += 3){
		strtol (argv[i], &next, 10);
		if ((next == argv[i]) || (*next != '\0')) {
			invalid_port();
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
