/*
 * pdu_print.h
 *
 *  Created on: Oct 5, 2017
 *      Author: lgerber
 */

#ifndef SRC_PDU_PDU_PRINT_H_
#define SRC_PDU_PDU_PRINT_H_
#include "pdu_templates.h"

int print_reg(pdu *self);

int print_ack(pdu *self);

int print_alive(pdu *self);

int print_notreg(pdu *self);

int print_getlist(pdu *self);

int print_slist(pdu *self);

int print_mess(pdu *self);

int print_quit(pdu *self);

int print_join(pdu *self);

int print_pjoin(pdu *self);

int print_pleave(pdu *self);

int print_participants(pdu *self);

#endif /* SRC_PDU_PDU_PRINT_H_ */
