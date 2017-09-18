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

typedef struct pdu_wrapper {
	unsigned char type;
	void* message;
} pdu_wrapper;

typedef struct pdu_REG {
	unsigned char server_name_length;
	unsigned int tcp_port;
	char* server_name;
	int (*add_server_name)(pdu_wrapper *self, char*);
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

typedef struct pdu_server_entry {
	unsigned char address[4];
	unsigned int port;
	unsigned char number_clients;
	unsigned char name_length;
	char *name;
} pdu_server_entry;

typedef struct pdu_SLIST {
	unsigned char padding[1];
	unsigned int number_servers;
	pdu_server_entry *current_servers;
} pdu_SLIST;



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
	unsigned char padding2[2];
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


int get_type(pdu_wrapper pdu);

int create_type(pdu_wrapper *pdu, int type);

int free_type(pdu_wrapper *pdu);

int free_server_entry(pdu_server_entry server);

pdu_wrapper* create_REG(unsigned server_name_length, unsigned int tcp_port);

int add_server_name(pdu_wrapper* pdu, char* server_name);



#endif /* PDU_TEMPLATES_H_ */
