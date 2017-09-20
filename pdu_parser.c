#include <stdio.h>
#include "pdu_parser.h"


char* pdu_reg_create_message(pdu_REG *self){

    char type[1];
    char port[4];
    char nameLen[255];
    sprintf(type, "%d", self->type);
    sprintf(nameLen, "%d", self->server_name_length);
    sprintf(port, "%d", self->tcp_port);

    char* mess = malloc(sizeof(char)*(strlen(type)+strlen(nameLen)+strlen(port)+strlen(self->server_name)));
    strcat(mess, type);
    strcat(mess, nameLen);
    strcat(mess, port);
    strcat(mess, self->server_name);
	return mess;
}

