/*
 * dummy_sockets.c
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */
#include "dummy_sockets.h"

io_handler* dummy_socket_mess(io_handler * dummy_socket){

	pdu_MESS *MESS = create_MESS(8, 99);
	MESS->add_message(MESS, 13, 1505933137, "Test Message.");
	MESS->add_client_identity(MESS, "identity");
	dummy_socket->buffer = MESS->create_message(MESS);
	free_pdu_mess(MESS);

	return dummy_socket;
}


