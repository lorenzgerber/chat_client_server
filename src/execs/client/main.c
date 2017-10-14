/*
 * main.c
 *
 *  Created on: Oct 10, 2017
 *      Author: lgerber
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <socket_templates.h>
#include <socket_creator.h>
#include <linked_list.h>

void print_slist_servers(pdu* slist);
void print_welcome(void);

void request_chat_servers(io_handler *com);

int main (int argc, char*argv[]){

	//list* servers = list_empty();

    char *input;
    int bufsize = 255;
    __ssize_t characters;
    int command = 1;
    input = (char *)malloc(bufsize * sizeof(char));
    if( input == NULL) {
        perror("Unable to allocate buffer\n");
        EXIT_FAILURE;
    }

    io_handler* name_server_com = create_tcp_client_communicator("itchy.cs.umu.se", 1337);
    name_server_com->connect(name_server_com, 5);
    name_server_com->status = 0;

    while(command){

        print_welcome();
        memset(input, 0, (size_t) bufsize);
        characters = getline(&input, (size_t *) &bufsize, stdin);

        if(characters > bufsize){
            printf("input too long\n");
            continue;
        }else if(strcmp(input,"/servers\n") == 0){
            request_chat_servers(name_server_com);
            name_server_com->status = 0;
            continue;
        }else if(strcmp(input,"/exit\n") == 0){
            break;
        }
        printf("%zu characters were read.\n",characters);
        printf("You typed: '%s'\n",input);
        command = 0;
    }

    free(input);
	free_tcp_client_communicator(name_server_com);
}

void request_chat_servers(io_handler *com) {

    pdu *getlist = create_getlist();

    com->send_pdu(com, getlist);
    getlist->free_pdu(getlist);

    pdu* slist = NULL;
    // looping until an error happens or we get data
    while(com->status == STATUS_RECEIVE_EMPTY || com->status == 0){
        slist = parse_header(com);
    }
    if(com->status != STATUS_RECEIVE_OK){

        printf("\nsomething wrong with receive in Client\n");
        return;
    }else{
        print_slist_servers(slist);
        return;
    }
}

void print_slist_servers(pdu* slist){
    for(int i = 0; i< slist->number_servers;i++){
        printf("\nName: %s, address: %d.%d.%d.%d, port: %d\n",
               slist->current_servers[i]->name,
               slist->current_servers[i]->address[0],
               slist->current_servers[i]->address[1],
               slist->current_servers[i]->address[2],
               slist->current_servers[i]->address[3],
               slist->current_servers[i]->port);
    }
    slist->free_pdu(slist);
}

void print_welcome(void){
    printf("\nWelcome to the super chat client!\n");
    printf("'/help' displays a list of commands\n");
    printf("Type the name or address of an active chat server to join\n");
    printf(">");
}