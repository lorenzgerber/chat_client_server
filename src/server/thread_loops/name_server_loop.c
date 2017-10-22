/*
 * name_server_loop.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */
#include "name_server_loop.h"

/**
 * name_server_loop
 *
 * Function run as separate thread that
 * connects to the nameserver, registers
 * and keeps on sending a heartbeat. If
 * for some reason the name server
 * registration is lost, a new registration
 * is send.
 * @param server struct
 */
void* name_server_loop(void *data){
	server* server = data;
	int registered = 0;

	char* address = server->nameserver_host;
	io_handler *udp_com;

	// create udp communicator
	udp_com = create_udp_communicator(address, server->nameserver_port);
	udp_com->connect(udp_com, 5);
	udp_com->status = 0;

	while(*server->bail_out != 1){

		if(registered == 0 && *server->bail_out != 1){
			//send REG to name server
			pdu* reg = create_reg(strlen(server->servername), server->our_port);
			reg->add_server_name(reg,server->servername);
			udp_com->send_pdu(udp_com, reg);
			reg->free_pdu(reg);

			//receive answer (ACK expected)
			pdu* answer_pdu = NULL;
			while(udp_com->status == STATUS_RECEIVE_EMPTY || udp_com->status == 0){
				answer_pdu = parse_header(udp_com);
			}
			if(udp_com->status != STATUS_RECEIVE_OK){
				printf("something wrong with receive\n");
			}
			udp_com->status = 0;

			if(answer_pdu != NULL && answer_pdu->type == 1) {
				printf("\nACK %d received as answer to REG:\n",
					   answer_pdu->id_number);
				server->our_id = answer_pdu->id_number;
				registered = 1;
				answer_pdu->print(answer_pdu);
				answer_pdu->free_pdu(answer_pdu);
			}else if(answer_pdu == NULL){
				printf("Did not receive anything");

			}else{
				fprintf(stderr, "\nReceived unexpected response from nameserver\n");
				fflush(stderr);
				return NULL;
			}
		}

		while(registered && *server->bail_out != 1){
			pdu *alive = create_alive(server->number_of_clients,
					(uint16_t) server->our_id);
			udp_com->send_pdu(udp_com, alive);
			alive->free_pdu(alive);

			//Receive answer
			pdu* answer_pdu_3 = NULL;
			while(udp_com->status == STATUS_RECEIVE_EMPTY || udp_com->status == 0){
				answer_pdu_3 = parse_header(udp_com);
			}
			if(udp_com->status != STATUS_RECEIVE_OK){
				printf("something wrong with receive\n");
			}

			udp_com->status = 0;
			if(answer_pdu_3 != NULL && answer_pdu_3->type == 1){
				printf("\nACK %d received as answer to ALIVE %d:\n",
					   answer_pdu_3->id_number,
					   server->our_id);
				answer_pdu_3->print(answer_pdu_3);
				answer_pdu_3->free_pdu(answer_pdu_3);
			}else if(answer_pdu_3!= NULL && answer_pdu_3->type == 100){
				printf("\nNOTREG received as answer to ALIVE %d:\n", server->our_id);
				registered = 0;
				answer_pdu_3->print(answer_pdu_3);
				answer_pdu_3->free_pdu(answer_pdu_3);
			}else{
				fprintf(stderr, "\nReceived unexpected response from nameserver\n");
				fflush(stderr);
				return NULL;
			}

			if(*server->bail_out != 1){
				sleep(15);
			}


		}
	}

	free_message_byte_array(udp_com->buffer);
	free(udp_com->read_buffer);
	udp_com->close(udp_com);
	free(udp_com);

	return NULL;
}



