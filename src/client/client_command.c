/*
 * client_command.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "client_command.h"

void print_main(void){
    printf("\n---Super chat client---\n");
    printf("type 'help' to get a list of commands\n");
    printf(">");
}

void print_usage_error(void){
    printf("\nInvalid arguments.\n");
    printf("Correct usage: client [user name] [ns|cs] [server name] [server port]");
}

void print_help(void){
    printf("\n'servers' updates the current active chat servers from the name server\n");
    printf("'join x' joins a chat server \"x\" from the name server list\n");
    printf("'connect serveraddress:port' connects directly to a chat server without using name server\n");
    printf("'ns serveraddress:port' sets the name server address\n");
    printf("'exit' shuts down the client\n");
}

int parse_arguments(int argc, char *argv[], current_user* u) {
    char* strtol_ptr;
    if(argc != 5){
        print_usage_error();
        return -1;
    }
    chat_server* join_server = malloc(sizeof(chat_server));
    memset(join_server->address,0,255);
    memset(u->identity,0,255);
    strcpy(u->identity, argv[1]);
    if(strcmp(argv[2], "ns") == 0){
        u->server_type = TYPE_NAME_SERVER;
    }else if(strcmp(argv[2], "cs") == 0){
        u->server_type = TYPE_CHAT_SERVER;
    }else{
        print_usage_error();
        return -1;
    }
    if(u->server_type == TYPE_CHAT_SERVER){
        strcpy(join_server->address,argv[3]);
        join_server->port = (uint16_t )strtol(argv[4],&strtol_ptr,10);
    }else if(u->server_type == TYPE_NAME_SERVER){
        memset(u->name_server->server_name, 0, 255);
        strcpy(u->name_server->server_name,argv[3]);
        u->name_server->port = (uint16_t )strtol(argv[4],&strtol_ptr,10);
    }
    u->join_server = join_server;

    return 0;
}

void get_list_to_user(pdu* slist, list* servers){
    list_position p = list_first(servers);
    printf("\nAvaliable chat servers from the name server\n");
    for(int i = 0; i< slist->number_servers;i++){

        chat_server* server = malloc(sizeof(chat_server));
        char address1[3], address2[3], address3[3], address4[3] = {0};
        //get address in string format
        sprintf(address1, "%d", slist->current_servers[i]->address[0]);
        sprintf(address2, "%d", slist->current_servers[i]->address[1]);
        sprintf(address3, "%d", slist->current_servers[i]->address[2]);
        sprintf(address4, "%d", slist->current_servers[i]->address[3]);
        strcpy(server->address, address1);
        strcat(server->address, ".");
        strcat(server->address, address2);
        strcat(server->address, ".");
        strcat(server->address, address3);
        strcat(server->address, ".");
        strcat(server->address, address4);

        strcpy(server->server_name, slist->current_servers[i]->name);
        server->port = slist->current_servers[i]->port;

        list_insert(p,server);
        p = list_next(p);
        printf("\nNAME: %s\nADDRESS: %d.%d.%d.%d\nPORT: %d\n",
               slist->current_servers[i]->name,
               slist->current_servers[i]->address[0],
               slist->current_servers[i]->address[1],
               slist->current_servers[i]->address[2],
               slist->current_servers[i]->address[3],
               slist->current_servers[i]->port);
    }
    slist->free_pdu(slist);
}

int set_name_server(current_user* user, const char* input){

    char address[strlen(input)];
    char* strtol_ptr;
    int status = 0;

    memset(address,0,strlen(input));

    int i = 0;
    //make sure the format is correct
    while(input[i] != '\n'){
        address[i] = input[i];
        if(input[i] == ':'){
            status = 1;
        }
        i++;
    }
    if(!status){
        printf("\nSpecify address with 'serveraddress:port'\n");
        return JOIN_STATUS_QUIT;
    }
    i = 0;
    while(input[i] != ':'){
        i++;
    }
    i++;
    //make sure port is integers
    while(input[i] != '\n'){
        if(!isdigit(input[i])){
            printf("\nPort number must be integers\n");
            return JOIN_STATUS_QUIT;
        }
        i++;
    }
    //copy data
    memset(user->name_server->address,0,255);
    i = 0;
    while(address[i] != ':'){
        user->name_server->address[i] = address[i];
        i++;
    }
    i++;
    user->name_server->port = (uint16_t )strtol(&address[i], &strtol_ptr, 10);
    printf("\nName server changed\n");
    return JOIN_STATUS_CONTINUE;
}