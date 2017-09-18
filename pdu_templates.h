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
	unsigned char type;
	void* message;
} pdu_prototype;

typedef struct pdu_REG {
	unsigned char type;
	unsigned char server_name_length;
	unsigned int tcp_port;
	char* server_name;
	int (*add_server_name)(struct pdu_REG *self, char*);
} pdu_REG;

typedef struct pdu_ALIVE {
	unsigned char type;
	unsigned char number_clients;
	unsigned int id_number;
} pdu_ALIVE;

typedef struct pdu_ACK {
	unsigned char type;
	unsigned char padding[2];
	unsigned int id_number;
} pdu_ACK;

typedef struct pdu_NOTREG {
	unsigned char type;
	unsigned char padding[2];
	unsigned int id_number;
} pdu_NOTREG;

typedef struct pdu_GETLIST {
	unsigned char type;
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
	unsigned char type;
	unsigned char padding[1];
	unsigned int number_servers;
	pdu_server_entry *current_servers;
} pdu_SLIST;



typedef struct pdu_JOIN {
	unsigned char type;
	unsigned char identity_length;
	unsigned char padding[2];
	char *identity;
} pdu_JOIN;

typedef struct pdu_PARTICIPANTS {
	unsigned char type;
	unsigned char number_identities;
	unsigned int length;
	char *identities;
} pdu_PARTICIPANTS;


typedef struct pdu_QUIT {
	unsigned char type;
	unsigned char padding[3];
} pdu_QUIT;

typedef struct pdu_MESS {
	unsigned char type;
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
	unsigned char type;
	unsigned char identity_length;
	unsigned char padding[2];
	unsigned long time_stamp;
	char *client_identity;
} pdu_PJOIN;

typedef struct pdu_PLEAVE {
	unsigned char type;
	unsigned char identity_length;
	unsigned char padding[2];
	unsigned long time_stamp;
	char *client_identity;
} pdu_PLEAVE;




int free_server_entry(pdu_server_entry server);

pdu_REG* create_REG(unsigned server_name_length, unsigned int tcp_port);

int add_server_name(pdu_REG* pdu, char* server_name);

int free_pdu_reg(pdu_REG *pdu);

#endif /* PDU_TEMPLATES_H_ */
