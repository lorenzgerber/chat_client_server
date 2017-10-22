/*
 * client_kicked.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_SERVER_CLIENT_KICKED_H_
#define SRC_SERVER_CLIENT_KICKED_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "server.h"
#include "pdu_parser.h"
#include "pdu_creator.h"
#include "socket_creator.h"

/**
 * client_kicked
 *
 * Function that sends a 'kicked'
 * message to all clients except
 * the calling one (com).
 *
 * @param communicator com, the client to be kicked
 */
int client_kicked(communicator *com);

#endif /* SRC_SERVER_CLIENT_KICKED_H_ */
