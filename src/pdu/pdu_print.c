/*
 * pdu_print.c
 *
 *  Created on: Oct 5, 2017
 *      Author: lgerber
 */

#include "pdu_print.h"

/**
 * Print function for a pdu of the type REG. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_reg(pdu *self){
	printf("op code: %d\n", self->type);
	printf("tcp_port: %d\n", self->tcp_port);
	printf("servername: %s\n", self->server_name);

	return 0;
}

/**
 * Print function for a pdu of the type ACK. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_ack(pdu *self){
	printf("op code: %d\n", self->type);
	printf("identity nr: %d\n", self->id_number);

	return 0;
}

/**
 * Print function for a pdu of the type ALIVE. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_alive(pdu *self){
	printf("op code: %d\n", self->type);
	printf("number of clients: %d\n", self->number_clients);
	printf("id number: %d\n", self->id_number);

	return 0;
}

/**
 * Print function for a pdu of the type NOTREG. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_notreg(pdu *self){
	printf("op code: %d\n", self->type);
	printf("id number: %d\n", self->id_number);

	return 0;
}

/**
 * Print function for a pdu of the type GETLIST. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_getlist(pdu *self){
	printf("op code: %d\n", self->type);

	return 0;
}

/**
 * Print function for a pdu of the type SLIST. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_slist(pdu *self){

	printf("op code: %d\n", self->type);
	printf("nr of servers: %d\n", self->number_servers);
	for(int i = 0;i < self->number_servers; i++){
		printf("****Server %d****\n", i+1);
		printf("adress: %d,%d,%d,%d\n", self->current_servers[i]->address[0],
										self->current_servers[i]->address[1],
										self->current_servers[i]->address[2],
										self->current_servers[i]->address[3]);
		printf("port: %d\n", self->current_servers[i]->port);
		printf("number of clients: %d\n", self->current_servers[i]->number_clients);
		printf("server name length: %d\n", self->current_servers[i]->name_length);
		printf("Servername: %s\n", self->current_servers[i]->name);
	}

	return 0;
}

/**
 * Print function for a pdu of the type MESS. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_mess(pdu *self){
	printf("op code: %d\n", self->type);
	printf("identity length: %d\n", self->identity_length);
	printf("Checksum: %d\n", self->checksum);
	printf("message length: %d\n", self->message_length);
	printf("timestamp: %u\n", self->time_stamp);
	printf("message: %s\n", self->message);
	printf("client identity: %s\n", self->identity);

	return 0;
}

/**
 * Print function for a pdu of the type QUIT. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_quit(pdu *self){
	printf("op code: %d\n", self->type);

	return 0;
}

/**
 * Print function for a pdu of the type JOIN. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_join(pdu *self){
	printf("op code: %d\n", self->type);
	printf("identity length: %d\n", self->identity_length);
	printf("identity: %s\n", self->identity);

	return 0;
}

/**
 * Print function for a pdu of the type PJOIN. The function is registered in a
 * pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_pjoin(pdu *self){
	printf("op code: %d\n", self->type);
	printf("identity length: %d\n", self->identity_length);
	printf("timestamp: %u\n", self->time_stamp);
	printf("client identity: %s\n", self->identity);

	return 0;
}

/**
 * Print function for a pdu of the type PLEAVE. The function is
 * registered in a pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_pleave(pdu *self){
	printf("op code: %d\n", self->type);
	printf("identity length: %d\n", self->identity_length);
	printf("timestamp: %u\n", self->time_stamp);
	printf("client identity: %s\n", self->identity);

	return 0;
}

/**
 * Print function for a pdu of the type PARTICIPANTS. The function
 * is registered in a pdu struct in the create function
 *
 * @param self The calling pdu
 * @return 0
 */
int print_participants(pdu *self){
	printf("op code: %d\n", self->type);
	printf("nr of identities: %d\n", self->number_identities);
	printf("identities length: %d\n", self->length);
	for(int i = 0; i < self->number_identities; i++){
		printf("Identity %d: %s\n",i+1,self->identities[i]);
	}

	return 0;
}






