/*
 * pdu_parser.h
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
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

message_byte_array* pdu_slist_create_message(pdu_SLIST *self);
message_byte_array* slist_create_message(pdu *self);

message_byte_array* pdu_join_create_message(pdu_JOIN *self);
message_byte_array* join_create_message(pdu *self);

message_byte_array* pdu_participants_create_message(pdu_PARTICIPANTS *self);
message_byte_array* participants_create_message(pdu *self);

message_byte_array* pdu_quit_create_message(pdu_QUIT *self);
message_byte_array* quit_create_message(pdu *self);

message_byte_array* pdu_mess_create_message(pdu_MESS *self);
message_byte_array* mess_create_message(pdu *self);

message_byte_array* pdu_pjoin_create_message(pdu_PJOIN *self);
message_byte_array* pjoin_create_message(pdu *self);

message_byte_array* pdu_pleave_create_message(pdu_PLEAVE *self);
message_byte_array* pleave_create_message(pdu *self);

#endif /* PDU_SERIALIZER_H_ */
