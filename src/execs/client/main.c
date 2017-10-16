/*
 * main.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
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
            print_main();
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
                continue;
            //connect command
            }else if(strncmp(input,"connect ",8) == 0){
                printf("\njoining %s\n", input+8);
                user->join_status = direct_connect(user, input+8);
                continue;
            //help command
            }else if(strncmp(input,"help",4) == 0){
                print_help();
                continue;
            //invalid command
            }else if(strncmp(input,"ns ",3) == 0){
                user->join_status = set_name_server(user, input+3);
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















