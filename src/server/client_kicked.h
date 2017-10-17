/*
 * client_kicked.h
 *
 *  Created on: Oct 17, 2017
 *      Author: lgerber
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

int client_kicked(communicator *com);

#endif /* SRC_SERVER_CLIENT_KICKED_H_ */
