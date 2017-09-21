/*
 * pdu_parser.h
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */

#ifndef PDU_PARSER_H_
#define PDU_PARSER_H_
#include <stdio.h>
#include "pdu_templates.h"
#include "message_byte_array.h"



message_byte_array* pdu_reg_create_message(pdu_REG *self);

message_byte_array* pdu_alive_create_message(pdu_ALIVE *self);

message_byte_array* pdu_ack_create_message(pdu_ACK *self);

message_byte_array* pdu_notreg_create_message(pdu_NOTREG *self);

message_byte_array* pdu_getlist_create_message(pdu_GETLIST *self);

message_byte_array* pdu_slist_create_message(pdu_SLIST *self);

message_byte_array* pdu_join_create_message(pdu_JOIN *self);

message_byte_array* pdu_participants_create_message(pdu_PARTICIPANTS *self);

message_byte_array* pdu_quit_create_message(pdu_QUIT *self);

message_byte_array* pdu_mess_create_message(pdu_MESS *self);

message_byte_array* pdu_pjoin_create_message(pdu_PJOIN *self);

message_byte_array* pdu_pleave_create_message(pdu_PLEAVE *self);

#endif /* PDU_PARSER_H_ */
