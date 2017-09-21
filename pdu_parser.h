/*
 * pdu_parser.h
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */

#ifndef PDU_PARSER_H_
#define PDU_PARSER_H_
#include "pdu_templates.h"
#include "message_byte_array.h"

message_byte_array* pdu_reg_create_message(pdu_REG *self);

#endif /* PDU_PARSER_H_ */
