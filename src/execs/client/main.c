/*
 * main.c
 *
 *  Created on: Oct 10, 2017
 *      Author: lgerber
 */


#define _GNU_SOURCE

#include "client.h"

int main (int argc, char*argv[]){

    //get the initial join address and assign default name server
    current_user* user = malloc(sizeof(struct current_user));
    chat_server* ns = malloc(sizeof(struct chat_server));
    strcpy(ns->server_name, NAME_SERVER);
    ns->port = NAME_SERVER_PORT;
    user->name_server = ns;

    int arg_status = parse_arguments(argc, argv, user);
    if(arg_status == -1){
        free(user);
        return EXIT_FAILURE;
    }

    user->join_status = JOIN_INITIAL;
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
        if(user->join_status == JOIN_INITIAL){
            if(user->server_type == TYPE_CHAT_SERVER){
                user->join_status = chat_loop(user);
                free(user->join_server);
            }else if(user->server_type == TYPE_NAME_SERVER){
                servers = request_chat_servers(user, servers);
                free(user->join_server);
                user->join_status = JOIN_FAIL;
            }
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
                servers = request_chat_servers(user, servers);
                continue;
            //exit command
            }else if(strcmp(input,"exit\n") == 0) {
                list_free(servers);
                break;
            //join command
            }else if(strncmp(input,"join ",5) == 0){
                user->join_status = join_server_in_list(user, input+5,servers);
                if (user->join_status == JOIN_FAIL){
                    printf("could not find server: %s", input);
                    continue;
                }else if(user->join_status == JOIN_SUCCESS){
                    continue;
                }else{
                    printf("Unexpected error\n");
                    list_free(servers);
                    break;
                }
            //connect command
            }else if(strncmp(input,"connect ",8) == 0){
                //TODO implement
                printf("\njoining %s\n", input+8);
                user->join_status = direct_connect(user, input+8);
                //join_status = chat_loop(cs);
                if (user->join_status == JOIN_FAIL){
                    printf("Unable to connect to server: %s", input+8);
                    continue;
                }else{
                    list_free(servers);
                    break;
                }
            //help command
            }else if(strncmp(input,"help",4) == 0){
                print_help();
                continue;
            //invalid command
            }else if(strncmp(input,"ns ",3) == 0){
                //TODO implement
                continue;
            }else{
                printf("unknown command\n");
                continue;
            }
        }

    }
    free(ns);
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
void print_usage_error(void){
    printf("\nInvalid arguments.\n");
    printf("Correct usage: client [user name] [ns|cs] [server name] [server port]");
}
int chat_loop(current_user *u) {

    io_handler* chat_server_com = create_tcp_client_communicator(u->join_server->address,
                                                                 u->join_server->port);
    chat_server_com->connect(chat_server_com, 5);

    pdu *join = create_join((uint8_t) strlen(u->identity));
    join->add_identity(join, u->identity);

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

list* request_chat_servers(current_user* u, list* server_list) {

    if(server_list != NULL){
        list_free(server_list);
    }
    list* servers = list_empty();
    list_set_mem_handler(servers,free);

    io_handler* name_server_com = create_tcp_client_communicator(u->name_server->server_name, u->name_server->port);
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
    printf("type 'help' to get a list of commands\n");
    printf(">");
}

void print_help(void){
    printf("\n'servers' updates the current active chat servers from the name server\n");
    printf("'join x' joins a chat server \"x\" from the name server list\n");
    printf("'connect serveraddress:port' connects directly to a chat server without using name server\n");
    printf("'ns serveraddress:port' sets the name server address\n");
    printf("'exit' shuts down the client\n");
}

int join_server_in_list(current_user* user, char* input,list* servers){
    list_position p = list_first(servers);
    do{
        if(p!=list_first(servers)){
            p=list_next(p);
        }
        chat_server* cs;
        cs = (chat_server *) list_inspect(p);
        if(cs != NULL){
            if(strncmp(input, cs->server_name, strlen(cs->server_name))==0){
                user->join_server = cs;
                user->join_status = chat_loop(user);
                if(user->join_status == JOIN_SUCCESS){
                    return JOIN_SUCCESS;
                }
            }
        }

    } while(!list_is_end(servers, p));

    return JOIN_FAIL;

}

int direct_connect(current_user* user, char* input){
    int i = 0;
    while(input[i] != ':'){
        i++;
    }
}