/*
 * client_connect.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "client_connect.h"

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

int join_server_in_list(current_user* user, char* input,list* servers){
    list_position p = list_first(servers);
    do{
        /*if(p!=list_first(servers)){
            p=list_next(p);
        }*/
        chat_server* cs;
        cs = (chat_server *) list_inspect(p);
        if(cs != NULL){
            if(strncmp(input, cs->server_name, strlen(cs->server_name))==0){
                user->join_server = cs;
                user->join_status = chat_loop(user);
                if(user->join_status == JOIN_SUCCESS){
                    return JOIN_SUCCESS;
                }else{
                    printf("\nError connecting to server\n");
                    return JOIN_ABORT;
                }
            }
        }
        p=list_next(p);

    } while(list_inspect(p) != NULL);
    printf("\nCould not find the server in the serverlist\n");
    return JOIN_ABORT;

}

int direct_connect(current_user* user, const char* input){

    chat_server* cs = malloc(sizeof(struct chat_server));
    char address[strlen(input)];
    char* strtol_ptr;
    int status = 0;

    memset(address,0,strlen(input));
    memset(cs->address,0,255);
    int i = 0;

    //make sure format is correct
    while(input[i] != '\n'){
        address[i] = input[i];
        if(input[i] == ':'){
            status = 1;
        }
        i++;
    }
    if(!status){
        printf("\nSpecify address with 'serveraddress:port'\n");
        return JOIN_ABORT;
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
            return JOIN_ABORT;
        }
        i++;
    }
    //copy data
    i = 0;
    while(address[i] != ':'){
        cs->address[i] = address[i];
        i++;
    }
    i++;
    cs->port = (uint16_t )strtol(&address[i], &strtol_ptr, 10);
    user->join_server = NULL;
    user->join_server = cs;
    user->join_status = chat_loop(user);
    if(user->join_status == JOIN_SUCCESS){
        free(cs);
        return JOIN_SUCCESS;
    }else{
        free(cs);
        printf("\nError connecting to server\n");
        return JOIN_ABORT;
    }
}
