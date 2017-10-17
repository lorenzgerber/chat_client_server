/*
 * chat_loop.c
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */


#include "chat_loop.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int senddd;

int chat_loop(current_user *u) {

    io_handler* chat_server_com = create_tcp_client_communicator(u->join_server->address,
                                                                 u->join_server->port);
    pthread_t* thread_handle;
    thread_handle = malloc(sizeof(pthread_t));
    threadarg* arg = malloc(sizeof(struct threadarg));
    arg->com = chat_server_com;
    arg->u = u;
    arg->status = (int *) 1;

    senddd = 1;

    //join server
    if(chat_server_com->connect(chat_server_com, 5) == 0){
        pdu *join = create_join((uint8_t) strlen(u->identity));
        join->add_identity(join, u->identity);

        chat_server_com->send_pdu(chat_server_com, join);
        free_join(join);
        chat_server_com->status = 0;
        pdu *receive_part = NULL;
        while (chat_server_com->status == STATUS_RECEIVE_EMPTY ||
               chat_server_com->status == 0) {
            receive_part = parse_header(chat_server_com);
        }
        //pthread_mutex_unlock(&mutex);
        if (chat_server_com->status != STATUS_RECEIVE_OK) {
            printf("\nsomething wrong with receive in Client\n");
            //free_tcp_client_communicator(chat_server_com);
            chat_server_com->close(chat_server_com);
            //free_tcp_client_communicator(chat_server_com);
            return JOIN_FAIL;
        }
        if (receive_part != NULL) {
            //*arg->reading = 0;
            receive_part->print(receive_part);
            receive_part->free_pdu(receive_part);
        }
        pthread_create(thread_handle, NULL, sendThread, arg);
        // looping until an error happens or we get data
        while (true) {

            pdu *receive_pdu = NULL;
            //pthread_mutex_lock(&mutex);
            while (chat_server_com->status == STATUS_RECEIVE_EMPTY ||
                   chat_server_com->status == 0) {
                receive_pdu = parse_header(chat_server_com);
            }
            //pthread_mutex_unlock(&mutex);
            if (chat_server_com->status != STATUS_RECEIVE_OK) {
                printf("\nsomething wrong with receive in Client\n");
                arg->status = (int*)0;
                senddd = 0;
                pthread_join(*thread_handle, NULL);
                free(thread_handle);
                free(arg);
                chat_server_com->close(chat_server_com);
                free_tcp_client_communicator(chat_server_com);
                return JOIN_FAIL;
            }
            if (receive_pdu != NULL) {
                //arg->status = 0;
                receive_pdu->print(receive_pdu);
                receive_pdu->free_pdu(receive_pdu);
            }

            chat_server_com->status = 0;


        }

    }
    pthread_join(*thread_handle, NULL);
    free(thread_handle);

    chat_server_com->close(chat_server_com);
    free_tcp_client_communicator(chat_server_com);
    free(arg);
    printf("exiting chat server\n");
    return 0;
}

void * sendThread(void *data){

    threadarg* arg = data;
    //allocate resources for getline
    char *input;
    int bufsize = 255;
    __ssize_t characters;
    input = (char *)malloc(bufsize * sizeof(char));
    if( input == NULL) {
        perror("Unable to allocate buffer\n");
        EXIT_FAILURE;
    }

    while(true){
        memset(input, 0, (size_t) bufsize);
        //if(*!arg->reading){

            characters = getline(&input, (size_t *) &bufsize, stdin);
        if(senddd == 1){
            if(characters > bufsize){
                printf("input too long\n");
            } else if(strcmp(input, "exit\n") == 0){
                pdu* quit = create_quit();
                arg->com->send_pdu(arg->com, quit);
                quit->free_pdu(quit);
                break;
            }else{
                pdu* mess = create_mess(0, 0);
                mess_add_message(mess, (uint16_t) characters, 0, input);
                mess_set_checksum(mess);
                arg->com->send_pdu(arg->com, mess);
                //*arg->reading = 1;
                mess->free_pdu(mess);
            }
        }else{
            break;
        }

    }

    free(input);
    return NULL;
}
