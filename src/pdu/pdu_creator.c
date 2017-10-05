/*
 * pdu_create.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */
#include "pdu_creator.h"

/*
 * pdu_REG
 */
int reg_add_server_name(pdu *pdu, char* server_name){
	if(strlen(server_name) == pdu->server_name_length){
		pdu->server_name = malloc((pdu->server_name_length+1)*sizeof(char));
		strcpy(pdu->server_name, server_name);
	} else {
		perror("server_name length missmatch\n");
		return -1;
	}
	return 0;
}

pdu* create_reg(uint8_t servername_length, uint16_t tcp_port){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_REG;
	pdu->server_name_length = servername_length;
	pdu->tcp_port = tcp_port;
	pdu->add_server_name = reg_add_server_name;
	pdu->create_message = reg_create_message;
	pdu->get_message_length = get_length_reg;
	pdu->free_pdu = free_reg;
	return pdu;
}

int free_reg(pdu *pdu){

	if(pdu->server_name != NULL){
		free(pdu->server_name);
	}
	free(pdu);
	return 0;
}






/*
 * pdu_ALIVE
 */

pdu* create_alive(uint8_t number_clients, uint16_t id_number){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_ALIVE;
	pdu->number_clients = number_clients;
	pdu->id_number = id_number;
	pdu->create_message = alive_create_message;
	pdu->get_message_length = get_length_alive;
    pdu->free_pdu = free_alive;
	return pdu;
}

int free_alive(pdu* pdu){
    free(pdu);
    return 0;
}



/*
 * pdu_ACK
 */

pdu* create_ack(uint16_t id_number){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_ACK;
	pdu->id_number = id_number;
	pdu->create_message = ack_create_message;
	pdu->get_message_length = get_length_ack;
    pdu->free_pdu = free_ack;

	return pdu;
}


int free_ack(pdu* pdu){
    free(pdu);
    return 0;
}



/*
 * pdu_NOTREG
 */

pdu* create_notreg(uint16_t id_number){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_NOTREG;
	pdu->id_number = id_number;
	pdu->create_message = notreg_create_message;
	pdu->get_message_length = get_length_notreg;
    pdu->free_pdu = free_notreg;
	return pdu;
}

int free_notreg(pdu* pdu){
    free(pdu);
    return 0;
}


/*
 * pdu_GETLIST
 */

pdu* create_getlist(void){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_GETLIST;
	pdu->create_message = getlist_create_message;
	pdu->get_message_length = get_length_getlist;
    pdu->free_pdu = free_getlist;
	return pdu;
}

int free_getlist(pdu* pdu){
    free(pdu);
    return 0;
}



/*
 * pdu_SLIST
 */
int server_entry_add_server_name(pdu_server_entry *pdu, char* server_name){
	if(strlen(server_name) == pdu->name_length){
		 pdu->name = malloc((pdu->name_length+1)*sizeof(char));
		strcpy(pdu->name, server_name);
	} else {
		perror("server_name length missmatch\n");
		return -1;
	}
	return 0;
}


pdu_server_entry* create_server_entry(uint8_t address[4], uint16_t port, uint8_t number_clients, uint8_t name_length){
	pdu_server_entry *pdu = malloc(sizeof(struct pdu_server_entry));
	for(int i = 0; i < 4;i++){
		pdu->address[i] = address[i];
	}
	pdu->port = port;
	pdu->number_clients = number_clients;
	pdu->name_length = name_length;
	pdu->add_server_name = server_entry_add_server_name;

	return pdu;
}


int add_server_entry(pdu *pdu, pdu_server_entry* server_entry){
	if(pdu->server_assigned == pdu->number_servers){
		perror("all servers already assigned\n");
		return -1;
	}
	pdu->current_servers[pdu->server_assigned] = server_entry;
	pdu->server_assigned++;
 	return pdu->server_assigned;
}


pdu* create_slist(uint16_t number_servers){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_SLIST;
	pdu->number_servers = number_servers;
	pdu->server_assigned = 0;
	pdu->current_servers = malloc(sizeof(pdu_server_entry*)*number_servers);
	pdu->add_server_entry = add_server_entry;
	pdu->create_message = slist_create_message;
	pdu->get_message_length = get_length_slist;
    pdu->free_pdu = free_slist;
	return pdu;
}



int free_server_entry(pdu_server_entry *server){
	if(server->name != NULL){
		free(server->name);
	}
	free(server);
	return 0;
}

int free_slist(pdu *pdu){

    if(pdu->number_servers !=0){
        for(int i = 0; i < pdu->number_servers;i++){
            free_server_entry(pdu->current_servers[i]);
        }
    }
    free(pdu->current_servers);
    free(pdu);
    return 0;
}


/*
 * pdu_JOIN
 */
int join_add_identity(pdu *pdu, char* identity){
	if(strlen(identity) == pdu->identity_length){
		pdu->identity = malloc((pdu->identity_length+1)*sizeof(char));
		strcpy(pdu->identity, identity);
	} else {
		perror("identity length missmatch\n");
		return -1;
	}
	return 0;
}


pdu* create_join(uint8_t identity_length){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_JOIN;
	pdu->identity_length = identity_length;
	pdu->add_identity = join_add_identity;
	pdu->create_message = join_create_message;
	pdu->get_message_length = get_length_join;
    pdu->free_pdu = free_join;
	return pdu;
}


int free_join(pdu *pdu){

    if(pdu->identity != NULL){
        free(pdu->identity);
    }
    free(pdu);
    return 0;
}




/*
 * pdu_PARTICIPANTS
 */

int participants_add_identities(pdu *pdu, char* identities){
	// need to implement check of length

	int lower = 0, found = 0;
	for(int i = 0 ; i < pdu->length;i++){
		//read until null termination (detect length)
		if(identities[i]=='\0'){

			pdu->identities[found] = malloc((i-lower+1)*sizeof(char));
			strncpy(pdu->identities[found], &identities[lower], i-lower+1);
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



pdu* create_participants(uint8_t number_identities, uint16_t length){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_PARTICIPANTS;
	pdu->number_identities = number_identities;
	pdu->length = length;
	pdu->identities = malloc(number_identities * sizeof(char*));
	pdu->add_identities = participants_add_identities;
	pdu->create_message = participants_create_message;
	pdu->get_message_length = get_length_participants;
    pdu->free_pdu = free_participants;
	return pdu;
}


int free_participants(pdu *pdu){

    if(pdu->number_identities !=0){
        for(int i = 0; i < pdu->number_identities;i++){
            free(pdu->identities[i]);
        }
    }

    free(pdu->identities);
    free(pdu);
    return 0;
}



/*
 * pdu_QUIT
 */

pdu* create_quit(void){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_QUIT;
	pdu->create_message = quit_create_message;
	pdu->get_message_length = get_length_quit;
    pdu->free_pdu = free_quit;
	return pdu;
}

int free_quit(pdu* pdu){
    free(pdu);
    return 0;
}

/*
 * MESS
 */


int mess_calc_checksum(pdu *pdu){

	return 0;
}


int mess_add_client_identity(pdu *pdu, char* client_identity){
	pdu->identity = malloc((pdu->identity_length+1)*sizeof(char));
	strcpy(pdu->identity, client_identity);

	return 0;
}


int mess_add_message(pdu *pdu, uint16_t message_length, uint32_t time_stamp, char* message){
	pdu->time_stamp = time_stamp;
	pdu->message_length = message_length;
	pdu->message = malloc((pdu->message_length+1)* sizeof(char));
	strcpy(pdu->message, message);

 	return 0;
}



pdu* create_mess(uint8_t identity_length, uint8_t checksum){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_MESS;
	pdu->identity_length = identity_length;
	pdu->checksum = checksum;
	pdu->add_message = mess_add_message;
	pdu->add_identity = mess_add_client_identity;
	pdu->calc_checksum = mess_calc_checksum;
	pdu->create_message = mess_create_message;
	pdu->get_message_length = get_length_mess;
    pdu->free_pdu = free_mess;
	return pdu;
}

int free_mess(pdu *pdu){

    if(pdu->identity_length !=0){
        free(pdu->identity); //identity corrent? does not have client_identity
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
int pjoin_add_client_identity(pdu *pdu, uint32_t time_stamp, char* client_identity){
	pdu->time_stamp = time_stamp;
	pdu->identity = malloc((pdu->identity_length+1)*sizeof(char));
	strcpy(pdu->identity, client_identity);
	return 0;
}


pdu* create_pjoin(uint8_t identity_length){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_PJOIN;
	pdu->identity_length = identity_length;
	pdu->add_client_identity_timestamp = pjoin_add_client_identity;
	pdu->create_message = pjoin_create_message;
	pdu->get_message_length = get_length_pjoin;
    pdu->free_pdu = free_pjoin;
	return pdu;
}

int free_pjoin(pdu *pdu){
    if(pdu->identity_length != 0){
        free(pdu->identity);
    }
    free(pdu);
    return 0;
}

/*
 * PLEAVE
 */

int pleave_add_client_identity(pdu *pdu, uint32_t time_stamp, char* client_identity){
	pdu->time_stamp = time_stamp;
	pdu->identity = malloc((pdu->identity_length+1)*sizeof(char));
	strcpy(pdu->identity, client_identity);
	return 0;
}


pdu* create_pleave(uint8_t identity_length){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_PLEAVE;
	pdu->identity_length = identity_length;
	pdu->add_client_identity_timestamp = pleave_add_client_identity;
	pdu->create_message = pleave_create_message;
	pdu->get_message_length = get_length_pleave;
    pdu->free_pdu = free_pleave;
	return pdu;
}

int free_pleave(pdu *pdu){
    if(pdu->identity_length != 0){
        free(pdu->identity);
    }
    free(pdu);
    return 0;
}

