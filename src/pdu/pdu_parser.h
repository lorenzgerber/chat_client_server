/*
 * pdu_parser.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef PDU_PARSER_H_
#define PDU_PARSER_H_

#include <string.h>
#include <arpa/inet.h>

#include "pdu_templates.h"
#include "pdu_creator.h"
#include "socket_templates.h"

/**
 * parse_header parses the the stream into a pdu object. Initially it reads
 * only the first word to determine what parsing function to call. There is one
 * such function for each pdu.
 *
 * @param socket io_handler, The struct containing the read stream.
 * @return pdu , the parsed pdu.
 */
pdu* parse_header(struct io_handler* socket);

pdu* parse_ACK(struct io_handler* socket);

pdu* parse_NOTREG(struct io_handler* socket);

pdu* parse_SLIST(struct io_handler* socket);

pdu* parse_JOIN(struct io_handler* socket);

pdu* parse_PARTICIPANTS(struct io_handler* socket);

pdu* parse_QUIT(struct io_handler* socket);

pdu* parse_MESS(struct io_handler* socket);

pdu* parse_PJOIN(struct io_handler* socket);

pdu* parse_PLEAVE(struct io_handler* socket);

#endif /* PDU_PARSER_H_ */
