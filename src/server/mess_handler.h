/*
 * mess_handler.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_SERVER_MESS_HANDLER_H_
#define SRC_SERVER_MESS_HANDLER_H_
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


int mess_handler(pdu* pdu_receive, communicator *com);


#endif /* SRC_SERVER_MESS_HANDLER_H_ */
