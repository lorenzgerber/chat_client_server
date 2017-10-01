/*
 * pdu_parser.h
 *
 *  Created on: Sep 30, 2017
 *      Author: lgerber
 */

#ifndef PDU_PARSER_H_
#define PDU_PARSER_H_

#include <string.h>
#include "pdu_templates.h"

int parse_header(uint8_t* buffer);

int parse_REG(uint8_t *buffer);

int parse_MESS(uint8_t *buffer);

#endif /* PDU_PARSER_H_ */
