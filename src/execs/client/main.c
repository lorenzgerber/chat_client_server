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

void get_list_to_user(pdu* slist, list* servers);
void print_welcome(void);
list* request_chat_servers();

typedef struct chat_server{
    char name[255];
    char address[20];
    uint16_t port;
}chat_server;

int main (int argc, char*argv[]){



    char *input;
    int bufsize = 255;
    __ssize_t characters;
    input = (char *)malloc(bufsize * sizeof(char));
    if( input == NULL) {
        perror("Unable to allocate buffer\n");
        EXIT_FAILURE;
    }

    while(true){
        list* servers = list_empty();
        print_welcome();
        memset(input, 0, (size_t) bufsize);
        characters = getline(&input, (size_t *) &bufsize, stdin);

        if(characters > bufsize){
            printf("input too long\n");
            continue;
        }else if(strcmp(input,"servers\n") == 0){
            if(servers != NULL){
                list_free(servers);
            }
            servers = request_chat_servers();
            list_free(servers);
            continue;
        }else if(strcmp(input,"exit\n") == 0) {
            list_free(servers);
            break;
        }else if(strncmp(input,"join ",5) == 0){
            printf("\njoining %s\n", input+5);
            list_free(servers);
            continue;
        }else{
            printf("unkown command\n");
            list_free(servers);
            continue;
        }
    }

    free(input);

}

list* request_chat_servers() {

    list* servers = list_empty();
    list_set_mem_handler(servers,free);

    io_handler* name_server_com = create_tcp_client_communicator("itchy.cs.umu.se", 1337);
    name_server_com->connect(name_server_com, 5);
    pdu *getlist = create_getlist();
    name_server_com->send_pdu(name_server_com, getlist);
    getlist->free_pdu(getlist);

    name_server_com->status = 0;
    pdu* slist = NULL;
    // looping until an error happens or we get data
    while(name_server_com->status == STATUS_RECEIVE_EMPTY || name_server_com->status == 0){
        slist = parse_header(name_server_com);
    }
    if(name_server_com->status != STATUS_RECEIVE_OK){
        printf("\nsomething wrong with receive in Client\n");
        free_tcp_client_communicator(name_server_com);
        return servers;
    }else{
        get_list_to_user(slist, servers);
        free_tcp_client_communicator(name_server_com);
        return servers;
    }
}

void get_list_to_user(pdu* slist, list* servers){
    for(int i = 0; i< slist->number_servers;i++){

        chat_server* server = malloc(sizeof(chat_server));
        strcpy(server->name, slist->current_servers[i]->name);
        strcpy(server->address, (char *) slist->current_servers[i]->address);
        server->port = slist->current_servers[i]->port;

        list_insert(list_first(servers),server);
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

void print_welcome(void){
    printf("\nWelcome to the super chat client!\n");
    printf("'servers' updates the current active chat servers\n");
    printf("'join x' joins the chat server x\n");
    printf("'exit' shuts down the client\n");
    printf(">");
}
