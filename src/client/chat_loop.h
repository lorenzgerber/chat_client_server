/*
 * chat_loop.h
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */

#ifndef SRC_CLIENT_CHAT_LOOP_H_
#define SRC_CLIENT_CHAT_LOOP_H_
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "client.h"


void *sendThread(void *data);

#endif /* SRC_CLIENT_CHAT_LOOP_H_ */
