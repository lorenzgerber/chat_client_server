/*
 * pdu_templates.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "pdu_templates.h"

/**
 * calc_word_padding
 *
 * Helper function to calculate
 * how much padding is needed
 * to fill up to a word.
 * @param length of message to pad
 * @return padding number of bytes
 */
int calc_word_padding(uint32_t length){
	int padding = 4 - (length % 4);
	if (padding == 4){
		padding = 0;
	}

	return padding;
}

/**
 * get_length_reg
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param reg pdu
 */
uint32_t get_length_reg(pdu *pdu){
    uint32_t  length_of_reg = (uint32_t) (LENGTH_OP +
                                          LENGTH_SERVER_NAME_LENGTH +
                                          LENGTH_PORT +
										  pdu->server_name_length +
										  calc_word_padding(pdu->server_name_length));
	return length_of_reg;
}

/**
 * get_length_alive
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param alive pdu
 */
uint32_t get_length_alive(pdu *pdu){
	uint32_t length_of_alive = 4;
	return length_of_alive;
}

/**
 * get_length_ack
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param ack pdu
 */
uint32_t get_length_ack(pdu *pdu){
	uint32_t length_of_ack = 4;
	return length_of_ack;
}

/**
 * get_length_notreg
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param notreg pdu
 */
uint32_t get_length_notreg(pdu *pdu){
	uint32_t length_of_notreg = 4;
	return length_of_notreg;
}

/**
 * get_length_getlist
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param getlist pdu
 */
uint32_t get_length_getlist(pdu *pdu){
	uint32_t length_of_getlist = 4;
	return length_of_getlist;
}

/**
 * get_length_slist
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param slist pdu
 */
uint32_t get_length_slist(pdu *pdu){

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

/**
 * get_length_join
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param join pdu
 */
uint32_t get_length_join(pdu *pdu){
    uint32_t length_of_join = (uint32_t) (LENGTH_OP +
                                          LENGTH_IDENTITY_LENGTH +
                                          LENGTH_PAD * 2 +
										  pdu->identity_length +
										  calc_word_padding(pdu->identity_length));
	return length_of_join;
}

/**
 * get_length_participants
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param participants pdu
 */
uint32_t get_length_participants(pdu *pdu){
    uint32_t length_of_participants = LENGTH_OP +
                                      LENGTH_NUMBER_IDENTITIES +
                                      LENGTH_MESSAGE_LENGTH;

    for(int i = 0; i < pdu->number_identities; i++){

        length_of_participants += (strlen(pdu->identities[i]) + 1);
    }

    length_of_participants += calc_word_padding(length_of_participants);

    return length_of_participants;
}

/**
 * get_length_quit
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param quit pdu
 */
uint32_t get_length_quit(pdu *pdu){
	uint32_t length_of_quit = 4;
	return length_of_quit;
}

/**
 * get_length_mess
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param mess pdu
 */
uint32_t get_length_mess(pdu *pdu){
    uint32_t length_of_mess = LENGTH_OP +
                              LENGTH_PAD +
                              LENGTH_IDENTITY_LENGTH +
                              LENGTH_CHECKSUM +
                              LENGTH_MESSAGE_LENGTH +
                              LENGTH_PAD * 2 +
                              LENGTH_TIME;
    length_of_mess += pdu->message_length +
    		calc_word_padding(pdu->message_length);
    if(pdu->identity_length > 0){
        length_of_mess += pdu->identity_length +
        		calc_word_padding(pdu->identity_length);
    }

	return length_of_mess;
}

/**
 * get_length_pjoin
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param pjoin pdu
 */
uint32_t get_length_pjoin(pdu *pdu){
    uint32_t length_of_pjoin = (uint32_t) (LENGTH_OP +
                                           LENGTH_IDENTITY_LENGTH +
                                           LENGTH_PAD * 2 +
                                           LENGTH_TIME +
                                           pdu->identity_length +
                                           calc_word_padding(pdu->identity_length));
	return length_of_pjoin;
}

/**
 * get_length_pleave
 *
 * Helper function to calculate
 * the length of a pdu'a serialized message
 * using the pdu struct.
 * @param pleave pdu
 */
uint32_t get_length_pleave(pdu *pdu){

	// variable part
	uint32_t length_client_identity = (uint32_t)
                                      (pdu->identity_length +
                                       calc_word_padding(pdu->identity_length));

	// fixed part
	uint32_t length = LENGTH_OP
			+ LENGTH_IDENTITY_LENGTH
			+ LENGTH_PAD * 2
			+ LENGTH_TIME
			+ length_client_identity;


	return length;
}


