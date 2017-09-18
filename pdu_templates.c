/*
 * pdu_templates.c
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */
#include "pdu_templates.h"

int get_type(pdu_wrapper pdu){
	return pdu.type;
}

int create_type(pdu_wrapper *pdu, int type){


	(*pdu).type = type;

	switch(type) {
	case 0:
		(*pdu).message = malloc(sizeof(pdu_REG));
		break;
	case 1:
		(*pdu).message = malloc(sizeof(pdu_ACK));
		break;
	case 2:
		(*pdu).message = malloc(sizeof(pdu_ALIVE));
		break;
	case 100:
		(*pdu).message = malloc(sizeof(pdu_NOTREG));
		break;
	case 3:
		(*pdu).message = malloc(sizeof(pdu_GETLIST));
		break;
	case 4:
		(*pdu).message = malloc(sizeof(pdu_SLIST));
		break;
	case 10:
		(*pdu).message = malloc(sizeof(pdu_MESS));
		break;
	case 11:
		(*pdu).message = malloc(sizeof(pdu_QUIT));
		break;
	case 12:
		(*pdu).message = malloc(sizeof(pdu_JOIN));
		break;
	case 16:
		(*pdu).message = malloc(sizeof(pdu_PJOIN));
		break;
	case 17:
		(*pdu).message = malloc(sizeof(pdu_PLEAVE));
		break;
	case 19:
		(*pdu).message = malloc(sizeof(pdu_PARTICIPANTS));
		break;
	default:
		//
		break;
	}

	return 0;
}


int free_type(pdu_wrapper *pdu){
	unsigned char type;

	if ((*pdu).message == NULL){
		perror("PDU is not configured configured");
		return -1;
	}

	type = (*pdu).type;

	switch(type) {
	case 0:
		if(((pdu_REG*)pdu->message)->server_name != NULL){
			free(((pdu_REG*)pdu->message)->server_name);
		}
		free(pdu->message);
		break;

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


pdu_wrapper* create_REG(unsigned servername_length, unsigned int tcp_port){
	pdu_wrapper *pdu = malloc(sizeof(pdu_wrapper));
	pdu->type = 0;
	create_type(pdu, 0);


	((pdu_REG*)pdu->message)->server_name_length = servername_length;
	((pdu_REG*)pdu->message)->tcp_port = tcp_port;
	((pdu_REG*)pdu->message)->add_server_name = add_server_name;
	return pdu;
}

int add_server_name(pdu_wrapper *pdu, char* server_name){
	if(strlen(server_name) == ((pdu_REG*)pdu->message)->server_name_length){
		((pdu_REG *)pdu->message)->server_name = malloc(((pdu_REG*)pdu->message)->server_name_length*sizeof(char));
		strcpy(((pdu_REG*)pdu->message)->server_name, server_name);
	} else {
		perror("server_name length missmatch\n");
		return -1;
	}
	return 0;
}





