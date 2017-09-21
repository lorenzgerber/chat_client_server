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

int calc_word_padding(uint32_t length){
	int padding = 4 - (length % 4);
	if (padding == 4){
		padding = 0;
	}

	return padding;
}


uint32_t get_length_REG(pdu_REG *pdu){
    uint32_t  length_of_reg = (uint32_t) (LENGTH_OP +
                                          LENGTH_SERVER_NAME_LENGTH +
                                          LENGTH_PORT +
										  pdu->server_name_length +
										  calc_word_padding(pdu->server_name_length));
	return length_of_reg;
}

uint32_t get_length_ALIVE(pdu_ALIVE *pdu){
	uint32_t length_of_alive = 4;
	return length_of_alive;
}

uint32_t get_length_ACK(pdu_ACK *pdu){
	uint32_t length_of_ack = 4;
	return length_of_ack;
}

uint32_t get_length_NOTREG(pdu_NOTREG *pdu){
	uint32_t length_of_notreg = 4;
	return length_of_notreg;
}

uint32_t get_length_GETLIST(pdu_GETLIST *pdu){
	uint32_t length_of_getlist = 4;
	return length_of_getlist;
}

uint32_t get_length_SLIST(pdu_SLIST *pdu){

	// fixed length
    uint32_t length_of_slist =
    		LENGTH_OP +
			LENGTH_PAD +
			LENGTH_NUMBER_SERVER;

    // variable length
    for(int i = 0; i < pdu->number_servers; i++){
        length_of_slist += LENGTH_ADDRESS +
                           LENGTH_PORT +
                           LENGTH_NUMBER_CLIENTS +
                           LENGTH_SERVER_NAME_LENGTH +
						   pdu->current_servers[i]->name_length +
						   calc_word_padding(pdu->current_servers[i]->name_length);

    }
	return length_of_slist;
}

uint32_t get_length_JOIN(pdu_JOIN *pdu){
    uint32_t length_of_join = (uint32_t) (LENGTH_OP +
                                          LENGTH_IDENTITY_LENGTH +
                                          LENGTH_PAD * 2 +
										  pdu->identity_length +
										  calc_word_padding(pdu->identity_length));
	return length_of_join;
}

uint32_t get_length_PARTICIPANTS(pdu_PARTICIPANTS *pdu){
	// todo
    uint32_t length_of_participants = LENGTH_OP +
                                      LENGTH_NUMBER_IDENTITIES +
                                      LENGTH_MESSAGE_LENGTH;

    for(int i = 0; i < pdu->number_identities; i++){

        length_of_participants += (strlen(pdu->identities[i]) + 1);
    }

    length_of_participants += length_of_participants +
                              calc_word_padding(length_of_participants);

    return length_of_participants;
}

uint32_t get_length_QUIT(pdu_QUIT *pdu){
	uint32_t length_of_quit = 4;
	return length_of_quit;
}

uint32_t get_length_MESS(pdu_MESS *pdu){
    uint32_t length_of_mess = LENGTH_OP +
                              LENGTH_PAD +
                              LENGTH_IDENTITY_LENGTH +
                              LENGTH_CHECKSUM +
                              LENGTH_MESSAGE_LENGTH +
                              LENGTH_PAD * 2 +
                              LENGTH_TIME;
    length_of_mess += pdu->message_length +
                      calc_word_padding(length_of_mess);
    if(pdu->identity_length > 0){
        length_of_mess += pdu->identity_length +
                          calc_word_padding(length_of_mess);
    }


	// todo
	return length_of_mess;
}

uint32_t get_length_PJOIN(pdu_PJOIN *pdu){
	// todo
    uint32_t length_of_pjoin = (uint32_t) (LENGTH_OP +
                                           LENGTH_IDENTITY_LENGTH +
                                           LENGTH_PAD * 2 +
                                           LENGTH_TIME +
                                           pdu->identity_length +
                                           calc_word_padding(pdu->identity_length));
	return length_of_pjoin;
}

uint32_t get_length_PLEAVE(pdu_PLEAVE *pdu){

	// variable part
	uint32_t length_client_identity = (uint32_t)
                                      (LENGTH_IDENTITY_LENGTH +
                                       calc_word_padding(LENGTH_IDENTITY_LENGTH));

	// fixed part
	uint32_t length = LENGTH_OP
			+ LENGTH_IDENTITY_LENGTH
			+ LENGTH_PAD * 2
			+ LENGTH_TIME
			+ length_client_identity;


	return length;
}


