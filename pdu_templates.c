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

int specify_type(base_pdu pdu, int type){

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
	case 3:
		pdu.message = (pdu_NOTREG) malloc(sizeof(pdu_NOTREG));
		break;
	case 4:
		pdu.message = (pdu_GETLIST) malloc(sizeof(pdu_GETLIST));
		break;
	case 10:
		pdu.message = (pdu_SLIST) malloc(sizeof(pdu_SLIST));
		break;
	case 11:
		pdu.message = (pdu_MESS) malloc(sizeof(pdu_MESS));
		break;
	case 12:
		pdu.message = (pdu_QUIT) malloc(sizeof(pdu_QUIT));
		break;
	case 16:
		pdu.message = (pdu_JOIN) malloc(sizeof(pdu_JOIN));
		break;
	case 17:
		pdu.message = (pdu_PJOIN) malloc(sizeof(pdu_PJOIN));
		break;
	case 19:
		pdu.message = (pdu_PLEAVE) malloc(sizeof(pdu_PLEAVE));
		break;
	case 100:
		pdu.message = (pdu_PARTICIPANTS) malloc(sizeof(pdu_PARTICIPANTS));
		break;
	default:
		//
		break;
	}

	return 0;
}



