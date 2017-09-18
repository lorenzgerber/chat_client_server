/*
 * pdu_templates.c
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */
#include "pdu_templates.h"

int get_type(base_pdu pdu){
	return pdu.type;
}

int create_type(base_pdu pdu, int type){

	if (pdu.type != NULL){
		perror("PDU is already configured");
		return -1;
	}

	pdu.type = type;

	switch(type) {
	case 0:
		pdu.message = (pdu_REG) malloc(sizeof(pdu_REG));
		break;
	case 1:
		pdu.message = (pdu_ACK) malloc(sizeof(pdu_ACK));
		break;
	case 2:
		pdu.message = (pdu_ALIVE) malloc(sizeof(pdu_ALIVE));
		break;
	case 100:
		pdu.message = (pdu_NOTREG) malloc(sizeof(pdu_NOTREG));
		break;
	case 3:
		pdu.message = (pdu_GETLIST) malloc(sizeof(pdu_GETLIST));
		break;
	case 4:
		pdu.message = (pdu_SLIST) malloc(sizeof(pdu_SLIST));
		break;
	case 10:
		pdu.message = (pdu_MESS) malloc(sizeof(pdu_MESS));
		break;
	case 11:
		pdu.message = (pdu_QUIT) malloc(sizeof(pdu_QUIT));
		break;
	case 12:
		pdu.message = (pdu_JOIN) malloc(sizeof(pdu_JOIN));
		break;
	case 16:
		pdu.message = (pdu_PJOIN) malloc(sizeof(pdu_PJOIN));
		break;
	case 17:
		pdu.message = (pdu_PLEAVE) malloc(sizeof(pdu_PLEAVE));
		break;
	case 19:
		pdu.message = (pdu_PARTICIPANTS) malloc(sizeof(pdu_PARTICIPANTS));
		break;
	default:
		//
		break;
	}

	return 0;
}

int free_type(base_pdu pdu){
	unsigned char type;

	if (pdu.type == NULL){
		perror("PDU is not configured configured");
		return -1;
	}

	type = pdu.type;

	switch(type) {
	case 0:
		if(((pdu_REG)pdu.message).server_name != NULL){
			free((pdu_REG)pdu.message);
		}
		free(pdu.message);
		break;

	case 4:
		int number_servers = ((pdu_SLIST)pdu.message).number_servers;
		if(number_servers !=0){
			for(int i = 0; i < number_servers;i++){
				pdu_server_entry *current_server = ((pdu_SLIST)pdu.message).current_servers[i];
				free_server_entry(current_server);
			}
		}
		free(pdu.message);
		break;
	case 10:
		pdu_MESS pdu_mess = ((pdu_MESS)pdu.message);
		if(pdu_mess.message != NULL){
			free(pdu_mess);
		}
		if(pdu_mess.client_identity != NULL){
			free(pdu_mess.client_identity);
		}
		free(pdu.message);
		break;
	case 12:
		pdu_JOIN pdu_join = ((pdu_JOIN)pdu.message);
		if (pdu_join.identity != NULL){
			free(pdu_join.identity);
		}
		free(pdu.message);
		break;
	case 16:
		pdu_PJOIN pdu_pjoin = ((pdu_PJOIN)pdu.message);
		if (pdu_pjoin.client_identity != NULL){
			free(pdu_pjoin.client_identity);
		}
		free(pdu.message);
		break;
	case 17:
		pdu_PLEAVE pdu_pleave = ((pdu_PLEAVE)pdu.message);
		if (pdu_pleave.client_identity != NULL){
			free(pdu_pleave.client_identity);
		}
		free(pdu.message);
		break;
	case 19:
		pdu_PARTICIPANTS pdu_participants = ((pdu_PARTICIPANTS)pdu.message);
		if (pdu_participants.identities != NULL){
			free(pdu_participants.identities);
		}
		free(pdu.message);
		break;
	default:
		free(pdu.message);
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



