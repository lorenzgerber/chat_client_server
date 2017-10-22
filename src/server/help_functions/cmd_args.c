/*
 * cmd_args.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "cmd_args.h"

/**
 * server_usage_error
 *
 * Function to print a message of the
 * correct command line args to stderrr.
 */
void server_usage_error(void){
    fprintf(stderr, "\nInvalid arguments.\n");
    fprintf(stderr, "Correct usage: server PORT SERVERNAME NAMESERVER_HOST NAMESERVER_PORT\n");
}

/**
 * invalid_port
 *
 * Function to print a message that the
 * provided commandline arg 'port' was
 * invalid.
 */
void invalid_port(void){
	fprintf(stderr, "\nInvalid port.\n");
}


/**
 * check_cmd_args
 *
 * Function that checks for the correct number
 * of command line args. Further, it checks
 * that the args for port can be parsed as numeric.
 */
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
