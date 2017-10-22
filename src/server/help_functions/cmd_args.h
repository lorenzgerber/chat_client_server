/*
 * cmd_args.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_SERVER_CMD_ARGS_H_
#define SRC_SERVER_CMD_ARGS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * server_usage_error
 *
 * Function to print a message of the
 * correct command line args to stderrr.
 */
int ceck_cmd_args(int argc, char* argv[]);

/**
 * invalid_port
 *
 * Function to print a message that the
 * provided commandline arg 'port' was
 * invalid.
 */
void server_usage_error(void);

/**
 * check_cmd_args
 *
 * Function that checks for the correct number
 * of command line args. Further, it checks
 * that the args for port can be parsed as numeric.
 */
void invalid_port(void);

#endif /* SRC_SERVER_CMD_ARGS_H_ */
