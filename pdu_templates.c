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




