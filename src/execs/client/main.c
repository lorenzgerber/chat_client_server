/*
 * main.c
 *
 *  Created on: Oct 10, 2017
 *      Author: lgerber
 */


#define _GNU_SOURCE
#include "client.h"

int parse_arguments(int argc, char *argv[], current_user* u);
void print_usage_error(void);


int main (int argc, char*argv[]){
    //get the initial join address
    current_user* user = malloc(sizeof(struct current_user));
    int arg_status = parse_arguments(argc, argv, user);
    if(arg_status == -1){
        free(user);
        EXIT_FAILURE;
    }
    //join status -2 indicates initial connect
    int join_status = -2;
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
        //first time connect from argument line
        if(join_status == -2){
            join_status = chat_loop(user);
            free(user->join_server);
        //all other iterations
        }else{
            //get command from user
            print_welcome();
            memset(input, 0, (size_t) bufsize);
            characters = getline(&input, (size_t *) &bufsize, stdin);

            //match input command
            if(characters > bufsize){
                printf("input too long\n");
                continue;
                //servers command
            }else if(strcmp(input,"servers\n") == 0){
                if(servers != NULL){
                    list_free(servers);
                }
                servers = request_chat_servers();
                continue;
                //exit command
            }else if(strcmp(input,"exit\n") == 0) {
                list_free(servers);
                break;
                //join command
            }else if(strncmp(input,"join ",5) == 0){
                list_position p = list_first(servers);
                do{
                    if(p!=list_first(servers)){
                        p=list_next(p);
                    }
                    chat_server* cs;
                    cs = (chat_server *) list_inspect(p);
                    if(cs != NULL){
                        printf("\n%s\n",cs->server_name);
                        if(strncmp(input+5, cs->server_name, strlen(cs->server_name))==0){
                            user->join_server = cs;
                            join_status = chat_loop(user);
                            if(join_status == 0){
                                continue;
                            }
                        }
                    }

                } while(!list_is_end(servers, p) && join_status == -1);
                if (join_status == -1){
                    printf("Chatserver not in your current chat server list: %s", input+5);
                    continue;
                }else{
                    list_free(servers);
                    break;
                }
                //connect command
            }else if(strncmp(input,"connect ",8) == 0){
                printf("\njoining %s\n", input+8);
                //join_status = chat_loop(cs);
                if (join_status == -1){
                    printf("Server not found: %s", input+8);
                    continue;
                }else{
                    list_free(servers);
                    break;
                }
                //invalid command
            }else{
                printf("unknown command\n");
                continue;
            }
        }

    }
    free(user);
    free(input);
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
        u->server_type = 0;
    }else if(strcmp(argv[2], "cs") == 0){
        u->server_type = 1;
    }else{
        print_usage_error();
        return -1;
    }
    strcpy(join_server->address,argv[3]);
    join_server->port = (uint16_t )strtol(argv[4],&strtol_ptr,10);

    u->join_server = join_server;

    return 0;
}
void print_usage_error(void){

}
int chat_loop(current_user *u) {

    io_handler* chat_server_com = create_tcp_client_communicator(u->join_server->address,
                                                                 u->join_server->port);
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
    }
    print_participants(participants);
    participants->free_pdu(participants);

    pdu* quit = create_quit();
    chat_server_com->send_pdu(chat_server_com, quit);
    quit->free_pdu(quit);
    free_tcp_client_communicator(chat_server_com);
    printf("exiting chat server\n");
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
    printf("'servers' updates the current active chat servers from the name server\n");
    printf("'join x' joins a chat server \"x\" from the name server list\n");
    printf("'connect serveraddress:port' connects directly to a chat server\n");
    printf("'exit' shuts down the client\n");
    printf(">");
}
