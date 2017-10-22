/*
 * pleave_handler.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_SERVER_PLEAVE_HANDLER_H_
#define SRC_SERVER_PLEAVE_HANDLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "server.h"
#include "pdu_parser.h"
#include "pdu_creator.h"
#include "socket_creator.h"
#include "participant_list_handler.h"


int send_pleave(pdu *pdu_receive, communicator *com, int server_message);

#endif /* SRC_SERVER_PLEAVE_HANDLER_H_ */
