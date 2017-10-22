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

#include "../message_handlers/join_handler.h"
#include "../message_handlers/pleave_handler.h"
#include "server.h"
#include "pdu_parser.h"
#include "pdu_creator.h"
#include "socket_creator.h"
#include "listener.h"
#include "participant_list_handler.h"

/**
 * mess_handler
 *
 * This function composes the MESS pdu to be
 * sent to all clients upon reception of a MESS pdu
 * from one client.
 * @param pdu_receive pdu, the incoming message to react upon
 * @param com communicator struct of the communicator thread
 * that received the message
 */
int mess_handler(pdu* pdu_receive, communicator *com);


#endif /* SRC_SERVER_MESS_HANDLER_H_ */
