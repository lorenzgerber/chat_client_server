#include <stdio.h>
#include "pdu_parser.h"

int pdu_create(void){
	printf("I'm the pdu creator\n");
	return 0;
}

int pdu_parse(void){
	printf("I'm the pdu parser\n");
	// request first word
	// determine type
	// create pdu struct
	// write initial data
	// decide: finished, read_rest, read_iter
	// call read_rest or read_iter
	// decide: finished, read footer
	// finished
	return 0;
}
