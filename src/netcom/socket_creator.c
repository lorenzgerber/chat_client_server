/*
 * socket_creator.c
 *
 *  Created on: Oct 1, 2017
 *      Author: lgerber
 */

#include "socket_creator.h"


int move_to_process_buffer(struct io_handler *handler, int n_word);


/**
 * create_client_tcp_socket
 *
 * Constructor for a client tcp socket. The
 * function creates an io_handler struct, configures
 * the member variables and registers the relevant
 * functions. The function also performs the
 * address lookup with the provided arguments
 * server_name and port.
 */
io_handler* create_tcp_client_communicator(char *server_name, int port){
    io_handler *io = malloc(sizeof(io_handler));

    io->socket_entity = ENTITY_CLIENT;

    // Register functions
    io->send_pdu = tcp_client_send_pdu;
    io->connect = tcp_client_connect;
    io->sfd_read_write = setup_tcp_send_socket();

    // get server address
    io->hints = get_tcp_server_address(&port, server_name);

    return io;
}

/**
 * tcp_client_connect
 *
 * Function to be registered in the client tcp io handler.
 * On calling, the function will try n times to connect
 * with the address information available in the io handler
 * struct.
 */
int tcp_client_connect(struct io_handler *self, int n_times){

    int status = -1;
    int counter = n_times;

    while(status == -1 && counter > 0){
        printf("tcp client tries connecting to server...\n");
        status = connect_to_tcp_server(self->sfd_read_write, self->hints);
        sleep(1);
        counter--;
    }
    if(status == 0){
        printf("tcp client connected to server...\n");
    }

    return status;
}

/**
 * tcp_client_send_pdu
 *
 * Function to be registered in the tcp client io handler.
 * It takes a pdu struct as argument and tries to send
 * the serialized message on the io handler socket.
 */
int tcp_client_send_pdu(struct io_handler *self, pdu* pdu){

    message_byte_array *MBA = pdu->create_message(pdu);

    if(sendto(self->sfd_read_write, MBA->array, pdu->get_message_length(pdu), 0, (struct sockaddr *)self->hints->ai_addr, self->hints->ai_addrlen)<0){
        fprintf(stderr, "sendto failed\n");
    }

    return 0;
}

io_handler* create_tcp_server_listener(char *server_name, uint16_t port){
    io_handler *io = malloc(sizeof(io_handler));

    io->socket_entity = ENTITY_SERVER;

    // Register functions
    io->listen = tcp_server_listen;
    setup_listener_socket(&io->sfd_listen, &port);

    return io;
}

io_handler* tcp_server_listen(struct io_handler *self){

    io_handler *com;
    int status = 0;
    status = listen_obtain_client_socket(&self->sfd_listen, &self->sfd_read_write);
    if(status == 0){
        int *sfd_read_write = malloc(sizeof(int));
        *sfd_read_write = self->sfd_read_write;
        com = malloc(sizeof(io_handler));
        com = create_tcp_server_communicator(sfd_read_write);
    }

    return com;
}

io_handler* create_tcp_server_communicator(int *sfd_read_write){
    io_handler *io = malloc(sizeof(io_handler));
    io->socket_entity = ENTITY_SERVER;
    io->read_buffer = malloc(sizeof(uint8_t)*131072);
    io->recv_length = 0;
    io->sfd_read_write = *sfd_read_write;

    io->request_n_word=tcp_server_request_n_word;

    return io;
}

int tcp_server_send_pdu(struct io_handler *self, pdu *pdu){

    // to be implemented. Mostly copy paste
    // from tcp client code


    return 0;
}

int tcp_server_request_n_word(struct io_handler *self, int n_word){

    int nread = 0;
    if(self->buffer != NULL){
        free_message_byte_array(self->buffer);
    }

    if(self->recv_length >= n_word*4){
        move_to_process_buffer(self, n_word);
        return 0;
    }

    nread = recv(self->sfd_read_write, self->read_buffer, sizeof(uint8_t)*131072, 0);

    if(nread == 0){
        fprintf(stderr, "Receiver - Client disconnected\n");
        fflush(stderr);
    } else if (nread > 0){
        self->recv_length+=nread;
        /*
        for(int i = 0; i < nread; i++){
            printf("%d ", self->read_buffer[i]);
        }*/
        if(self->recv_length >= n_word*4){
            move_to_process_buffer(self, n_word);
        }
    }


    return 0;
}

io_handler* create_udp_client_communicator(char* server_name, int port){

    io_handler *io = malloc(sizeof(io_handler));

    io->socket_entity = ENTITY_SERVER;

    // Register functions
    io->send_pdu = udp_client_send_pdu;
    //io->connect = udp_client_connect;
    io->sfd_read_write = setup_udp_send_socket();

    // get server address
    io->hints = get_udp_server_address(&port, server_name);

    return io;
}

int move_to_process_buffer(struct io_handler *handler, int n_word){
    size_t length = n_word *4;
    //handler->process_buffer = malloc(sizeof(uint8_t)* length);
    handler->buffer = create_message_byte_array(n_word);
    memcpy(handler->buffer->array, handler->read_buffer, length);
    handler->read_head = handler->buffer->array;
    memcpy(handler->read_buffer, handler->read_buffer+length, length);
    handler->recv_length -= length;

    return 0;
}

int udp_client_send_pdu(struct io_handler *self, pdu* pdu){
    message_byte_array *MBA = pdu->create_message(pdu);

    if(sendto(self->sfd_read_write, MBA->array, pdu->get_message_length(pdu), 0, (struct sockaddr *)self->hints->ai_addr, self->hints->ai_addrlen)<0){
        fprintf(stderr, "sendto failed\n");
    }

    return 0;
}

int udp_client_connect(struct io_handler *self, int n_times){

    int status = -1;
    int counter = n_times;

    while(status == -1 && counter > 0){
        printf("udp client tries connecting to server...\n");
        status = connect_to_udp_server(self->sfd_read_write, self->hints);
        sleep(1);
        counter--;
    }
    if(status == 0){
        printf("udp client connected to server...\n");
    }

    return status;
}

io_handler* create_udp_server_listener(char *server_name, uint16_t port){
    io_handler *io = malloc(sizeof(io_handler));

    io->socket_entity = ENTITY_SERVER;

    // Register functions
    io->listen = udp_server_listen;
    setup_listener_socket_udp(&io->sfd_listen, &port);

    return io;
}

io_handler* udp_server_listen(struct io_handler *self){

    io_handler *com;
    int status = 0;
    status = udp_listen_obtain_client_socket(&self->sfd_listen, &self->sfd_read_write);
    if(status == 0){
        int *sfd_read_write = malloc(sizeof(int));
        *sfd_read_write = self->sfd_read_write;
        com = malloc(sizeof(io_handler));
        com = create_udp_server_communicator(sfd_read_write);
    }

    return com;
}


io_handler* create_udp_server_communicator(int *sfd_read_write){

    io_handler *io = malloc(sizeof(io_handler));
    io->socket_entity = ENTITY_SERVER;
    io->read_buffer = malloc(sizeof(uint8_t)*131072);
    io->recv_length = 0;
    io->sfd_read_write = *sfd_read_write;

    io->request_n_word=udp_server_request_n_word;

    return io;
}

int udp_server_send_pdu(struct io_handler *self, pdu *pdu){

    // to be implemented. Mostly copy paste
    // from tcp client code


    return 0;
}

int udp_server_request_n_word(struct io_handler *self, int n_word){

    int nread = 0;
    if(self->buffer != NULL){
        free_message_byte_array(self->buffer);
    }

    if(self->recv_length >= n_word*4){
        move_to_process_buffer(self, n_word);
        return 0;
    }

    nread = recv(self->sfd_read_write, self->read_buffer, sizeof(uint8_t)*131072, 0);

    if(nread == 0){
        fprintf(stderr, "Receiver - Client disconnected\n");
        fflush(stderr);
    } else if (nread > 0){
        self->recv_length+=nread;
        /*
        for(int i = 0; i < nread; i++){
            printf("%d ", self->read_buffer[i]);
        }*/
        if(self->recv_length >= n_word*4){
            move_to_process_buffer(self, n_word);
        }
    }


    return 0;
}

/**
 * Function read from dummy socket
 *
 * This function is registered in create_dummy_socket
 *
 */
int dummy_socket_request_n_word(struct io_handler *self, int n_word){

	//uint8_t* next_read = &self->buffer->array[self->read_head];
	if(self->read_head == NULL){
		self->read_head = &self->buffer->array[0];
		self->read_next = self->read_head + n_word *4;
	} else {
		self->read_head = self->read_next;
		self->read_next += n_word * 4;
	}


	return 0;//next_read;
}

/**
 * create_dummy_socket
 *
 * This method is used for testing. It creates a socket
 * mock to test the parser functionality.
 */
io_handler* create_dummy_socket(int op_code, int socket_entity){
	io_handler *dummy_socket = malloc(sizeof(io_handler));
	dummy_socket->read_head = NULL;
	dummy_socket->read_next = NULL;
	dummy_socket->request_n_word = dummy_socket_request_n_word;
	dummy_socket->socket_entity = socket_entity;

	switch(op_code){
		case PDU_ACK:
			return dummy_socket_ack(dummy_socket);
		case PDU_NOTREG:
			return dummy_socket_notreg(dummy_socket);
		case PDU_SLIST:
			return dummy_socket_slist(dummy_socket);
		case PDU_JOIN:
			return dummy_socket_join(dummy_socket);
		case PDU_PARTICIPANTS:
			return dummy_socket_participants(dummy_socket);
        case PDU_QUIT:
            return dummy_socket_quit(dummy_socket);
		case PDU_MESS:
			return dummy_socket_mess(dummy_socket);
		case PDU_PJOIN:
			return  dummy_socket_pjoin(dummy_socket);
		case PDU_PLEAVE:
			return dummy_socket_pleave(dummy_socket);
		default:
			break;
	}

	return dummy_socket;
}
