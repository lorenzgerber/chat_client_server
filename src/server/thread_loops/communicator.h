/*
 * communicator.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
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

/**
 * com_loop
 *
 * This function is run in a separate
 * thread and contains the main client/server
 * communication code. It takes as argument a
 * pointer to a communicator struct.
 *
 * The whole function is contained in a while loop
 * that depends on a variable in the main server
 * struct. The function contains a conditional lock
 * to prevent busy waiting while there is no client
 * assigned to the current thread.
 *
 * When active, the function runs through several
 * stages of conditionals to handle JOIN, QUIT and
 * MESS pdu's.
 * @param data a communication struct
 */
void * com_loop(void* data);

/**
 * shutdown_connection
 *
 * Function to close down the current connection of
 * the communication thread. This function frees
 * up the resources of the thread. However the
 * thread keeps running and will go to sleep in
 * the conditional lock until it gets waken up
 * from the listener.
 * @param communicator struct of the connection to be closed
 */
int shutdown_connection(communicator *com);



#endif /* SRC_SERVER_COMMUNICATOR_H_ */
