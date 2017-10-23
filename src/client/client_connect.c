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

/**
 * This function establishes a temporary udp connection to the users current
 * name server defined in the current_user struct. It then sends a getlist
 * request and parses the response slist pdu. The list of active chat servers
 * is updated.
 *
 * @param u current_user, Struct containing the name server information.
 * @param server_list list, This is the chat server list that is used by
 * the menus "servers" and "join" commands.
 * @return list, A new list containing updated information.
 */
list* request_chat_servers(current_user* u, list* server_list) {

    if(server_list != NULL){
        list_position p = list_first(server_list);
        do{
            chat_server* cs = NULL;
            cs = (chat_server *) list_inspect(p);
            if(cs != NULL){
                if(cs->address != NULL){
                    free(cs->address);
                }
                if(cs->server_name != NULL){
                    free(cs->server_name);
                }
            }
            p=list_next(p);

        } while(list_inspect(p) != NULL);
        list_free(server_list);
    }
    list* servers = list_empty();
    list_set_mem_handler(servers,free);

    io_handler* name_server_com = create_tcp_client_communicator(u->name_server->address, u->name_server->port);
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
    }
    get_list_to_user(slist, servers);
    free_tcp_client_communicator(name_server_com);
    return servers;

}

/**
 * This function is used when the user invokes the "join" command. It searches
 * the active chat server list for a chat server with the passed name. The user
 * may update this list with the "servers" command should the list be
 * out of date.
 *
 * @param user current_user, This structs "join_server" is updated.
 * @param input char, The argument passed with "join" from the menu.
 * @param servers list, The list of active chat servers.
 * @return int, Status variable.
 */
int join_server_in_list(current_user* user, char* input,list* servers){
    list_position p = list_first(servers);
    do{
        chat_server* cs;
        cs = (chat_server *) list_inspect(p);
        if(cs != NULL){
            if(strncmp(input, cs->server_name, strlen(cs->server_name))==0){
                user->join_server = cs;
                user->join_status = chat_loop(user);
                if(user->join_status == JOIN_STATUS_CONTINUE){
                    return JOIN_STATUS_CONTINUE;
                }else{
                    return JOIN_STATUS_QUIT;
                }
            }
        }
        p=list_next(p);

    } while(list_inspect(p) != NULL);
    printf("\nCould not find the server in the serverlist\n");
    return JOIN_STATUS_CONTINUE;

}

/**
 * This function is used to directly connect to an address without using its
 * name. This is done via the "connect" command from the menu.
 *
 * @param user current_user, This structs join_server is updated.
 * @param input char, The argument passed with the "connect" command.
 * @return int, Status variable.
 */
int direct_connect(current_user* user, const char* input){

    chat_server* cs = malloc(sizeof(struct chat_server));
    char* address = calloc(0,sizeof(char)*255);
    cs->address = address;
    char* strtol_ptr;
    int status = 0;

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
        return JOIN_STATUS_CONTINUE;
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
            return JOIN_STATUS_CONTINUE;
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
    if(user->join_status == JOIN_STATUS_CONTINUE){
        free(cs->address);
        free(cs);
        return JOIN_STATUS_CONTINUE;
    }else{
        free(cs->address);
        free(cs);
        return JOIN_STATUS_QUIT;
    }
}
