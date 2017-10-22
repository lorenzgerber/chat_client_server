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

/**
 * send_pleave
 *
 * Function that sends out pleave messages
 * to all active clients. In case the server
 * itself announces quit, the flat server_message
 * should be set and no client_name will be added
 * to the messages.
 * @param pdu_receive, either quit message from client
 * or empty pdu in case of server message
 * @param com communicator struct, the communicator thread to
 * send the message
 * @param server_message int 0 = normal client quit, 1 = server message
 */
int send_pleave(pdu *pdu_receive, communicator *com, int server_message);

#endif /* SRC_SERVER_PLEAVE_HANDLER_H_ */
