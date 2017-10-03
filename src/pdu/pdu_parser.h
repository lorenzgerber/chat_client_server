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

pdu* parse_header(struct io_handler* socket);

pdu* parse_ACK(struct io_handler* socket, uint8_t* read_position);

pdu* parse_NOTREG(struct io_handler* socket, uint8_t* read_position);

pdu* parse_SLIST(struct io_handler* socket, uint8_t* read_position);

pdu* parse_JOIN(struct io_handler* socket, uint8_t* read_position);

pdu* parse_PARTICIPANTS(struct io_handler* socket, uint8_t* read_position);

int parse_QUIT(struct io_handler* socket, uint8_t* read_position);

int parse_MESS(struct io_handler* socket, uint8_t *read_position);

int parse_PJOIN(struct io_handler* socket, uint8_t* read_position);

int parse_PLEAVE(struct io_handler* socket, uint8_t* read_position);

#endif /* PDU_PARSER_H_ */
