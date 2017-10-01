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

int reg_add_server_name(pdu *pdu, char* server_name){
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

pdu* create_reg(uint8_t servername_length, uint16_t tcp_port){
	pdu *pdu = malloc(sizeof(pdu));
	pdu->type = PDU_REG;
	pdu->server_name_length = servername_length;
	pdu->tcp_port = tcp_port;
	pdu->add_server_name = reg_add_server_name;
	pdu->create_message = reg_create_message;
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
pdu_ALIVE* create_ALIVE(uint8_t number_clients, uint16_t id_number){
	pdu_ALIVE *pdu = malloc(sizeof(pdu_ALIVE));
	pdu->type = PDU_ALIVE;
	pdu->number_clients = number_clients;
	pdu->id_number = id_number;
	pdu->create_message = pdu_alive_create_message;
	return pdu;
}

pdu* create_alive(uint8_t number_clients, uint16_t id_number){
	pdu *pdu = malloc(sizeof(pdu));
	pdu->type = PDU_ALIVE;
	pdu->number_clients = number_clients;
	pdu->id_number = id_number;
	pdu->create_message = alive_create_message;
	return pdu;
}


int free_pdu_alive(pdu_ALIVE* pdu){
	free(pdu);
	return 0;
}

/*
 * pdu_ACK
 */
pdu_ACK* create_ACK(uint16_t id_number){
	pdu_ACK *pdu = malloc(sizeof(pdu_ACK));
	pdu->type = PDU_ACK;
	pdu->id_number = id_number;
	pdu->create_message = pdu_ack_create_message;
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
	pdu->create_message = pdu_notreg_create_message;
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
	pdu->create_message = pdu_getlist_create_message;
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
	pdu->create_message = pdu_slist_create_message;
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
	pdu->create_message = pdu_join_create_message;
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
	// need to implement check of length

	int lower = 0, found = 0;
	for(int i = 0 ; i < pdu->length;i++){
		//read until null termination (detect length)
		if(identities[i]=='\0'){

			pdu->identities[found] = malloc((i-lower+1)*sizeof(char));
			strncpy(pdu->identities[found], &identities[lower], i-lower);
			lower = i+1;
			found++;
		}
	}
	if (found < pdu->number_identities){
		perror("not enough identities provided");
		return -1;
	}

	return 0;
}

pdu_PARTICIPANTS* create_PARTICIPANTS(uint8_t number_identities, uint16_t length){
	pdu_PARTICIPANTS *pdu = malloc(sizeof(pdu_PARTICIPANTS));
	pdu->type = PDU_PARTICIPANTS;
	pdu->number_identities = number_identities;
	pdu->length = length;
	pdu->identities = malloc(number_identities * sizeof(char*));
	pdu->add_identities = pdu_participants_add_identities;
	pdu->create_message = pdu_participants_create_message;
	return pdu;
}

int free_pdu_participants(pdu_PARTICIPANTS *pdu){

	if(pdu->number_identities !=0){
		for(int i = 0; i < pdu->number_identities;i++){
			free(pdu->identities[i]);
		}
	}

	free(pdu);
	return 0;
}

/*
 * pdu_QUIT
 */
pdu_QUIT* create_QUIT(void){
	pdu_QUIT *pdu = malloc(sizeof(pdu_QUIT));
	pdu->type = PDU_QUIT;
	pdu->create_message = pdu_quit_create_message;
	return pdu;
}

int free_pdu_quit(pdu_QUIT* pdu){
	free(pdu);
	return 0;
}

/*
 * MESS
 */
/*
int pdu_mess_padded_message_length(pdu_MESS *pdu){
	int result = pdu->message_length + (pdu->message_length % 4) - 4;
	return result;
}*/

int pdu_mess_calc_checksum(pdu_MESS *pdu){

	return 0;
}

int pdu_mess_add_client_identity(pdu_MESS *pdu, char* client_identity){
	pdu->client_identity = malloc(pdu->identity_length*sizeof(char));
	strcpy(pdu->client_identity, client_identity);

	return 0;
}

int pdu_mess_add_message(pdu_MESS *pdu, uint16_t message_length, uint32_t time_stamp, char* message){
	pdu->time_stamp = time_stamp;
	pdu->message_length = message_length;
	pdu->message = malloc(pdu->message_length* sizeof(char));
	strcpy(pdu->message, message);

 	return 0;
}


pdu_MESS* create_MESS(uint8_t identity_length, uint8_t checksum){
	pdu_MESS *pdu = malloc(sizeof(pdu_MESS));
	pdu->type = PDU_MESS;
	pdu->identity_length = identity_length;
	pdu->checksum = checksum;
	pdu->add_message = pdu_mess_add_message;
	pdu->add_client_identity = pdu_mess_add_client_identity;
	pdu->calc_checksum = pdu_mess_calc_checksum;
	//pdu->padded_message_length = pdu_mess_padded_message_length;
	pdu->create_message = pdu_mess_create_message;
	return pdu;
}

int free_pdu_mess(pdu_MESS *pdu){

	if(pdu->identity_length !=0){
		free(pdu->client_identity);
	}
	if(pdu->message_length >0){
		free(pdu->message);
	}

	free(pdu);
	return 0;
}

/*
 * PJOIN
 */
int pdu_pjoin_add_client_identity(pdu_PJOIN *pdu, uint32_t time_stamp, char* client_identity){
	pdu->time_stamp = time_stamp;
	pdu->client_identity = malloc(pdu->identity_length*sizeof(char));
	strcpy(pdu->client_identity, client_identity);
	return 0;
}


pdu_PJOIN* create_PJOIN(uint8_t identity_length){
	pdu_PJOIN *pdu = malloc(sizeof(pdu_PJOIN));
	pdu->type = PDU_PJOIN;
	pdu->identity_length = identity_length;
	pdu->add_client_identity = pdu_pjoin_add_client_identity;
	pdu->create_message = pdu_pjoin_create_message;
	return pdu;
}

int free_pdu_pjoin(pdu_PJOIN *pdu){
	if(pdu->identity_length != 0){
		free(pdu->client_identity);
	}
	free(pdu);
	return 0;
}


/*
 * PLEAVE
 */
int pdu_pleave_add_client_identity(pdu_PLEAVE *pdu, uint32_t time_stamp, char* client_identity){
	pdu->time_stamp = time_stamp;
	pdu->client_identity = malloc(pdu->identity_length*sizeof(char));
	strcpy(pdu->client_identity, client_identity);
	return 0;
}


pdu_PLEAVE* create_PLEAVE(uint8_t identity_length){
	pdu_PLEAVE *pdu = malloc(sizeof(pdu_PLEAVE));
	pdu->type = PDU_PLEAVE;
	pdu->identity_length = identity_length;
	pdu->add_client_identity = pdu_pleave_add_client_identity;
	pdu->create_message = pdu_pleave_create_message;
	return pdu;
}

int free_pdu_pleave(pdu_PLEAVE *pdu){
	if(pdu->identity_length != 0){
		free(pdu->client_identity);
	}
	free(pdu);
	return 0;
}


