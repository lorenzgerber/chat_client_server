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

#ifndef PDU_SERIALIZER_H_
#define PDU_SERIALIZER_H_

#include <stdio.h>

#include "pdu_templates.h"
#include "message_byte_array.h"

message_byte_array* reg_create_message(pdu *self);

message_byte_array* alive_create_message(pdu *self);

message_byte_array* ack_create_message(pdu *self);

message_byte_array* notreg_create_message(pdu *self);

message_byte_array* getlist_create_message(pdu *self);

message_byte_array* slist_create_message(pdu *self);

message_byte_array* join_create_message(pdu *self);

message_byte_array* participants_create_message(pdu *self);

message_byte_array* quit_create_message(pdu *self);

message_byte_array* mess_create_message(pdu *self);

message_byte_array* pjoin_create_message(pdu *self);

message_byte_array* pleave_create_message(pdu *self);

#endif /* PDU_SERIALIZER_H_ */
