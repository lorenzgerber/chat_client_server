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


	pdu_wrapper *REG_MESSAGE = create_REG(9,2000);
	((pdu_REG*)REG_MESSAGE->message)->add_server_name(REG_MESSAGE,"arschlocsfasdh");
	printf("%s", ((pdu_REG*)REG_MESSAGE->message)->server_name);
	free_type(REG_MESSAGE);


	return 0;
}


