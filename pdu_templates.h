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
#include "message_byte_array.h"

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
	char* (*create_message)(struct pdu_REG *self);
} pdu_REG;

typedef struct pdu_ALIVE {
	uint8_t type;
	uint8_t number_clients;
	uint16_t id_number;
} pdu_ALIVE;

typedef struct pdu_ACK {
	uint8_t type;
	uint8_t padding[2];
	uint16_t id_number;
} pdu_ACK;

typedef struct pdu_NOTREG {
	uint8_t type;
	uint8_t padding[2];
	uint16_t id_number;
} pdu_NOTREG;

typedef struct pdu_GETLIST {
	uint8_t type;
	uint8_t padding[3];
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
	int (*add_server_entry)(struct pdu_SLIST *self, pdu_server_entry*);
	int server_assigned;
	pdu_server_entry **current_servers;
} pdu_SLIST;



typedef struct pdu_JOIN {
	uint8_t type;
	uint8_t identity_length;
	uint8_t padding[2];
	char *identity;
	int (*add_identity)(struct pdu_JOIN *self, char*);
} pdu_JOIN;

typedef struct pdu_PARTICIPANTS {
	uint8_t type;
	uint8_t number_identities;
	uint16_t length;
	char **identities;
	int (*add_identities)(struct pdu_PARTICIPANTS *self, char*);
} pdu_PARTICIPANTS;


typedef struct pdu_QUIT {
	uint8_t type;
	uint8_t padding[3];
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
} pdu_MESS;

typedef struct pdu_PJOIN {
	uint8_t type;
	uint8_t identity_length;
	uint8_t padding[2];
	uint32_t time_stamp;
	char *client_identity;
	int (*add_client_identity)(struct pdu_PJOIN *self, uint32_t, char*);
} pdu_PJOIN;

typedef struct pdu_PLEAVE {
	uint8_t type;
	uint8_t identity_length;
	uint8_t padding[2];
	uint32_t time_stamp;
	char *client_identity;
	int (*add_client_identity)(struct pdu_PLEAVE *self, uint32_t, char*);
} pdu_PLEAVE;


int get_type(void *message);

#endif /* PDU_TEMPLATES_H_ */
