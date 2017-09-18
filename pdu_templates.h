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

typedef struct base_pdu {
	unsigned char type;
	void* message;
} base_pdu;

typedef struct pdu_REG {
	unsigned char server_length;
	unsigned int tcp_port;
	char* server_name;
} pdu_REG;

typedef struct pdu_ALIVE {
	unsigned char number_clients;
	unsigned int id_number;
} pdu_ALIVE;

typedef struct pdu_ACK {
	unsigned char padding[2];
	unsigned int id_number;
} pdu_ACK;

typedef struct pdu_NOTREG {
	unsigned char padding[2];
	unsigned int id_number;
} pdu_NOTREG;

typedef struct pdu_GETLIST {
	unsigned char padding[3];
} pdu_GETLIST;

typedef struct pdu_SLIST {
	unsigned char padding[1];
	unsigned int number_servers;
	struct server* current_servers[];
} pdu_SLIST;

typedef struct server_entry {
	unsigned char address[4];
	unsigned int port;
	unsigned char number_clients;
	unsigned char name_length;
	char *name;
} pdu_server_entry;

typedef struct pdu_JOIN {
	unsigned char identity_length;
	unsigned char padding[2];
	char *identity;
} pdu_JOIN;

typedef struct pdu_PARTICIPANTS {
	unsigned char number_identities;
	unsigned int length;
	char *identities;
} pdu_PARTICIPANTS;


typedef struct pdu_QUIT {
	unsigned char padding[3];
} pdu_QUIT;

typedef struct pdu_MESS {
	unsigned char padding1[1];
	unsigned char identity_length;
	unsigned char checksum;
	unsigned int message_length;
	unsigned char patting2[2];
	unsigned long time_stamp;
	char *message;
	char *client_identity;
} pdu_MESS;

typedef struct pdu_PJOIN {
	unsigned char identity_length;
	unsigned char padding[2];
	unsigned long time_stamp;
	char *client_identity;
} pdu_PJOIN;

typedef struct pdu_PLEAVE {
	unsigned char identity_length;
	unsigned char padding[2];
	unsigned long time_stamp;
	char *client_identity;
} pdu_PLEAVE;


int get_type(base_pdu pdu);

int create_type(base_pdu pdu, int type);





#endif /* PDU_TEMPLATES_H_ */
