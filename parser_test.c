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


int main(int argc, char*argv[]){

	pdu_parse();

	// create REG message with args server name length and tcp-port
	pdu_REG *REG_MESSAGE = create_REG(9,2000);
	// configure server name
	REG_MESSAGE->add_server_name(REG_MESSAGE,"arschloch");
	// test access
	printf("%s", REG_MESSAGE->server_name);
	// free memory
	free_pdu_reg(REG_MESSAGE);


	return 0;
}


