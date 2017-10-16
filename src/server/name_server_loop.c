/*
 * name_server_loop.c
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */
#include "name_server_loop.h"

void* name_server_loop(void *data){
	server* server = data;
	int registered = 0;

	char* address = NAME_SERVER;
	io_handler *udp_com;

	// create udp communicator
	udp_com = create_udp_communicator(address, NAME_SERVER_PORT);
	udp_com->connect(udp_com, 5);
	udp_com->status = 0;

	while(*server->bail_out != 1){

		if(registered == 0){
			//send REG to name server
			pdu* reg = create_reg(13, 2000);
			reg->add_server_name(reg,"ljugarbaenken");
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

		while(registered){
			pdu *alive = create_alive(100, (uint16_t) server->our_id);
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

			sleep(15);

		}
	}

	free_message_byte_array(udp_com->buffer);
	free(udp_com->read_buffer);
	udp_com->close(udp_com);
	free(udp_com);

	return NULL;
}



