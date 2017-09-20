/*
 * pdu_create.h
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */

#ifndef PDU_CREATOR_H_
#define PDU_CREATOR_H_
#include "pdu_templates.h"
#include "pdu_parser.h"


// REG
int pdu_reg_add_server_name(pdu_REG *pdu, char* server_name);

pdu_REG* create_REG(uint8_t server_name_length, uint16_t tcp_port);

int free_pdu_reg(pdu_REG *pdu);


// ALIVE
pdu_ALIVE* create_ALIVE(uint16_t id_number);

int free_pdu_alive(pdu_ALIVE* pdu);

pdu_ALIVE* create_ACK(uint16_t id_number);

int free_pdu_ack(pdu_ACK* pdu);


// NOTREG
pdu_NOTREG* create_NOTREG(uint16_t id_number);

int free_pdu_notreg(pdu_NOTREG* pdu);


// GETLIST
pdu_GETLIST* create_GETLIST(void);

int free_pdu_getlist(pdu_GETLIST* pdu);

// SLIST
int server_entry_add_server_name(pdu_server_entry* pdu, char* server_name);

pdu_server_entry *create_server_entry(uint8_t address[4], uint16_t port, uint8_t number_clients, uint8_t name_length);

int add_server_entry(pdu_SLIST *pdu, pdu_server_entry* server_entry);

pdu_SLIST* create_SLIST(uint16_t number_servers);

int free_server_entry(pdu_server_entry *server);

int free_pdu_slist(pdu_SLIST *pdu);

// JOIN

int pdu_join_add_identity(pdu_JOIN *pdu, char* identity);

pdu_JOIN* create_JOIN(uint8_t identity_length);

int free_pdu_(pdu_JOIN *pdu);



#endif /* PDU_CREATOR_H_ */
