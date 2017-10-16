/*
 * pleave_handler.h
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
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


int send_pleave(pdu *pdu_receive, communicator *com);

#endif /* SRC_SERVER_PLEAVE_HANDLER_H_ */
