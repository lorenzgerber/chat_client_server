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

/**
 * This function prints the main message to the user that shows up each time
 * the program waits for input.
 */
void print_main(void){
    printf("\n---Super chat client---\n");
    printf("type 'help' to get a list of commands\n");
    printf(">");
}

/**
 * This function informs the user how to correctly start the program if wrong
 * or invalid parameters were passed.
 */
void print_usage_error(void){
    printf("\nInvalid arguments.\n");
    printf("Correct usage: client [user name] [ns|cs] [server name] [server port]");
}

/**
 * This function prints a list over the menu and chat sessions different
 * system commands.
 */
void print_help(void){
    printf("\n'servers' updates the current active chat servers from the name server\n");
    printf("'join x' joins a chat server \"x\" from the name server list\n");
    printf("'connect serveraddress:port' connects directly to a chat server without using name server\n");
    printf("'ns serveraddress:port' sets the name server address\n");
    printf("'quit' leaves an active chat session\n");
    printf("'exit' shuts down the client\n");
}

/**
 * This function parses the program arguments and passes them into a
 * current_user struct. It also overwrites the users default name server
 * if a name server was passed as the argument. The default name server is
 * defined in "client.h".
 *
 * @param argc int, Number of arguments from main.
 * @param argv char**, The arguments from main.
 * @param u current_user, The current_user struct to be filled with
 * information from argument line.
 * @return int, Status of parse_arguments.
 */
int parse_arguments(int argc, char *argv[], current_user* u) {
    char* strtol_ptr;
    if(argc != 5){
        print_usage_error();
        return -1;
    }
    u->identity = strdup(argv[1]);
    //memset(u->identity,0,255);
    //memcpy(u->identity, argv[1], strlen(argv[1])+1);
    if(strcmp(argv[2], "ns") == 0){
        u->server_type = TYPE_NAME_SERVER;
    }else if(strcmp(argv[2], "cs") == 0){
        u->server_type = TYPE_CHAT_SERVER;
    }else{
        print_usage_error();
        return -1;
    }
    if(u->server_type == TYPE_CHAT_SERVER){
        char* address = malloc( sizeof(char)*256);
        chat_server* join_server = malloc(sizeof(chat_server));
        join_server->address = address;
        memcpy(join_server->address,argv[3],strlen(argv[3])+1);
        join_server->port = (uint16_t )strtol(argv[4],&strtol_ptr,10);
        u->join_server = join_server;
    }else if(u->server_type == TYPE_NAME_SERVER){
        //memset(u->name_server->address, 0, 255);
        strcpy(u->name_server->address,argv[3]);
        u->name_server->port = (uint16_t )strtol(argv[4],&strtol_ptr,10);
    }


    return 0;
}

/**
 * This function fills a list of chat_servers from a parsed slist pdu and
 * also displays the lists information to the user.
 * @param slist pdu, Parsed slist pdu containing the active chat servers
 * obtained from the name server.
 * @param servers list, The list to be refreshed with information from
 * the pdu.
 */
void get_list_to_user(pdu* slist, list* servers){
    list_position p = list_first(servers);
    printf("\nAvaliable chat servers from the name server\n");
    for(int i = 0; i< slist->number_servers;i++){

        //char* address = calloc(0,sizeof(char)*256);
        //char* server_name = calloc(0,sizeof(char)*256);
        chat_server* server = malloc(sizeof(chat_server)+512);
        char* temp = calloc(0,sizeof(char)*8);
        //server->server_name = server_name;
        //server->address = address;
        char address1[3], address2[3], address3[3], address4[3] = {0};
        //get address in string format
        sprintf(address1, "%d", slist->current_servers[i]->address[0]);
        sprintf(address2, "%d", slist->current_servers[i]->address[1]);
        sprintf(address3, "%d", slist->current_servers[i]->address[2]);
        sprintf(address4, "%d", slist->current_servers[i]->address[3]);
        //slist->current_servers[1]->
        temp[0] = (unsigned char)address1;
        temp[1] = '.';
        temp[2] = (unsigned char)address2;
        temp[3] = '.';
        temp[4] = (unsigned char)address3;
        temp[5] = '.';
        temp[6] = (unsigned char)address4;

        server->address = strdup(temp);

        //memset(server->server_name,0,255);
        //strcpy(server->server_name, slist->current_servers[i]->name);
        server->server_name = strdup(slist->current_servers[i]->name);
        server->port = slist->current_servers[i]->port;

        list_insert(p,server);
        p = list_next(p);
        printf("\nNAME: %s\nADDRESS: %s\nPORT: %d\n",
               slist->current_servers[i]->name,
               temp,
               slist->current_servers[i]->port);
        free(temp);
    }
    slist->free_pdu(slist);
}

/**
 * This function changes the active name server for the current user.
 *
 * @param user current_user, This struct has the current name server
 * information.
 * @param input char, The string passed by user with the "ns" command.
 * @return int, the state of the menu session.
 */
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

/**
 * This function formats a unix timestamp into a more user friendly format
 * before displaying them in chat sessions.
 * @param unix_time uint32_t, Timestamp in unix format.
 * @return int, 0.
 */
int unix_to_localtime(uint32_t unix_time){
	time_t     now = unix_time;
	struct tm  ts;
	char       buf[80];

	// Get current time
	time(&now);

	// Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
	ts = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ts);
	printf("%s", buf);
	return 0;
}
