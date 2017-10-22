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

int ceck_cmd_args(int argc, char* argv[]);
void server_usage_error(void);
void invalid_port(void);

#endif /* SRC_SERVER_CMD_ARGS_H_ */
