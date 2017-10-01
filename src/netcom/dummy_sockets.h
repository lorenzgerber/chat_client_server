/*
 * dummy_sockets.h
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */

#ifndef SRC_NETCOM_DUMMY_SOCKETS_H_
#define SRC_NETCOM_DUMMY_SOCKETS_H_
#include "pdu_creator.h"
#include "message_byte_array.h"
#include "socket_templates.h"

io_handler* dummy_socket_mess(io_handler * dummy_socket);

io_handler* dummy_socket_ack(io_handler* dummy_socket);

io_handler* dummy_socket_notreg(io_handler* dummy_socket);

io_handler* dummy_socket_slist(io_handler* dummy_socket);

io_handler* dummy_socket_join(io_handler* dummy_socket);

io_handler* dummy_socket_participants(io_handler* dummy_socket);

#endif /* SRC_NETCOM_DUMMY_SOCKETS_H_ */
