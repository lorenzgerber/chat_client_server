/*
 * socket_creator.h
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */

#ifndef SRC_NETCOM_SOCKET_CREATOR_H_
#define SRC_NETCOM_SOCKET_CREATOR_H_

#include "socket_templates.h"

io_handler* create_dummy_socket(int op_code);

uint8_t* dummy_socket_request_n_word(struct io_handler *self);

#endif /* SRC_NETCOM_SOCKET_CREATOR_H_ */
