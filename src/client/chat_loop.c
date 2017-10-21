/*
 * chat_loop.c
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */

#include "chat_loop.h"

int server_hand_shake(io_handler *chat_server_com, current_user *u);

void chat_loop_cleanup(pthread_t *th, threadarg *arg, io_handler *com);

/**
 * This function controls a chat session the user starts from the menu.
 * First, a connection is established with the chat server, followed by
 * a launch of the input thread which will handle outgoing communication
 * with the chat server. This function then handles the receiving and
 * parsing and displaying of the incomming messages. It is also resposible
 * for getting the user to the correct state in the main program depending
 * on errors or user input.
 *
 * @param u current_user, Struct containing the chat server address and
 * user information.
 * @return int, The chat sessions ending state.
 */
int chat_loop(current_user *u) {

    io_handler* chat_server_com = create_tcp_client_communicator(u->join_server->address,
                                                                 u->join_server->port);
    pthread_t* thread_handle = malloc(sizeof(pthread_t));
    threadarg* arg = malloc(sizeof(struct threadarg));
    arg->com = chat_server_com;
    arg->status = malloc(sizeof(int*));
    *arg->status = ACTIVE;

    //connect to server
    if(chat_server_com->connect(chat_server_com, 5) == 0){

        //join server
        if((chat_server_com->status = server_hand_shake(chat_server_com, u) == JOIN_STATUS_CONTINUE)){

            //send thread
            pthread_create(thread_handle, NULL, sendThread, arg);

            //listen thread
            while (true) {

                pdu *receive_pdu = NULL;
                while ((chat_server_com->status == STATUS_RECEIVE_EMPTY ||
                       chat_server_com->status == 0) && *arg->status != DONE) {

                    //parse incomming pdu's
                    receive_pdu = parse_header(chat_server_com);
                }

                if (chat_server_com->status != STATUS_RECEIVE_OK) {
                    //parsing until receive error or done signal from send thread
                    if(*arg->status == DONE){
                        chat_loop_cleanup(thread_handle, arg, chat_server_com);
                        return JOIN_STATUS_CONTINUE;
                    }else if(*arg->status == DONE_EXIT){
                        chat_loop_cleanup(thread_handle, arg, chat_server_com);
                        return JOIN_STATUS_QUIT;
                    }else{
                        printf("\nsomething wrong with receive in Client\n");
                        chat_loop_cleanup(thread_handle, arg, chat_server_com);
                        return JOIN_STATUS_CONTINUE;
                    }
                }
                if (receive_pdu != NULL) {
                    //print messages in "else", server messages in "if" are
                    //printed in red.
                    if(receive_pdu->identity_length == 0 && receive_pdu->identity == NULL){
                    	fflush(stdout);
                        printf(RED"%s\n"RESET,receive_pdu->message);
                        receive_pdu->free_pdu(receive_pdu);
                        fflush(stdout);
                    }else{
                        fflush(stdout);
                        switch(receive_pdu->type){
                            case 16:
                                unix_to_localtime(receive_pdu->time_stamp);
                                printf("**%u:%s has joined the server**\n",receive_pdu->time_stamp, receive_pdu->identity);
                                receive_pdu->free_pdu(receive_pdu);
                                fflush(stdout);
                                break;
                            case 17:
                                unix_to_localtime(receive_pdu->time_stamp);
                                printf("**%u:%s has left the server**\n",receive_pdu->time_stamp, receive_pdu->identity);
                                receive_pdu->free_pdu(receive_pdu);
                                fflush(stdout);
                                break;
                            case 10:
                            	unix_to_localtime(receive_pdu->time_stamp);
                            	printf(" %s> %s \n",receive_pdu->identity, receive_pdu->message);
                                receive_pdu->free_pdu(receive_pdu);
                                fflush(stdout);
                                break;
                            default:
                                receive_pdu->free_pdu(receive_pdu);
                                fflush(stdout);
                                break;

                        }
                    }
                }
                //check for status changes from send thread.
                if(*arg->status == DONE){
                    chat_loop_cleanup(thread_handle, arg, chat_server_com);
                    return JOIN_STATUS_CONTINUE;
                }else if(*arg->status == DONE_EXIT){
                    chat_loop_cleanup(thread_handle, arg, chat_server_com);
                    return JOIN_STATUS_QUIT;
                }
                //reset receive status before next iteration
                chat_server_com->status = 0;
            }
        }else{
            printf("\nsomething wrong with receive in Client\n");
            chat_loop_cleanup(thread_handle,arg,chat_server_com);
        }
    }else{
        printf("Failed to connect to chat server\n");
    }

    chat_loop_cleanup(thread_handle,arg,chat_server_com);
    printf("exiting chat server\n");

    return JOIN_STATUS_CONTINUE;
}

/**
 * This function free's memory used by the chat loop.
 *
 * @param th p_thread_t, The sending thread handle.
 * @param arg threadarg, The threadarg struct passed to the sending thread.
 * @param com io_handler, The chat server communicator from chat loop.
 */
void chat_loop_cleanup(pthread_t *th, threadarg *arg, io_handler *com) {
    pthread_join(*th, NULL);
    free(th);
    free(arg->status);
    free(arg);
    com->close(com);
    free_tcp_client_communicator(com);
}

/**
 * This function initializes the agree'd starting premises for joining the
 * chat server by first sending a getlist pdu and receiving an slist pdu.
 * The function also presents this to the user, signifying that the session
 * is active.
 *
 * @param chat_server_com io_handler, The chat server communicator.
 * @param u current_user, Struct containing the chat server address and
 * user information.
 * @return int, Status of the getlist-slist starting communication.
 */
int server_hand_shake(io_handler *chat_server_com, current_user *u){

    pdu *join = create_join((uint8_t) strlen(u->identity));
    join->add_identity(join, u->identity);
    chat_server_com->send_pdu(chat_server_com, join);
    free_join(join);
    chat_server_com->status = 0;
    pdu *receive_partticipants = NULL;
    while (chat_server_com->status == STATUS_RECEIVE_EMPTY ||
           chat_server_com->status == 0) {
        receive_partticipants = parse_header(chat_server_com);
    }

    if (chat_server_com->status != STATUS_RECEIVE_OK) {

        return JOIN_STATUS_QUIT;
    }
    if (receive_partticipants != NULL) {
        printf("\nConnected to chat server at %s:%d\n", u->join_server->address, u->join_server->port);
        printf("Users currently in this chat:\n\n");
        for(int i = 0; i < receive_partticipants->number_identities;i++){
            printf("%s\n", receive_partticipants->identities[i]);
        }
        printf("\n");
        fflush(stdout);
        receive_partticipants->free_pdu(receive_partticipants);
        fflush(stdout);
    }
    return JOIN_STATUS_CONTINUE;
}

/**
 * This is the sending thread. It handles the input from the user and packets
 * these into pdu messages that are sent to the chat server. It also
 * handles local commands that the user might want to perform("quit" etc).
 * The chat loop and send thread communicates via the passed threadarg struct.
 * This struct contains the io_handler to the chat server and a status pointer
 * that controls the current chat session.
 *
 * @param data threadarg, This contains the io_handler and chat status pointer.
 * @return void.
 */
void * sendThread(void *data){

    threadarg* arg = data;

    //allocate resources for getline
    char *input;
    int bufsize = 65535;
    //uint16_t characters;
    input = (char *)malloc(bufsize * sizeof(char));
    if( input == NULL) {
        perror("Unable to allocate buffer\n");
        EXIT_FAILURE;
    }

    while(*arg->status != DONE && *arg->status != DONE_EXIT){

        memset(input, 0, (size_t) bufsize);
        fgets (input, bufsize, stdin);
        uint16_t length = (uint16_t) strlen(input);
        char*message = calloc(length, sizeof(char));
        memcpy(message, input, (size_t) (length - 1));



        if(strcmp(message, "quit") == 0){
            pdu* quit = create_quit();
            arg->com->send_pdu(arg->com, quit);
            quit->free_pdu(quit);
            *arg->status = DONE;
            free(message);
            break;
        }else if(strcmp(message, "exit") ==0){
            pdu* quit = create_quit();
            arg->com->send_pdu(arg->com, quit);
            quit->free_pdu(quit);
            *arg->status = DONE_EXIT;
            free(message);
            break;
        }else{
            pdu* mess = create_mess(0, 0);
            mess_add_message(mess, length, 0, message);
            mess_set_checksum(mess);
            arg->com->send_pdu(arg->com, mess);
            mess->free_pdu(mess);
        }
        free(message);
    }

    free(input);
    return NULL;
}
