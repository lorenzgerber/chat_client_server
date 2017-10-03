/*
 * pdu_parser.c
 *
 *  Created on: Sep 30, 2017
 *      Author: lgerber
 */

#include "pdu_parser.h"

pdu* parse_header(struct io_handler *socket){

	uint8_t op_code;
	uint8_t* read_position;
	read_position = socket->request_n_word(socket, 1);
	op_code = *read_position;

    pdu* return_pdu = NULL;
	printf("---------------\n");
	printf("op_code %d\n", op_code);
	switch(op_code){
		case PDU_ACK:
			return_pdu = parse_ACK(socket, read_position);
			break;
		case PDU_NOTREG:
			return_pdu = parse_NOTREG(socket, read_position);
			break;
		case PDU_SLIST:
			return_pdu = parse_SLIST(socket, read_position);
			break;
		case PDU_JOIN:
			return_pdu = parse_JOIN(socket, read_position);
			break;
		case PDU_PARTICIPANTS:
			return_pdu = parse_PARTICIPANTS(socket, read_position);
			break;
        case PDU_QUIT:
            return_pdu = parse_QUIT(socket, read_position);
            break;
        case PDU_MESS:
            return_pdu = parse_MESS(socket, read_position);
            break;
		case PDU_PJOIN:
			return_pdu = parse_PJOIN(socket, read_position);
			break;
		case PDU_PLEAVE:
			return_pdu = parse_PLEAVE(socket, read_position);
			break;
	}

    return return_pdu;
}
pdu* parse_ACK(struct io_handler* socket, uint8_t* read_position){
	uint16_t id_nr = ((uint16_t)*(read_position+2) << 8) | (uint16_t)*(read_position+3);
	id_nr = ntohs(id_nr);
    pdu *ack = create_ack(id_nr);
	printf("identity number %d\n", id_nr);

	return ack;
}

pdu* parse_NOTREG(struct io_handler* socket, uint8_t* read_position){
	uint16_t id_nr = ((uint16_t)*(read_position+2) << 8) | (uint16_t)*(read_position+3);
	id_nr = ntohs(id_nr);
    pdu* notreg = create_notreg(id_nr);
	printf("identity number %d\n", id_nr);

	return notreg;
}

pdu* parse_SLIST(struct io_handler* socket, uint8_t* read_position){

	uint16_t nr_of_servers = ((uint16_t)*(read_position+2) << 8) | (uint16_t)*(read_position+3);
	nr_of_servers = ntohs(nr_of_servers);
    pdu *slist = create_slist(nr_of_servers);

    printf("Number of servers %d\n\n", nr_of_servers);
	for(int i = 0; i < nr_of_servers; i++){
		read_position = socket->request_n_word(socket, 2);
		printf("Adress server %d: %d.%d.%d.%d\n",i+1,*read_position,*(read_position+1),*(read_position+2),*(read_position+3));
        uint8_t address[4] = {*read_position,*(read_position+1),*(read_position+2),*(read_position+3)};

		uint16_t port = ((uint16_t) *(read_position+4) << 8) | *(read_position+5);
		port = ntohs(port);
		printf("server %d port: %d\n",i+1, port);

		uint8_t number_of_clients = *(read_position+6);
		printf("server %d number of clients: %d\n",i+1, number_of_clients);

		uint8_t namelen = *(read_position+7);
		printf("server %d name length: %d\n", i+1, namelen);
		read_position = socket->request_n_word(socket, (namelen + 4 - 1)/4);
		printf("server %d name: ", i+1);
        char servername[255];
        memset(servername, 0, 255);
        int j;
		for(j = 0; j < namelen; j++){
			printf("%c", (int)*(read_position+j));
            servername[j] = (int)*(read_position+j);
		}
        servername[j+1] = '\0';
		printf("\n\n");
        pdu_server_entry* server = create_server_entry(address,port,number_of_clients,namelen);
        server->add_server_name(server, servername);
        slist->add_server_entry(slist, server);
	}
	return slist;
}

pdu* parse_JOIN(struct io_handler* socket, uint8_t* read_position){

	uint8_t idlength = *(read_position+1);
	printf("length of id: %d\n", idlength);
    pdu *join = create_join(idlength);

	read_position = socket->request_n_word(socket, (idlength + 4 - 1)/4);
	printf("server id: ");
    char identity[255];
    memset(identity, 0, 255);
    int i;
	for(i = 0; i < idlength; i++){
		printf("%c", (int)*(read_position+i));
        identity[i] = (int)*(read_position+i);
	}
    identity[i+1] = '\0';
    join->add_identity(join, identity);
	printf("\n");
	return join;
}

pdu* parse_PARTICIPANTS(struct io_handler* socket, uint8_t* read_position){

	uint8_t nr_of_clients = *(read_position+1);
	printf("nr of clients: %d\n", nr_of_clients);

	uint16_t length_of_clients = ((uint16_t)*(read_position+2) << 8) | (uint16_t)*(read_position+3);
	length_of_clients = ntohs(length_of_clients);
	printf("length of clients: %d\n\n", length_of_clients);

	read_position = socket->request_n_word(socket, (length_of_clients + 4 - 1)/4);
	printf("Clients connected to server:\n");
	pdu *participants = create_participants(nr_of_clients, length_of_clients);
	char clients[255*nr_of_clients];
	memset(clients, 0, 255*nr_of_clients);
	for(int i = 0; i < length_of_clients; i++){
		if((int)*(read_position+i) != '\0'){
			printf("%c", (int)*(read_position+i));
			clients[i] = (int)*(read_position+i);
		}else{
			printf("\n");
			clients[i] = (int)*(read_position+i);
		}
	}
	participants->add_identities(participants, clients);
	return participants;
}

pdu* parse_QUIT(struct io_handler* socket, uint8_t* read_position){
	pdu *quit = create_quit();
    return quit;
}

pdu* parse_MESS(struct io_handler* socket, uint8_t* read_position){




	// build step by step a MESS pdu
	uint8_t identity_length = *(read_position+2);
	uint8_t checksum = *(read_position+3);
    char identity[identity_length+1];

	printf("identity length %d\n", identity_length);
	printf("checksum %d\n", checksum);

	pdu *MESS = create_mess(identity_length, checksum);

	read_position = socket->request_n_word(socket, 1);

	uint16_t message_length = ((uint16_t) *read_position << 8) | *(read_position+1);
	message_length = ntohs(message_length);

	printf("message_length: %d\n", message_length);

	read_position = socket->request_n_word(socket, 1);
	uint32_t time_stamp = ((uint32_t)*read_position << 24) | *(read_position+1) << 16 | *(read_position+2) << 8 | *(read_position+3);
	time_stamp = ntohl(time_stamp);

	printf("timestamp: %u\n", time_stamp);


	uint32_t length_padded = (uint32_t) (message_length / 4 + ((message_length % 4 > 0) ? 1 : 0));

	read_position = socket->request_n_word(socket, length_padded);
	char message[message_length + 1];
    memset(message, 0, message_length+1);// = malloc(sizeof(char)*(message_length+1));
	memcpy(message, read_position, sizeof(char)*message_length);


	MESS->add_message(MESS, message_length, time_stamp, message);

	for(int i = 0; i < message_length; i++){
		printf("%c", message[i]);
	}
	printf("\n");

	if(socket->socket_entity==ENTITY_SERVER){
		if(identity_length > 0){
			printf("something wrong, client sent entity length\n");
		}
	} else {
		length_padded = (uint32_t) (identity_length / 4 + ((identity_length % 4 > 0) ? 1 : 0));
		read_position = socket->request_n_word(socket, length_padded);
        memset(identity, 0, sizeof(char)*(identity_length+1));
		//identity = malloc(sizeof(char)*(identity_length+1));
		memcpy(identity, read_position, sizeof(char)*identity_length);
		MESS->add_identity(MESS, identity);//changed from add_client_identity
	}

	for(int i = 0; i < identity_length; i++){
		printf("%c", identity[i]);
	}
	printf("\n");


	return MESS;

}

pdu* parse_PJOIN(struct io_handler* socket, uint8_t* read_position){

	uint8_t identity_length = *(read_position+1);
	printf("identity length: %d\n", identity_length);

	read_position = socket->request_n_word(socket, 1);
	uint32_t time_stamp = ((uint32_t)*read_position << 24) | *(read_position+1) << 16 | *(read_position+2) << 8 | *(read_position+3);
	time_stamp = ntohl(time_stamp);
	printf("timestamp: %u\n", time_stamp);

	read_position = socket->request_n_word(socket, (identity_length + 4 - 1)/4);
	char identity[255];
	memset(identity, 0, 255);
	int i;
	for(i = 0; i < identity_length; i++){
		printf("%c", *(read_position+i));
		identity[i] = *(read_position+i);
	}
	identity[i+1] = '\0';
	printf("\n");

	pdu *pjoin = create_pjoin(identity_length);
	pjoin->add_client_identity_timestamp(pjoin, time_stamp, identity);

	return pjoin;
}

pdu* parse_PLEAVE(struct io_handler* socket, uint8_t* read_position){

	uint8_t identity_length = *(read_position+1);
	printf("identity length: %d\n", identity_length);

	read_position = socket->request_n_word(socket, 1);
	uint32_t time_stamp = (*read_position << 24) | *(read_position+1) << 16 | *(read_position+2) << 8 | *(read_position+3);
	time_stamp = ntohl(time_stamp);
	printf("timestamp: %u\n", time_stamp);

	read_position = socket->request_n_word(socket, (identity_length + 4 - 1)/4);

	char identity[255];
	memset(identity, 0, 255);
	int i;
	for(i = 0; i < identity_length; i++){
		printf("%c", *(read_position+i));
		identity[i] = *(read_position+i);
	}
	identity[i+1] = '\0';
	printf("\n");

	pdu *pleave = create_pleave(identity_length);
	pleave->add_client_identity_timestamp(pleave, time_stamp, identity);

	return pleave;
}
