/*
 * pdu_parser.h
 *
 *  Created on: Sep 30, 2017
 *      Author: lgerber
 */

#ifndef PDU_PARSER_H_
#define PDU_PARSER_H_

#include <string.h>
#include <arpa/inet.h>
#include "pdu_templates.h"
#include "pdu_creator.h"
#include "socket_templates.h"

int parse_header(struct io_handler* socket);

int parse_REG(struct io_handler* socket);

int parse_MESS(struct io_handler* socket, uint8_t *read_position);

#endif /* PDU_PARSER_H_ */
