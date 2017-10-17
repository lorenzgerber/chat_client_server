/*
 * chat_loop.h
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */

#ifndef SRC_CLIENT_CHAT_LOOP_H_
#define SRC_CLIENT_CHAT_LOOP_H_
#define _GNU_SOURCE
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "client.h"


void *sendThread(void *data);

#endif /* SRC_CLIENT_CHAT_LOOP_H_ */
