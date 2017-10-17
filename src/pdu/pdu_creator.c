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
/**
 * create_reg
 *
 * This function creates a pdu of the typ REG. It requires the
 * length of the sending hostname and its port number. The function
 * also assigns needed functions to calculate its length, free
 * memory, print, serialize and register a servername.
 *
 * @param servername_length this number must match a registered server
 * @param tcp_port the port number
 * @return pdu REG
 */
pdu* create_reg(uint8_t servername_length, uint16_t tcp_port){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_REG;
	pdu->server_name_length = servername_length;
	pdu->tcp_port = tcp_port;
	pdu->add_server_name = reg_add_server_name;
	pdu->create_message = reg_create_message;
	pdu->get_message_length = get_length_reg;
	pdu->print = print_reg;
	pdu->free_pdu = free_reg;
	return pdu;
}

/**
 * This function is called from a REG pdu. It adds
 * a server to the pdu
 *
 * @param pdu The REG pdu to add the server to
 * @param server_name The name of the server
 * @return 0
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

/**
 * This function is called from a REG pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling REG pdu
 * @return 0
 */
int free_reg(pdu *pdu){

	if(pdu->server_name != NULL){
		free(pdu->server_name);
	}
	free(pdu);
	return 0;
}


/*
 * pdu_ACK
 */
/**
 * create_ack
 *
 * This function creates a pdu of the typ ACK. It requires id
 * of the client that is to recieive it. The function
 * also assigns needed functions to calculate its length, free
 * memory, print and serialize into a message_byte_array.
 *
 * @param id_number id of the client
 * @return pdu ACK
 */
pdu* create_ack(uint16_t id_number){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_ACK;
	pdu->id_number = id_number;
	pdu->create_message = ack_create_message;
	pdu->get_message_length = get_length_ack;
	pdu->print = print_ack;
	pdu->free_pdu = free_ack;

	return pdu;
}

/**
 * This function is called from an ACK pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling ACK pdu
 * @return 0
 */
int free_ack(pdu* pdu){
    free(pdu);
    return 0;
}


/*
 * pdu_ALIVE
 */
/**
 * create_alive
 *
 * This function creates a pdu of the typ ALIVE. It requires id
 * of the sending nameserver and the number of clients
 * in the session. The function also assigns needed functions to
 * calculate its length, free memory, print and serialize into a
 * message_byte_array.
 *
 * @param number_clients The number of clients in the session
 * @param id_number id of the server
 * @return pdu ALIVE
 */
pdu* create_alive(uint8_t number_clients, uint16_t id_number){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_ALIVE;
	pdu->number_clients = number_clients;
	pdu->id_number = id_number;
	pdu->create_message = alive_create_message;
	pdu->get_message_length = get_length_alive;
	pdu->print = print_alive;
	pdu->free_pdu = free_alive;
	return pdu;
}

/**
 * This function is called from an ALIVE pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling ALIVE pdu
 * @return 0
 */
int free_alive(pdu* pdu){
    free(pdu);
    return 0;
}


/*
 * pdu_NOTREG
 */
/**
 * create_notreg
 *
 * This function creates a pdu of the typ NOTREG. It requires id
 * of the sending nameserver. The function also assigns needed
 * functions to calculate its length, free memory, print and
 * serialize into a message_byte_array.
 *
 * @param id_number id of nameserver
 * @return pdu NOTREG
 */
pdu* create_notreg(uint16_t id_number){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_NOTREG;
	pdu->id_number = id_number;
	pdu->create_message = notreg_create_message;
	pdu->get_message_length = get_length_notreg;
	pdu->print = print_notreg;
    pdu->free_pdu = free_notreg;
	return pdu;
}

/**
 * This function is called from an NOTREG pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling NOTREG pdu
 * @return 0
 */
int free_notreg(pdu* pdu){
    free(pdu);
    return 0;
}


/*
 * pdu_GETLIST
 */
/**
 * create_getlist
 *
 * This function creates a pdu of the typ GETLIST.
 *
 * The function also assigns needed functions to calculate
 * its length, free memory, print and serialize into a
 * message_byte_array.
 *
 * @return pdu GETLIST
 */
pdu* create_getlist(void){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_GETLIST;
	pdu->create_message = getlist_create_message;
	pdu->get_message_length = get_length_getlist;
	pdu->print = print_getlist;
    pdu->free_pdu = free_getlist;
	return pdu;
}

/**
 * This function is called from an GETLIST pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling GETLIST pdu
 * @return 0
 */
int free_getlist(pdu* pdu){
    free(pdu);
    return 0;
}


/*
 * pdu_SLIST
 */
/**
 * create_slist
 *
 * This function creates a pdu of the typ SLIST. It requires the
 * number of servers in the pdu. The function also assigns needed
 * functions to calculate its length, free memory, print, serialize
 * and add a pdu_server_entry.
 *
 * @param number_servers number of servers in the pdu
 * @return pdu SLIST
 */
pdu* create_slist(uint16_t number_servers){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_SLIST;
	pdu->number_servers = number_servers;
	pdu->server_assigned = 0;
	pdu->current_servers = malloc(sizeof(pdu_server_entry*)*number_servers);
	pdu->add_server_entry = add_server_entry;
	pdu->create_message = slist_create_message;
	pdu->get_message_length = get_length_slist;
	pdu->print=print_slist;
	pdu->free_pdu = free_slist;
	return pdu;
}
/**
 * This function creates a new server entry that can be added
 * to an SLIST pdu. It requires the address, port, name length
 * and the number of clients on the server.
 *
 * @param address Adress of the serverentry
 * @param port Port of the serverentry
 * @param number_clients The number of clients on the server entry
 * @param name_length The length of the servers name
 * @return pdu_server_entry to be added in SLIST
 */
pdu_server_entry* create_server_entry(uint8_t address[4],
									  uint16_t port,
									  uint8_t number_clients,
									  uint8_t name_length){
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

/**
 * This function is called by create_server_entry to add a server
 * name in the entry.
 *
 * @param pdu The calling pdu_server_entry
 * @param server_name The name of the server
 * @return 0
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

/**
 * This function is called by an SLIST pdu to add a server entry to it
 *
 * @param pdu The calling SLIST pdu
 * @param server_entry The server entry to be added
 * @return The number the server entry has in the pdu
 */
int add_server_entry(pdu *pdu, pdu_server_entry* server_entry){
	if(pdu->server_assigned == pdu->number_servers){
		perror("all servers already assigned\n");
		return -1;
	}
	pdu->current_servers[pdu->server_assigned] = server_entry;
	pdu->server_assigned++;
 	return pdu->server_assigned;
}

/**
 * This function is called from an SLIST pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling SLIST pdu
 * @return 0
 */
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

/**
 * This function is called by free_slist to free memory used
 * by server entries in the pdu
 *
 * @param server
 * @return
 */
int free_server_entry(pdu_server_entry *server){
	if(server->name != NULL){
		free(server->name);
	}
	free(server);
	return 0;
}


/*
 * pdu_JOIN
 */
/**
 * create_join
 *
 * This function creates a pdu of the typ JOIN. It requires the
 * length the joining client. The function also assigns needed
 * functions to calculate its length, free memory, print,
 * serialize and add the identity.
 *
 * @param identity_length Length of identity
 * @return pdu JOIN
 */
pdu* create_join(uint8_t identity_length){
	pdu *pdu = malloc(sizeof(struct pdu));

    pdu->identity = NULL;
	pdu->type = PDU_JOIN;
	pdu->identity_length = identity_length;
	pdu->add_identity = join_add_identity;
	pdu->create_message = join_create_message;
	pdu->get_message_length = get_length_join;
	pdu->print = print_join;
	pdu->free_pdu = free_join;
	return pdu;
}
/**
 * This function is called from a JOIN pdu and
 * adds an identity to it.
 *
 * @param pdu The calling JOIN pdu
 * @param identity A string with the identity
 * @return 0
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

/**
 * This function is called from a JOIN pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling JOIN pdu
 * @return 0
 */
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
/**
 * create_participants
 *
 * This function creates a pdu of the typ PARTICIPANTS. It requires the
 * number of participants in the session and the length of the field
 * containing all of those identities. The function also assigns needed
 * functions to calculate its length, free memory, print,
 * serialize and add identities.
 *
 * @param length Length of identities field
 * @param number_identities Number of identities in the participants pdu
 * @return pdu PARTICIPANTS
 */
pdu* create_participants(uint8_t number_identities, uint16_t length){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_PARTICIPANTS;
	pdu->number_identities = number_identities;
	pdu->length = length;
	pdu->identities = malloc(number_identities * sizeof(char*));
	pdu->add_identities = participants_add_identities;
	pdu->create_message = participants_create_message;
	pdu->get_message_length = get_length_participants;
	pdu->print = print_participants;
	pdu->free_pdu = free_participants;
	return pdu;
}

/**
 * This function is called from a PARTICIPANTS pdu. it adds
 * the identites field to the pdu
 *
 * @param pdu The calling PARTICIPANTS pdu
 * @param identities A string containing the identities field
 * @return 0
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

/**
 * This function is called from a PARTICIPANTS pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling PARTICIPANTS pdu
 * @return 0
 */
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
/**
 * create_quit
 *
 * This function creates a pdu of the typ QUIT. The function
 * assigns needed functions to calculate its length, free memory, print,
 * serialize into a message_byte_array.
 *
 * @return pdu QUIT
 */
pdu* create_quit(void){
	pdu *pdu = malloc(sizeof(struct pdu));
	pdu->type = PDU_QUIT;
	pdu->create_message = quit_create_message;
	pdu->get_message_length = get_length_quit;
	pdu->print = print_quit;
    pdu->free_pdu = free_quit;
	return pdu;
}

/**
 * This function is called from a QUIT pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling QUIT pdu
 * @return 0
 */
int free_quit(pdu* pdu){
    free(pdu);
    return 0;
}

/*
 * MESS
 */
/**
 * create_mess
 *
 * This function creates a pdu of the typ MESS. It requires the
 * length of the sender and the checksum. The function also assigns needed
 * functions to calculate its length, free memory, print, serialize,
 * calculate checksum and add identity & message.
 *
 * @param length Length of the sending identity
 * @param checksum The calculated checksum
 * @return pdu MESS
 */
pdu* create_mess(uint8_t identity_length, uint8_t checksum){
	pdu *pdu = malloc(sizeof(struct pdu));

    pdu->identity = NULL;
    pdu->time_stamp = 0;
    pdu->message = NULL;

	pdu->type = PDU_MESS;
	pdu->identity_length = identity_length;
	pdu->checksum = checksum;
	pdu->add_message = mess_add_message;
	pdu->add_identity = mess_add_client_identity;
	pdu->verify_checksum = mess_verify_checksum;
	pdu->set_checksum = mess_set_checksum;
	pdu->create_message = mess_create_message;
	pdu->get_message_length = get_length_mess;
	pdu->print = print_mess;
	pdu->free_pdu = free_mess;
	return pdu;
}

/**
 * This function is called from a MESS pdu and adds an identity to it.
 *
 * @param pdu The calling MESS pdu
 * @param client_identity A string containing the identity to add
 * @return 0
 */
int mess_add_client_identity(pdu *pdu, char* client_identity){
	pdu->identity = malloc((strlen(client_identity)+1)*sizeof(char));
	pdu->identity_length = strlen(client_identity);
	strcpy(pdu->identity, client_identity);

	return 0;
}

/**
 * This function is called from a MESS pdu and adds a message to it
 *
 * @param pdu The calling MESS pdu
 * @param message_length Length of the message
 * @param time_stamp The message timestamp
 * @param message A string containing the message
 * @return 0
 */
int mess_add_message(pdu *pdu, uint16_t message_length, uint32_t time_stamp, char* message){
	pdu->time_stamp = time_stamp;
	pdu->message_length = message_length;
	pdu->message = malloc((pdu->message_length+1)* sizeof(char));
	strcpy(pdu->message, message);

	return 0;
}

/**
 * This function is called from a MESS pdu and validates the checksum
 * on its message
 *
 * @param pdu The calling MESS pdu
 * @return 0
 */
int mess_verify_checksum(pdu *pdu){
	uint16_t message_length = 0;
	int sum = 0;
	uint8_t checksum = 0;
	message_byte_array *MBA;
	MBA = pdu->create_message(pdu);
	message_length = get_length_mess(pdu);
	for(int i = 0; i < message_length; i++){
		sum += MBA->array[i];
	}

	free_message_byte_array(MBA);

	checksum = sum % 255;

	if(checksum != 0){
		return -1;
	}
	return 0;
}

/**
 * This function is called from a MESS pdu and sets the checksum
 * on its message
 *
 * @param pdu The calling MESS pdu
 * @return 0
 */
uint8_t mess_set_checksum(pdu *pdu){

	uint16_t message_length = 0;
	int sum = 0;
	pdu->checksum = 0;
	message_byte_array *MBA;
	MBA = pdu->create_message(pdu);
	message_length = get_length_mess(pdu);
	for(int i = 0; i < message_length; i++){
		sum += MBA->array[i];
	}
	free_message_byte_array(MBA);

	pdu->checksum = 255 - ((sum - pdu->checksum) % 255);

	return 0;
}

/**
 * This function is called from a MESS pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling MESS pdu
 * @return 0
 */
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
/**
 * create_pjoin
 *
 * This function creates a pdu of the typ PJOIN. It requires the
 * length of the user. The function also assigns needed
 * functions to calculate its length, free memory, print, serialize
 * and add client id & timestamp
 *
 * @param identity_length
 * @return
 */
pdu* create_pjoin(uint8_t identity_length){
	pdu *pdu = malloc(sizeof(struct pdu));
    pdu->time_stamp = 0;
    pdu->identity = NULL;
	pdu->type = PDU_PJOIN;
	pdu->identity_length = identity_length;
	pdu->add_client_identity_timestamp = pjoin_add_client_identity;
	pdu->create_message = pjoin_create_message;
	pdu->get_message_length = get_length_pjoin;
	pdu->print = print_pjoin;
	pdu->free_pdu = free_pjoin;
	return pdu;
}
/**
 * This function is called by a PJOIN pdu to add a client the time
 * it joined.
 *
 * @param pdu The calling PJOIN pdu
 * @param time_stamp THe timestamp the user joined
 * @param client_identity A string containing the identity of the user
 * @return 0
 */
int pjoin_add_client_identity(pdu *pdu,
							  uint32_t time_stamp,
							  char* client_identity){
	pdu->time_stamp = time_stamp;
	pdu->identity = malloc((pdu->identity_length+1)*sizeof(char));
	strcpy(pdu->identity, client_identity);
	return 0;
}

/**
 * This function is called from a PJOIN pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling PJOIN pdu
 * @return 0
 */
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
/**
 * pleave
 *
 * This function creates a pdu of the type PLEAVE. It requires the
 * length of the user. The function also assigns needed
 * functions to calculate its length, free memory, print, serialize
 * and add client id & timestamp
 *
 * @param identity_length
 * @return
 */
pdu* create_pleave(uint8_t identity_length){
	pdu *pdu = malloc(sizeof(struct pdu));
    pdu->time_stamp = 0;
    pdu->identity = NULL;
	pdu->type = PDU_PLEAVE;
	pdu->identity_length = identity_length;
	pdu->add_client_identity_timestamp = pleave_add_client_identity;
	pdu->create_message = pleave_create_message;
	pdu->get_message_length = get_length_pleave;
	pdu->print = print_pleave;
	pdu->free_pdu = free_pleave;
	return pdu;
}

/**
 * This function is called by a PLEAVE pdu to add a client the time
 * it left.
 *
 * @param pdu The calling PLEAVE pdu
 * @param time_stamp THe timestamp the user left
 * @param client_identity A string containing the identity of the user
 * @return 0
 */
int pleave_add_client_identity(pdu *pdu,
							   uint32_t time_stamp,
							   char* client_identity){
	pdu->time_stamp = time_stamp;
	if(pdu->identity_length != 0){
		pdu->identity = malloc((pdu->identity_length+1)*sizeof(char));
		strcpy(pdu->identity, client_identity);
	}

	return 0;
}

/**
 * This function is called from a PLEAVE pdu. It frees the
 * memory used by the pdu.
 *
 * @param pdu The calling PLEAVE pdu
 * @return 0
 */
int free_pleave(pdu *pdu){
    if(pdu->identity_length != 0){
        free(pdu->identity);
    }
    free(pdu);
    return 0;
}

