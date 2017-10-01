/*
 * pdu_templates.h
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */

#ifndef PDU_TEMPLATES_H_
#define PDU_TEMPLATES_H_
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PDU_REG 0
#define PDU_ACK 1
#define PDU_ALIVE 2
#define PDU_NOTREG 100
#define PDU_GETLIST 3
#define PDU_SLIST 4
#define PDU_MESS 10
#define PDU_QUIT 11
#define PDU_JOIN 12
#define PDU_PJOIN 16
#define PDU_PLEAVE 17
#define PDU_PARTICIPANTS 19

#define LENGTH_OP 1
#define LENGTH_SERVER_NAME 1		// REG, SLIST
#define LENGTH_PORT 2				// REG, MESS
#define LENGTH_NUMBER_CLIENTS 1		// SLIST, ALIVE
#define LENGTH_ID_NUMBER 2			// NOTREG, ACK, ALIVE
#define LENGTH_NUMBER_SERVER 2		// SLIST
#define LENGTH_ADDRESS 4			// SLIST
#define LENGTH_SERVER_NAME_LENGTH 1 // SLIST, REG
#define LENGTH_IDENTITY_LENGTH 1	// PLEAVE, PJOIN, MESS, JOIN
#define LENGTH_NUMBER_IDENTITIES 1	// PARTICIPANTS
#define LENGTH_CHECKSUM 1 			// MESS
#define LENGTH_MESSAGE_LENGTH 2 	// MESS
#define LENGTH_TIME 4 				// PLEAVE, PJOIN, MESS
#define LENGTH_PAD 1





typedef struct pdu_prototype {
	uint8_t type;
	uint8_t server_name_length;
	char* server_name;
} pdu_prototype;



typedef struct pdu_REG {
	uint8_t type;
	uint8_t server_name_length;
	uint16_t tcp_port;
	char* server_name;
	int (*add_server_name)(struct pdu_REG *self, char*);
	struct message_byte_array* (*create_message)(struct pdu_REG *self);
} pdu_REG;

typedef struct pdu_ALIVE {
	uint8_t type;
	uint8_t number_clients;
	uint16_t id_number;
	struct message_byte_array* (*create_message)(struct pdu_ALIVE *self);
} pdu_ALIVE;

typedef struct pdu_ACK {
	uint8_t type;
	uint8_t padding[2];
	uint16_t id_number;
	struct message_byte_array* (*create_message)(struct pdu_ACK *self);
} pdu_ACK;

typedef struct pdu_NOTREG {
	uint8_t type;
	uint8_t padding[2];
	uint16_t id_number;
	struct message_byte_array* (*create_message)(struct pdu_NOTREG *self);
} pdu_NOTREG;

typedef struct pdu_GETLIST {
	uint8_t type;
	uint8_t padding[3];
	struct message_byte_array* (*create_message)(struct pdu_GETLIST *self);
} pdu_GETLIST;

typedef struct pdu_server_entry {
	uint8_t address[4];
	uint16_t port;
	uint8_t number_clients;
	uint8_t name_length;
	char *name;
	int (*add_server_name)(struct pdu_server_entry *self, char*);
} pdu_server_entry;

typedef struct pdu_SLIST {
	uint8_t type;
	uint8_t padding[1];
	uint16_t number_servers;
	int server_assigned;
	pdu_server_entry **current_servers;
	int (*add_server_entry)(struct pdu_SLIST *self, pdu_server_entry*);
	struct message_byte_array* (*create_message)(struct pdu_SLIST *self);
} pdu_SLIST;



typedef struct pdu_JOIN {
	uint8_t type;
	uint8_t identity_length;
	uint8_t padding[2];
	char *identity;
	int (*add_identity)(struct pdu_JOIN *self, char*);
	struct message_byte_array* (*create_message)(struct pdu_JOIN *self);
} pdu_JOIN;

typedef struct pdu_PARTICIPANTS {
	uint8_t type;
	uint8_t number_identities;
	uint16_t length;
	char **identities;
	int (*add_identities)(struct pdu_PARTICIPANTS *self, char*);
	struct message_byte_array* (*create_message)(struct pdu_PARTICIPANTS *self);
} pdu_PARTICIPANTS;


typedef struct pdu_QUIT {
	uint8_t type;
	uint8_t padding[3];
	struct message_byte_array* (*create_message)(struct pdu_QUIT *self);
} pdu_QUIT;

typedef struct pdu_MESS {
	uint8_t type;
	uint8_t padding1[1];
	uint8_t identity_length;
	uint8_t checksum;
	uint16_t message_length;
	uint8_t padding2[2];
	uint32_t time_stamp;
	char *message;
	char *client_identity;
	int (*add_message)(struct pdu_MESS *self, uint16_t, uint32_t, char*);
	int (*add_client_identity)(struct pdu_MESS *self, char*);
	int (*calc_checksum)(struct pdu_MESS *self);
	int (*padded_message_length)(struct pdu_MESS *self);
	struct message_byte_array* (*create_message)(struct pdu_MESS *self);
} pdu_MESS;

typedef struct pdu_PJOIN {
	uint8_t type;
	uint8_t identity_length;
	uint8_t padding[2];
	uint32_t time_stamp;
	char *client_identity;
	int (*add_client_identity)(struct pdu_PJOIN *self, uint32_t, char*);
	struct message_byte_array* (*create_message)(struct pdu_PJOIN *self);
} pdu_PJOIN;

typedef struct pdu_PLEAVE {
	uint8_t type;
	uint8_t identity_length;
	uint8_t padding[2];
	uint32_t time_stamp;
	char *client_identity;
	int (*add_client_identity)(struct pdu_PLEAVE *self, uint32_t, char*);
	struct message_byte_array* (*create_message)(struct pdu_PLEAVE *self);
} pdu_PLEAVE;


typedef struct pdu {
	uint8_t 	type;

	uint8_t 			checksum;
	pdu_server_entry 	**current_servers;
	char 				**identities;
	char 				*identity;
	uint8_t 			identity_length;
	uint16_t 			id_number;
	uint16_t 			length; // FROM PARTICIPANTS, length of identities blob
	char 				*message;
	uint16_t 			message_length;
	uint8_t 			number_clients;
	uint8_t 			number_identities;
	uint16_t 			number_servers;
	int 				server_assigned; // not sure what for SLIST
	char* 				server_name;
	uint8_t 			server_name_length;
	uint16_t 			tcp_port;
	uint32_t 			time_stamp;

	int (*add_message)(struct pdu *self, uint16_t, uint32_t, char*);
	int (*calc_checksum)(struct pdu *self);
	int (*padded_message_length)(struct pdu *self);
	int (*add_identities)(struct pdu *self, char*);
	int (*add_identity)(struct pdu *self, char*);
	int (*add_client_identity)(struct pdu *self, uint32_t time_stamp, char* client_identity);
	int (*add_server_entry)(struct pdu *self, pdu_server_entry*);
	int (*add_server_name)(struct pdu *self, char*);
	struct message_byte_array* (*create_message)(struct pdu *self);

} pdu;



int get_type(void *message);

int calc_word_padding(uint32_t length);


uint32_t get_length_REG(pdu_REG *pdu);
uint32_t get_length_reg(pdu *pdu);

uint32_t get_length_ALIVE(pdu_ALIVE *pdu);
uint32_t get_length_alive(pdu *pdu);

uint32_t get_length_ACK(pdu_ACK *pdu);
uint32_t get_length_ack(pdu *pdu);

uint32_t get_length_NOTREG(pdu_NOTREG *pdu);
uint32_t get_length_notreg(pdu *pdu);

uint32_t get_length_GETLIST(pdu_GETLIST *pdu);
uint32_t get_length_getlist(pdu *pdu);

uint32_t get_length_SLIST(pdu_SLIST *pdu);
uint32_t get_length_slist(pdu *pdu);

uint32_t get_length_JOIN(pdu_JOIN *pdu);
uint32_t get_length_join(pdu *pdu);

uint32_t get_length_PARTICIPANTS(pdu_PARTICIPANTS *pdu);
uint32_t get_length_participants(pdu *pdu);

uint32_t get_length_QUIT(pdu_QUIT *pdu);
uint32_t get_length_quit(pdu *pdu);

uint32_t get_length_MESS(pdu_MESS *pdu);
uint32_t get_length_mess(pdu *pdu);

uint32_t get_length_PJOIN(pdu_PJOIN *pdu);
uint32_t get_length_pjoin(pdu *pdu);

uint32_t get_length_PLEAVE(pdu_PLEAVE *pdu);
uint32_t get_length_pleave(pdu *pdu);

#endif /* PDU_TEMPLATES_H_ */
