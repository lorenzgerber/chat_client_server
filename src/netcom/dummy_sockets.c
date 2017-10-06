/*
 * dummy_sockets.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */
#include "dummy_sockets.h"

/**
 * dummy_socket_ack
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the ack parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_ack(io_handler* dummy_socket){

	pdu* ACK = create_ack(4444);

	dummy_socket->buffer = ACK->create_message(ACK);
	free_ack(ACK);
	return dummy_socket;
}

/**
 * dummy_socket_notreg
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the notreg parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_notreg(io_handler* dummy_socket){
	pdu* NOTREG = create_notreg(5555);

	dummy_socket->buffer = NOTREG->create_message(NOTREG);
	free_notreg(NOTREG);

	return dummy_socket;
}


/**
 * dummy_socket_slist
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the slist parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_slist(io_handler* dummy_socket){

	pdu *SLIST = create_slist(2);
	uint8_t address[4] = {127,0,0,1};
	pdu_server_entry* server1 = create_server_entry(address,2000,4,10);
	server1->add_server_name(server1, "servername");
	SLIST->add_server_entry(SLIST, server1);

	uint8_t  address2[4] = {127,0,0,2};
	pdu_server_entry* server2 = create_server_entry(address2,2001,5,11);
	server2->add_server_name(server2, "servername2");
	SLIST->add_server_entry(SLIST, server2);

	dummy_socket->buffer = SLIST->create_message(SLIST);
	free_slist(SLIST);

	return dummy_socket;
}

/**
 * dummy_socket_join
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the join parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_join(io_handler* dummy_socket){

	pdu *JOIN = create_join(8);
	JOIN->add_identity(JOIN, "identity");

	dummy_socket->buffer = JOIN->create_message(JOIN);
	free_join(JOIN);

	return dummy_socket;
}

/**
 * dummy_socket_participants
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the participants parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_participants(io_handler* dummy_socket){

	pdu *PARTICIPANTS = create_participants(3, 15);
	PARTICIPANTS->add_identities(PARTICIPANTS, "partic\0ipa\0nts\0");

	dummy_socket->buffer = PARTICIPANTS->create_message(PARTICIPANTS);
	free_participants(PARTICIPANTS);

	return dummy_socket;
}

/**
 * dummy_socket_quit
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the quit parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_quit(io_handler* dummy_socket){

    pdu *QUIT = create_quit();

    dummy_socket->buffer = QUIT->create_message(QUIT);
    free_quit(QUIT);

    return dummy_socket;
}

/**
 * dummy_socket_mess
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the mess parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_mess(io_handler * dummy_socket){

    pdu *MESS = create_mess(8, 99);
    MESS->add_message(MESS, 13, 1505933137, "Test Message.");
    MESS->add_identity(MESS, "identity");  //changed from add_client_identity
    dummy_socket->buffer = MESS->create_message(MESS);
    free_mess(MESS);

    return dummy_socket;
}

/**
 * dummy_socket_pjoin
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the pjoin parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_pjoin(io_handler * dummy_socket){

	pdu *PJOIN = create_pjoin(8);
	PJOIN->add_client_identity_timestamp(PJOIN, 1505933137, "identity");

	dummy_socket->buffer = PJOIN->create_message(PJOIN);
	free_pjoin(PJOIN);

	return dummy_socket;
}

/**
 * dummy_socket_pleave
 *
 * Function to create a dummy socket for
 * testing the parsers. It takes a generic struct io_handler
 * as input argument and configures it to contain
 * data for testing the pleave parser.
 * @param dummy_socket struct io_handler
 * @return dummy_socket a configured io_handler
 *
 */
io_handler* dummy_socket_pleave(io_handler * dummy_socket){

	pdu *PLEAVE = create_pleave(8);
	PLEAVE->add_client_identity_timestamp(PLEAVE, 1505933137, "identity");

	dummy_socket->buffer = PLEAVE->create_message(PLEAVE);
	free_pleave(PLEAVE);

	return dummy_socket;
}

