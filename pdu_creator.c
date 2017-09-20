/*
 * pdu_create.c
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */
#include "pdu_creator.h"

/*
 * pdu_REG
 */

int pdu_reg_add_server_name(pdu_REG *pdu, char* server_name){
	if(strlen(server_name) == pdu->server_name_length){
		pdu->server_name = malloc(pdu->server_name_length*sizeof(char));
		strcpy(pdu->server_name, server_name);
	} else {
		perror("server_name length missmatch\n");
		return -1;
	}
	return 0;
}

pdu_REG* create_REG(uint8_t servername_length, uint16_t tcp_port){
	pdu_REG *pdu = malloc(sizeof(pdu_REG));
	pdu->type = PDU_REG;
	pdu->server_name_length = servername_length;
	pdu->tcp_port = tcp_port;
	pdu->add_server_name = pdu_reg_add_server_name;
	pdu->create_message = pdu_reg_create_message;
	return pdu;
}



int free_pdu_reg(pdu_REG *pdu){

	if(pdu->server_name != NULL){
		free(pdu->server_name);
	}
	free(pdu);
	return 0;
}

/*
 * pdu_ALIVE
 */
pdu_ALIVE* create_ALIVE(uint16_t id_number){
	pdu_ALIVE *pdu = malloc(sizeof(pdu_ALIVE));
	pdu->type = PDU_ALIVE;
	pdu->id_number = id_number;
	return pdu;
}

int free_pdu_alive(pdu_ALIVE* pdu){
	free(pdu);
	return 0;
}

/*
 * pdu_ACK
 */
pdu_ALIVE* create_ACK(uint16_t id_number){
	pdu_ALIVE *pdu = malloc(sizeof(pdu_ACK));
	pdu->type = PDU_ALIVE;
	pdu->id_number = id_number;
	return pdu;
}

int free_pdu_ack(pdu_ACK* pdu){
	free(pdu);
	return 0;
}

/*
 * pdu_NOTREG
 */
pdu_NOTREG* create_NOTREG(uint16_t id_number){
	pdu_NOTREG *pdu = malloc(sizeof(pdu_NOTREG));
	pdu->type = PDU_NOTREG;
	pdu->id_number = id_number;
	return pdu;
}

int free_pdu_notreg(pdu_NOTREG* pdu){
	free(pdu);
	return 0;
}



/*
 * pdu_GETLIST
 */
pdu_GETLIST* create_GETLIST(void){
	pdu_GETLIST *pdu = malloc(sizeof(pdu_GETLIST));
	pdu->type = PDU_GETLIST;
	return pdu;
}

int free_pdu_getlist(pdu_GETLIST* pdu){
	free(pdu);
	return 0;
}




/*
 * pdu_SLIST
 */
int server_entry_add_server_name(pdu_server_entry *pdu, char* server_name){
	if(strlen(server_name) == pdu->name_length){
		 pdu->name = malloc(pdu->name_length*sizeof(char));
		strcpy(pdu->name, server_name);
	} else {
		perror("server_name length missmatch\n");
		return -1;
	}
	return 0;
}


pdu_server_entry* create_server_entry(uint8_t address[4], uint16_t port, uint8_t number_clients, uint8_t name_length){
	pdu_server_entry *pdu = malloc(sizeof(pdu_server_entry));
	for(int i = 0; i < 4;i++){
		pdu->address[i] = address[i];
	}
	pdu->port = port;
	pdu->number_clients = number_clients;
	pdu->name_length = name_length;
	pdu->add_server_name = server_entry_add_server_name;

	return pdu;
}

int add_server_entry(pdu_SLIST *pdu, pdu_server_entry* server_entry){
	if(pdu->server_assigned == pdu->number_servers){
		perror("all servers already assigned\n");
		return -1;
	}
	pdu->current_servers[pdu->server_assigned] = server_entry;
	pdu->server_assigned++;
 	return pdu->server_assigned;
}

pdu_SLIST* create_SLIST(uint16_t number_servers){
	pdu_SLIST *pdu = malloc(sizeof(pdu_SLIST));
	pdu->type = PDU_SLIST;
	pdu->number_servers = number_servers;
	pdu->server_assigned = 0;
	pdu->current_servers = malloc(sizeof(pdu_server_entry*)*number_servers); //probably wrong
	*pdu->current_servers = (pdu_server_entry*)malloc(number_servers * sizeof(pdu_server_entry*));
	pdu->add_server_entry = add_server_entry;
	return pdu;
}

int free_server_entry(pdu_server_entry *server){
	if(server->name != NULL){
		free(server->name);
	}
	return 0;
}

int free_pdu_slist(pdu_SLIST *pdu){

	if(pdu->number_servers !=0){
		for(int i = 0; i < pdu->number_servers;i++){
			free_server_entry(pdu->current_servers[i]);
		}
	}
	free(pdu);
	return 0;
}



/*
 * pdu_JOIN
 */
int pdu_join_add_identity(pdu_JOIN *pdu, char* identity){
	if(strlen(identity) == pdu->identity_length){
		pdu->identity = malloc(pdu->identity_length*sizeof(char));
		strcpy(pdu->identity, identity);
	} else {
		perror("identity length missmatch\n");
		return -1;
	}
	return 0;
}

pdu_JOIN* create_JOIN(uint8_t identity_length){
	pdu_JOIN *pdu = malloc(sizeof(pdu_JOIN));
	pdu->type = PDU_JOIN;
	pdu->identity_length = identity_length;
	pdu->add_identity = pdu_join_add_identity;
	return pdu;
}

int free_pdu_join(pdu_JOIN *pdu){

	if(pdu->identity != NULL){
		free(pdu->identity);
	}
	free(pdu);
	return 0;
}


/*
 * pdu_PARTICIPANTS
 */
int pdu_participants_add_identities(pdu_PARTICIPANTS *pdu, char* identities){

	//Check length of provided string
	if(strlen(identities) != pdu->length){
		perror("identities length missmatch\n");
		return -1;
	}

	int lower, found = 0;
	for(int i = 0 ; i < pdu->length;i++){
		//read until null termination (detect length)
		if(identities[i]=='\0'){
			pdu->identities[i] = malloc(i-lower*sizeof(char));
			strncpy(pdu->identities[i], &identities[lower], i-lower);
			lower = i;
			found++;
		}
	}
	if (found < pdu->number_identities){
		perror("not enough identities provided");
		return -1;
	}

	return 0;
}

pdu_PARTICIPANTS* create_PARTICIPANTS(uint8_t number_identities){
	pdu_PARTICIPANTS *pdu = malloc(sizeof(pdu_PARTICIPANTS));
	pdu->type = PDU_PARTICIPANTS;
	pdu->number_identities = number_identities;
	*pdu->identities = (char*)malloc(number_identities * sizeof(char*));
	pdu->add_identities = pdu_participants_add_identities;
	return pdu;
}

int free_pdu_participant(pdu_PARTICIPANTS *pdu){

	if(pdu->identities != NULL){
		free(pdu->identities);
	}
	free(pdu);
	return 0;
}

