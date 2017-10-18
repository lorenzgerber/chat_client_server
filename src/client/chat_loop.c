/*
 * chat_loop.c
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */

#include "chat_loop.h"

int server_hand_shake(io_handler *chat_server_com, current_user *u);

void chat_loop_cleanup(pthread_t *th, threadarg *arg, io_handler *com);

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
        if((chat_server_com->status = server_hand_shake(chat_server_com, u) == JOIN_SUCCESS)){

            //send thread
            pthread_create(thread_handle, NULL, sendThread, arg);

            //listen thread
            while (true) {

                pdu *receive_pdu = NULL;
                while (chat_server_com->status == STATUS_RECEIVE_EMPTY ||
                       chat_server_com->status == 0) {

                    //parse incomming pdu's
                    receive_pdu = parse_header(chat_server_com);
                }

                if (chat_server_com->status != STATUS_RECEIVE_OK) {
                    //parsing until receive error or done signal from send thread
                    if(*arg->status != DONE){
                        printf("\nsomething wrong with receive in Client\n");
                    }
                    chat_loop_cleanup(thread_handle, arg, chat_server_com);
                    return JOIN_ABORT;
                }
                if (receive_pdu != NULL) {
                    //print messages, server messages in red
                    if(receive_pdu->identity_length == 0 && receive_pdu->identity == NULL){
                    	fflush(stdout);
                        printf(RED"%s\n"RESET,receive_pdu->message);
                        fflush(stdout);
                    }else{
                    	fflush(stdout);
                        printf("%s>%s \n",receive_pdu->identity, receive_pdu->message);
                        fflush(stdout);
                        //receive_pdu->print(receive_pdu);
                        receive_pdu->free_pdu(receive_pdu);
                    }

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

    return 0;
}

void chat_loop_cleanup(pthread_t *th, threadarg *arg, io_handler *com) {
    pthread_join(*th, NULL);
    free(th);
    free(arg->status);
    free(arg);
    com->close(com);
    free_tcp_client_communicator(com);
}

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

        return JOIN_ABORT;
    }
    if (receive_partticipants != NULL) {

        receive_partticipants->print(receive_partticipants);
        fflush(stdout);
        receive_partticipants->free_pdu(receive_partticipants);
        fflush(stdout);
    }
    return JOIN_SUCCESS;
}

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

    while(true){

        memset(input, 0, (size_t) bufsize);
        fgets (input, bufsize, stdin);
        uint16_t length = strlen(input);
        char*message = calloc(length, sizeof(char));
        memcpy(message, input, length-1);



        if(strcmp(message, "exit\n") == 0){
            pdu* quit = create_quit();
            arg->com->send_pdu(arg->com, quit);
            quit->free_pdu(quit);
            *arg->status = DONE;
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
