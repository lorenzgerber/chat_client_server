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
#define RESET "\x1B[0m"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "client.h"

void *sendThread(void *data);

#endif /* SRC_CLIENT_CHAT_LOOP_H_ */
