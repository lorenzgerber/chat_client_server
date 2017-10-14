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
typedef struct chat_server{
    char name[255];
    char address[20];
    uint16_t port;
}chat_server;
void get_list_to_user(pdu* slist, list* servers);
void print_welcome(void);
list* request_chat_servers();
int chat_loop(chat_server *server);
int main (int argc, char*argv[]){

    int joined;
    char *input;
    int bufsize = 255;
    __ssize_t characters;
    input = (char *)malloc(bufsize * sizeof(char));
    if( input == NULL) {
        perror("Unable to allocate buffer\n");
        EXIT_FAILURE;
    }
    list* servers = list_empty();
    while(true){

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
            continue;
        }else if(strcmp(input,"exit\n") == 0) {
            list_free(servers);
            break;
        }else if(strncmp(input,"join ",5) == 0){
            printf("\njoining %s\n", input+5);
            list_position p = list_first(servers);
            do{
                if(p!=list_first(servers)){
                    p=list_next(p);
                }
                chat_server* cs;
                cs = (chat_server *) list_inspect(p);
                printf("\n%s\n",cs->name);
                if(strncmp(input+5, cs->name, strlen(cs->name))==0){
                    printf("\n**%s**\n",cs->address);
                    joined = chat_loop(cs);
                    printf("\nsuccess\n");
                }
            } while(!list_is_end(servers, p));
            list_free(servers);
            break;
        }else{
            printf("unknown command\n");
            continue;
        }
    }

    free(input);

}

int chat_loop(chat_server *server) {

    io_handler* chat_server_com = create_tcp_client_communicator(server->address, server->port);
    chat_server_com->connect(chat_server_com, 5);
    pdu *join = create_join(8);
    join->add_identity(join, "identity");
    chat_server_com->send_pdu(chat_server_com, join);
    free_join(join);

    chat_server_com->status = 0;
    pdu* participants = NULL;
    // looping until an error happens or we get data
    while(chat_server_com->status == STATUS_RECEIVE_EMPTY || chat_server_com->status == 0){
        participants = parse_header(chat_server_com);
    }
    if(chat_server_com->status != STATUS_RECEIVE_OK){
        printf("\nsomething wrong with receive in Client\n");
        free_tcp_client_communicator(chat_server_com);
    }else{
        free_tcp_client_communicator(chat_server_com);
    }
    print_participants(participants);
    return 0;
}

list* request_chat_servers() {

    list* servers = list_empty();
    list_set_mem_handler(servers,free);

    io_handler* name_server_com = create_tcp_client_communicator("itchy.cs.umu.se", 1337);
    name_server_com->connect(name_server_com, 5);
    pdu *getlist = create_getlist();
    name_server_com->send_pdu(name_server_com, getlist);
    free_getlist(getlist);

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
        char address1[3] = {0};
        char address2[3] = {0};;
        char address3[3] = {0};;
        char address4[3] = {0};;

        chat_server* server = malloc(sizeof(chat_server));
        strcpy(server->name, slist->current_servers[i]->name);
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
