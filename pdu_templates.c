/*
 * pdu_templates.c
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */
#include "pdu_templates.h"

int get_type(void *message){
	return ((pdu_prototype*) message)->type;
}


int free_type(pdu_prototype *pdu){
	unsigned char type;

	type = (*pdu).type;

	switch(type) {

	case 4:


		if(((pdu_SLIST *)pdu->message)->number_servers !=0){
			for(int i = 0; i < ((pdu_SLIST *)pdu->message)->number_servers;i++){
				free_server_entry(((pdu_SLIST*)pdu->message)->current_servers[i]);
			}
		}
		free(pdu->message);
		break;
	case 10:

		if(((pdu_MESS*)pdu->message)->message != NULL){
			free(pdu->message);
		}
		if(((pdu_MESS*)pdu->message)->client_identity != NULL){
			free(((pdu_MESS*)pdu->message)->client_identity);
		}
		free(pdu->message);
		break;
	case 12:
		if (((pdu_JOIN*)pdu->message)->identity != NULL){
			free(((pdu_JOIN*)pdu->message)->identity);
		}
		free(pdu->message);
		break;
	case 16:
		if (((pdu_PJOIN*)pdu->message)->client_identity != NULL){
			free(((pdu_PJOIN*)pdu->message)->client_identity);
		}
		free(pdu->message);
		break;
	case 17:
		if (((pdu_PLEAVE*)pdu->message)->client_identity != NULL){
			free(((pdu_PLEAVE*)pdu->message)->client_identity);
		}
		free(pdu->message);
		break;
	case 19:
		if (((pdu_PARTICIPANTS*)pdu->message)->identities != NULL){
			free(((pdu_PARTICIPANTS*)pdu->message)->identities);
		}
		free(pdu->message);
		break;
	default:
		free(pdu->message);
		break;
	}

	return 0;
}

int free_server_entry(pdu_server_entry server){
	if(server.name != NULL){
		free(server.name);
	}
	return 0;
}


pdu_REG* create_REG(unsigned servername_length, unsigned int tcp_port){
	pdu_REG *pdu = malloc(sizeof(pdu_REG));
	pdu->type = PDU_REG;
	pdu->server_name_length = 10;

	pdu->server_name_length = servername_length;
	pdu->tcp_port = tcp_port;
	pdu->add_server_name = add_server_name;
	return pdu;
}

int add_server_name(pdu_REG *pdu, char* server_name){
	if(strlen(server_name) == pdu->server_name_length){
		pdu->server_name = malloc(pdu->server_name_length*sizeof(char));
		strcpy(pdu->server_name, server_name);
	} else {
		perror("server_name length missmatch\n");
		return -1;
	}
	return 0;
}

int free_pdu_reg(pdu_REG *pdu){

	if(pdu->server_name != NULL){
		free(pdu->server_name);
	}
	free(pdu);
	return 0;
}





