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


// PARTICIPANTS
int pdu_participants_add_identities(pdu_PARTICIPANTS *pdu, char* identities);

pdu_PARTICIPANTS* create_PARTICIPANTS(uint8_t number_identities, uint16_t length);

int free_pdu_participants(pdu_PARTICIPANTS *pdu);


// QUIT

pdu_QUIT* create_QUIT(void);

int free_pdu_quit(pdu_QUIT* pdu);


// MESS
int pdu_padded_message_length(pdu_MESS *pdu);

int pdu_mess_calc_checksum(pdu_MESS *pdu);

int pdu_mess_add_identity(pdu_MESS *pdu, char* identity);

int pdu_mess_add_message(pdu_MESS *pdu, uint16_t message_length, uint32_t time_stamp, char* message);

pdu_MESS* create_MESS(uint8_t identity_length, uint8_t checksum);

int free_pdu_mess(pdu_MESS *pdu);


// PJOIN

int pdu_pjoin_add_client_identity(pdu_PJOIN *pdu, uint32_t time_stamp, char* client_identity);

pdu_PJOIN* create_PJOIN(uint8_t identity_length);

int free_pdu_pjoin(pdu_PJOIN *pdu);


// PLEAVE

int pdu_pleave_add_client_identity(pdu_PLEAVE *pdu, uint32_t time_stamp, char* client_identity);

pdu_PLEAVE* create_PLEAVE(uint8_t identity_length);

int free_pdu_pleave(pdu_PLEAVE *pdu);


#endif /* PDU_CREATOR_H_ */