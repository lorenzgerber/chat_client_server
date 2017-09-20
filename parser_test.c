/*
 * parser_test.c
 *
 *  Created on: Sep 18, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdu_parser.h"
#include "pdu_templates.h"
#include "pdu_creator.h"


int main(int argc, char*argv[]){


	// Example create REG and add server name
	pdu_REG *REG_MESSAGE = create_REG(9,2000);
	REG_MESSAGE->add_server_name(REG_MESSAGE,"arschloch");
	printf("%s\n", REG_MESSAGE->server_name);
	printf("%s\n", REG_MESSAGE->create_message(REG_MESSAGE));
	free_pdu_reg(REG_MESSAGE);


	// Example create SLIST and populate with one server entry
	pdu_SLIST *SLIST_MESSAGE = create_SLIST(1);
	uint8_t address[4] = {127,0,0,1};
	pdu_server_entry* server1 = create_server_entry(address,2000,10,4);
	server1->add_server_name(server1, "depp");
	SLIST_MESSAGE->add_server_entry(SLIST_MESSAGE, server1);
	printf("%s\n", SLIST_MESSAGE->current_servers[0]->name);
	free_pdu_slist(SLIST_MESSAGE);


	//Example create PARTICIPANTS
	pdu_PARTICIPANTS *PARTICIPANTS = create_PARTICIPANTS(3, 15);
	PARTICIPANTS->add_identities(PARTICIPANTS, "bull\0shit\0crap\0");
	for(int i = 0; i < PARTICIPANTS->number_identities;i++){
		printf("%s\n", PARTICIPANTS->identities[i]);
	}
	free_pdu_participants(PARTICIPANTS);





	return 0;
}


