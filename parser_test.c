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
	pdu_create();


	pdu_REG *REG_MESSAGE = create_REG(9,2000);
	REG_MESSAGE->add_server_name(REG_MESSAGE,"arschloch");
	printf("%s", REG_MESSAGE->server_name);
	free_pdu_reg(REG_MESSAGE);


	return 0;
}


