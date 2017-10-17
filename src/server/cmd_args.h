/*
 * cmd_args.h
 *
 *  Created on: Oct 17, 2017
 *      Author: lgerber
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
