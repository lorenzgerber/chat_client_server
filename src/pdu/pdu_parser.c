/*
 * pdu_parser.c
 *
 *  Created on: Sep 30, 2017
 *      Author: lgerber
 */

#include "pdu_parser.h"

/**
 * parse_header parses the the stream into a pdu object. Initially it reads
 * only the first word to determine what parsing function to call. There is one
 * such function for each pdu.
 *
 * @param socket io_handler, The struct containing the read stream.
 * @return pdu , the parsed pdu.
 */
pdu* parse_header(struct io_handler *socket){

	uint8_t op_code;
	uint8_t* read_position;

	read_position = socket->request_n_word(socket, 1);
	op_code = *read_position;

    pdu* return_pdu = NULL;

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

pdu* parse_ACK(struct io_handler* socket, const uint8_t* read_position){
    //assemble the 16 bit id number from two 8 bits read
	uint16_t id_nr = (*(read_position+2) << 8) | *(read_position+3);
	id_nr = ntohs(id_nr);
    pdu *ack = create_ack(id_nr);

    return ack;
}

pdu* parse_NOTREG(struct io_handler* socket, const uint8_t* read_position){
    //assemble the 16 bit id number from two 8 bits read
	uint16_t id_nr = (*(read_position+2) << 8) | *(read_position+3);
	id_nr = ntohs(id_nr);
    pdu* notreg = create_notreg(id_nr);

	return notreg;
}

pdu* parse_SLIST(struct io_handler* socket, const uint8_t* read_position){
    //assemble the 16 bit number of servers from two 8 bits read
	uint16_t nr_of_servers = (*(read_position+2) << 8) | *(read_position+3);
	nr_of_servers = ntohs(nr_of_servers);
    pdu *slist = create_slist(nr_of_servers);

	for(int i = 0; i < nr_of_servers; i++){
        //read 2 additional words and set the pdu's adress,port,nr of clients
		//and servername length
		read_position = socket->request_n_word(socket, 2);
        uint8_t address[4] = {*read_position,
							  *(read_position+1),
							  *(read_position+2),
							  *(read_position+3)};

		uint16_t port = (*(read_position+4) << 8) | *(read_position+5);
		port = ntohs(port);

		uint8_t number_of_clients = *(read_position+6);

		uint8_t namelen = *(read_position+7);

        //read sufficient number of words to assign server name
		read_position = socket->request_n_word(socket, (namelen + 4 - 1)/4);

        char servername[255];
        memset(servername, 0, 255);
        int j;
		for(j = 0; j < namelen; j++){
            servername[j] = (char)*(read_position + j);
		}
        servername[j+1] = '\0';

        pdu_server_entry* server = create_server_entry(address,
													   port,
													   number_of_clients,
													   namelen);
        server->add_server_name(server, servername);
        slist->add_server_entry(slist, server);
	}
	return slist;
}

pdu* parse_JOIN(struct io_handler* socket, const uint8_t* read_position){
    //read the id length
	uint8_t idlength = *(read_position+1);
    pdu *join = create_join(idlength);

    //read sufficient number of words to assign identity
	read_position = socket->request_n_word(socket, (idlength + 4 - 1)/4);

    char identity[255];
    memset(identity, 0, 255);
    int i;
	for(i = 0; i < idlength; i++){
        identity[i] = (char)*(read_position+i);
	}
    identity[i+1] = '\0';

    join->add_identity(join, identity);

	return join;
}

pdu* parse_PARTICIPANTS(struct io_handler* socket,
						const uint8_t* read_position){
    //read number of clients
	uint8_t nr_of_clients = *(read_position+1);
    //assemble 16 bit number of clients from two 8 bits
	uint16_t length_of_clients = *(read_position+2) << 8 | *(read_position+3);
	length_of_clients = ntohs(length_of_clients);
    //read sufficient number of words to assign participants
	read_position = socket->request_n_word(socket,
										   (length_of_clients + 4 - 1)/4);

	pdu *participants = create_participants(nr_of_clients, length_of_clients);

	char clients[255*nr_of_clients];
	memset(clients, 0, (size_t) (255 * nr_of_clients));
    int i;
	for(i = 0; i < length_of_clients; i++){
        clients[i] = (char)*(read_position+i);
	}
    clients[i+1] = '\0';

	participants->add_identities(participants, clients);

	return participants;
}

pdu* parse_QUIT(struct io_handler* socket, uint8_t* read_position){
	pdu *quit = create_quit();
    return quit;
}

pdu* parse_MESS(struct io_handler* socket, uint8_t* read_position){

    //read id length and checksum
	uint8_t identity_length = *(read_position+2);
	uint8_t checksum = *(read_position+3);

	pdu *mess = create_mess(identity_length, checksum);

    //read 1 additional word
	read_position = socket->request_n_word(socket, 1);

    //assemble 16 bit message length
	uint16_t message_length = (*read_position << 8) | *(read_position+1);
	message_length = ntohs(message_length);

    //read 1 additional word and assemble 32 bit time stamp
	read_position = socket->request_n_word(socket, 1);
	uint32_t time_stamp = (*read_position << 24) |
						   *(read_position+1) << 16 |
						   *(read_position+2) << 8 |
						   *(read_position+3);
	time_stamp = ntohl(time_stamp);

    //get padded message length
    uint32_t length_padded = (uint32_t) (message_length / 4 +
										((message_length % 4 > 0) ? 1 : 0));

    //read sufficient number of words to get message
	read_position = socket->request_n_word(socket, length_padded);
	char message[message_length + 1];
    memset(message, 0, message_length+1);
	memcpy(message, read_position, sizeof(char)*message_length);

    mess->add_message(mess, message_length, time_stamp, message);

    //add client identity if client
    char identity[identity_length+1];
	if(socket->socket_entity==ENTITY_SERVER){
        if(identity_length > 0){
			printf("something wrong, client sent entity length\n");
		}
	} else {
		length_padded = (uint32_t) (identity_length / 4 +
								   ((identity_length % 4 > 0) ? 1 : 0));
		read_position = socket->request_n_word(socket, length_padded);
        memset(identity, 0, sizeof(char)*(identity_length+1));
		memcpy(identity, read_position, sizeof(char)*identity_length);
        mess->add_identity(mess, identity);
	}

	return mess;

}

pdu* parse_PJOIN(struct io_handler* socket, const uint8_t* read_position){
    //read identity length
	uint8_t identity_length = *(read_position+1);

    //read additional word and assemble 32 bit time stamp
	read_position = socket->request_n_word(socket, 1);
	uint32_t time_stamp = (*read_position << 24) |
						   *(read_position+1) << 16 |
						   *(read_position+2) << 8 |
						   *(read_position+3);
	time_stamp = ntohl(time_stamp);

    //read sufficient number of words to get identity
	read_position = socket->request_n_word(socket,
										   (identity_length + 4 - 1)/4);

	char identity[255];
	memset(identity, 0, 255);
	int i;
	for(i = 0; i < identity_length; i++){
		identity[i] = *(read_position+i);
	}
	identity[i+1] = '\0';

	pdu *pjoin = create_pjoin(identity_length);
	pjoin->add_client_identity_timestamp(pjoin, time_stamp, identity);

	return pjoin;
}

pdu* parse_PLEAVE(struct io_handler* socket, const uint8_t* read_position){
    //read identity length
	uint8_t identity_length = *(read_position+1);

    //read additional word and assemble 32 bit time stamp
	read_position = socket->request_n_word(socket, 1);
	uint32_t time_stamp = (*read_position << 24) |
						   *(read_position+1) << 16 |
						   *(read_position+2) << 8 |
						   *(read_position+3);
	time_stamp = ntohl(time_stamp);

    //read sufficient number of words to get identity
	read_position = socket->request_n_word(socket,
										   (identity_length + 4 - 1)/4);

	char identity[255];
	memset(identity, 0, 255);
	int i;
	for(i = 0; i < identity_length; i++){
		identity[i] = *(read_position+i);
	}
	identity[i+1] = '\0';

	pdu *pleave = create_pleave(identity_length);
	pleave->add_client_identity_timestamp(pleave, time_stamp, identity);

	return pleave;
}
