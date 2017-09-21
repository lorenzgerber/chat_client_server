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

uint32_t get_length_REG(pdu_REG pdu){
	// todo
    uint32_t  length_of_reg = (uint32_t) (8 - (pdu.server_name_length % 4));
	return length_of_reg;
}

uint32_t get_length_ALIVE(pdu_ALIVE pdu){
	uint32_t length_of_alive = 4;
	return length_of_alive;
}

uint32_t get_length_ACK(pdu_ACK pdu){
	uint32_t length_of_ack = 4;
	return length_of_ack;
}

uint32_t get_length_GETLIST(pdu_GETLIST pdu){
	uint32_t length_of_getlist = 4;
	return length_of_getlist;
}

uint32_t get_length_SLIST(pdu_SLIST pdu){
	// todo
    uint32_t length_of_slist = 4;
    for(int i = 0; i < pdu.number_servers; i++){
        length_of_slist += 12 - (pdu.current_servers[i]->name_length % 4);

    }
	return 0;
}

uint32_t get_length_JOIN(pdu_JOIN pdu){
	// todo
	return 0;
}

uint32_t get_length_PARTICIPANTS(pdu_PARTICIPANTS pdu){
	// todo
	return 0;
}

uint32_t get_length_QUIT(pdu_QUIT pdu){
	uint32_t length_of_quit = 4;
	return length_of_quit;
}

uint32_t get_length_MESS(pdu_MESS pdu){
	// todo
	return 0;
}

uint32_t get_length_PJOIN(pdu_PJOIN pdu){
	// todo
	return 0;
}

uint32_t get_length_PLEAVE(pdu_PLEAVE pdu){
	// todo
	return 0;
}


