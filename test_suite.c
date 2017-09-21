/*
 * test_suite.c
 *
 *  Created on: Sep 21, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdu_parser.h"
#include "pdu_templates.h"
#include "pdu_creator.h"
#include "message_byte_array.h"


int main(int argc, char*argv[]){

	uint32_t length = 0;
	message_byte_array *MBA;

	/*
	 * Test REG
	 */

	// Create REG struct and add 1 server name
	pdu_REG *REG = create_REG(10,2000);
	REG->add_server_name(REG,"servername");

	// Create REG byte stream message to send
	MBA = REG->create_message(REG);
	length = get_length_REG(REG);

	for(int i = 0;i < length;i++){
		printf("%d, ", MBA->array[i]);
	}

	// free REG and MBA
	free_pdu_reg(REG);
	free_message_byte_array(MBA);

	/*
	 * Test ALIVE
	 */
	// Create ALIVE Struct
	//pdu_ALIVE *ALIVE = create_ALIVE(100, 10000);

	// Create ALIVE byte stream message to send
	//MBA =


	// Example create SLIST and populate with one server entry
	pdu_SLIST *SLIST = create_SLIST(1);
	uint8_t address[4] = {127,0,0,1};
	pdu_server_entry* server1 = create_server_entry(address,2000,4,10);
	server1->add_server_name(server1, "servername");
	SLIST->add_server_entry(SLIST, server1);


	length = get_length_SLIST(SLIST);
	for(int i = 0; i < length; i++){
		printf("%d, ", MBA->array[i]);
	}

	printf("%s\n", SLIST->current_servers[0]->name);
	free_pdu_slist(SLIST);



	//Example create PARTICIPANTS
	pdu_PARTICIPANTS *PARTICIPANTS = create_PARTICIPANTS(3, 15);
	PARTICIPANTS->add_identities(PARTICIPANTS, "bull\0shit\0crap\0");
	for(int i = 0; i < PARTICIPANTS->number_identities;i++){
		printf("%s\n", PARTICIPANTS->identities[i]);
	}
	free_pdu_participants(PARTICIPANTS);


	pdu_MESS *MESS = create_MESS(8, 45);
	MESS->add_message(MESS, 64, 1505933137, "Now this is ä very funny story that I gonna unfold for you here.");
	MESS->add_client_identity(MESS, "bullcrap");
	printf("%s\n", MESS->message);
	free_pdu_mess(MESS);

	message_byte_array *array = create_message_byte_array(10);
    //char* cp = "z";
	array->add_uint32(array, 12312);
	array->add_uint32(array, 23234);
    array->add_chars(array,"abcd", 1);

	for(int i = 0;i < 12;i++){
		printf("%d, ", array->array[i]);
	}




	return 0;
}

