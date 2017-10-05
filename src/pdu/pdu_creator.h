/*
 * pdu_create.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */
#ifndef PDU_CREATOR_H_
#define PDU_CREATOR_H_
#include <string.h>

#include "pdu_serializer.h"
#include "pdu_templates.h"
#include "pdu_print.h"


// REG
int reg_add_server_name(pdu *pdu, char* server_name);

pdu* create_reg(uint8_t server_name_length, uint16_t tcp_port);

int free_reg(pdu *pdu);




// ALIVE
pdu* create_alive(uint8_t number_clients, uint16_t id_number);

int free_alive(pdu* pdu);

// ACK
pdu* create_ack(uint16_t id_number);

int free_ack(pdu* pdu);

// NOTREG
pdu* create_notreg(uint16_t id_number);

int free_notreg(pdu* pdu);

// GETLIST
pdu* create_getlist(void);

int free_getlist(pdu* pdu);



// SLIST
int server_entry_add_server_name(pdu_server_entry* pdu, char* server_name);

pdu_server_entry *create_server_entry(uint8_t address[4], uint16_t port, uint8_t number_clients, uint8_t name_length);

int add_server_entry(pdu *pdu, pdu_server_entry* server_entry);

pdu* create_slist(uint16_t number_servers);

int free_server_entry(pdu_server_entry *server);

int free_slist(pdu *pdu);


// JOIN

int join_add_identity(pdu *pdu, char* identity);

pdu* create_join(uint8_t identity_length);

int free_join(pdu *pdu);

// PARTICIPANTS
int participants_add_identities(pdu *pdu, char* identities);

pdu* create_participants(uint8_t number_identities, uint16_t length);

int free_participants(pdu *pdu);

// QUIT

pdu* create_quit(void);

int free_quit(pdu* pdu);

// MESS
int mess_calc_checksum(pdu *pdu);

int mess_add_identity(pdu *pdu, char* identity);

int mess_add_message(pdu *pdu, uint16_t message_length, uint32_t time_stamp, char* message);

pdu* create_mess(uint8_t identity_length, uint8_t checksum);

int free_mess(pdu *pdu);

// PJOIN

int pjoin_add_client_identity(pdu *pdu, uint32_t time_stamp, char* client_identity);

pdu* create_pjoin(uint8_t identity_length);

int free_pjoin(pdu *pdu);

// PLEAVE

int pleave_add_client_identity(pdu *pdu, uint32_t time_stamp, char* client_identity);


pdu* create_pleave(uint8_t identity_length);

int free_pleave(pdu *pdu);

#endif /* PDU_CREATOR_H_ */
