/*
 * communicator.h
 *
 *  Created on: Oct 15, 2017
 *      Author: lgerber
 */

#ifndef SRC_SERVER_COMMUNICATOR_H_
#define SRC_SERVER_COMMUNICATOR_H_





#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "server.h"
#include "pdu_parser.h"
#include "pdu_creator.h"
#include "socket_creator.h"
#include "listener.h"
#include "participant_list_handler.h"
#include "join_handler.h"
#include "pleave_handler.h"
#include "mess_handler.h"
#include "client_kicked.h"


void * com_loop(void* data);
int shutdown_connection(communicator *com);



#endif /* SRC_SERVER_COMMUNICATOR_H_ */
