/*
 * join_handler.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_SERVER_JOIN_HANDLER_H_
#define SRC_SERVER_JOIN_HANDLER_H_

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

/**
 * join_handler
 *
 * function that adds a new user to the
 * client_identity list, then sends a
 * PARTICIPANT message to the new user and
 * finally sends a PJOIN message to all
 * other users on the server.
 *
 * @param pdu struct pdu_receive, join pdu upon to send a pjoin
 * @param communicator struct com, communicator to handle a join message
 */
int join_handler(pdu* pdu_receive, communicator *com);

#endif /* SRC_SERVER_JOIN_HANDLER_H_ */
