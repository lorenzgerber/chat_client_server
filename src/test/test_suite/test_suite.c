/*
 * test_suite.c
 *
 *  Created on: Sep 21, 2017
 *      Author: lgerber
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdu_templates.h"
#include "pdu_creator.h"
#include "message_byte_array.h"
#include "pdu_serializer.h"


int main(int argc, char*argv[]){

	uint32_t length = 0;
	message_byte_array *MBA;

    /*
	 * Test REG
	 */
	printf("\n\nREG\n");

    // -------REG using new struct-----------------
	pdu *reg = create_reg(10,2000);
	reg->add_server_name(reg,"servername");

	MBA = reg->create_message(reg);
    length = get_length_reg(reg);
    printf("Length of message = %d\n", length);
    for(int i = 0;i < length;i++){
		printf("%d, ", MBA->array[i]);
	}
	reg->free_pdu(reg);
	free_message_byte_array(MBA);

    /*
	 * Test ALIVE
	 */
	printf("\n\nALIVE\n");


    //------ALIVE using new struct------
    pdu *alive = create_alive(100, 10000);

    MBA = alive->create_message(alive);
    length = get_length_alive(alive);
    printf("Length of message = %d\n", length);
    for(int i = 0;i < length;i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    alive->free_pdu(alive);
    free_message_byte_array(MBA);

	/*
	 * Test ACK
	 */
	printf("\n\nACK\n");

    //-------ACK using new struct------
    pdu *ack = create_ack(10000);

    MBA = ack->create_message(ack);
    length = get_length_ack(ack);
    printf("Length of message = %d\n", length);
    for(int i = 0;i < length;i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    ack->free_pdu(ack);
    free_message_byte_array(MBA);

    /*
	 * Test NOTREG
	 */
	printf("\n\nNOTREG\n");

    //------NOTREG using new struct--------
    pdu *notreg = create_notreg(10000);

    MBA = notreg->create_message(notreg);
    length = get_length_notreg(notreg);
    printf("Length of message = %d\n", length);
    for(int i = 0;i < length;i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    notreg->free_pdu(notreg);
    free_message_byte_array(MBA);

	/*
	 * GETLIST
	 */
	printf("\n\nGETLIST\n");

    //-------GETLIST using new struct-----
    pdu *getlist = create_getlist();

    MBA = getlist->create_message(getlist);
    length = get_length_getlist(getlist);
    printf("Length of message = %d\n", length);
    for(int i = 0;i < length;i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    getlist->free_pdu(getlist);
    free_message_byte_array(MBA);

    /*
	 * SLIST
	 */
	printf("\n\nSLIST\n");


    //-------SLIST using new struct--------
    pdu *slist = create_slist(2);

    uint8_t address3[4] = {127,0,0,1};
    pdu_server_entry* server3 = create_server_entry(address3,2000,4,10);
    server3->add_server_name(server3, "servername");
    slist->add_server_entry(slist, server3);

    uint8_t address4[4] = {127,0,0,2};
    pdu_server_entry* server4 = create_server_entry(address4,2001,4,11);
    server4->add_server_name(server4, "servername2");
    slist->add_server_entry(slist, server4);

    MBA = slist->create_message(slist);
    length = get_length_slist(slist);
    printf("Length of message = %d\n", length);

    for(int i = 0; i < length; i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    slist->free_pdu(slist);
    free_message_byte_array(MBA);

	/*
	 * JOIN
	 */
    printf("\n\nJOIN\n");

	//Create JOIN and add identity
	pdu_JOIN *JOIN = create_JOIN(8);
	JOIN->add_identity(JOIN, "identity");

	// Create join byte stream message to send
	MBA = JOIN->create_message(JOIN);
	length = get_length_JOIN(JOIN);
    printf("Length of message = %d\n", length);
	for(int i = 0; i < length; i++){
		printf("%d, ", MBA->array[i]);
	}
    printf("\n");

	// free JOIN and MBA
	free_pdu_join(JOIN);
	free_message_byte_array(MBA);

    //------JOIN using new struct---------
    pdu *join = create_join(8);
    join->add_identity(join, "identity");

    MBA = join->create_message(join);
    length = get_length_join(join);
    printf("Length of message = %d\n", length);
    for(int i = 0; i < length; i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    join->free_pdu(join);
    free_message_byte_array(MBA);

	/*
	 * PARTICIPANTS
	 */
    printf("\n\nPARTICIPANTS\n");

	//Create PARTICIPANTS and add some identities
	pdu_PARTICIPANTS *PARTICIPANTS = create_PARTICIPANTS(3, 15);
	PARTICIPANTS->add_identities(PARTICIPANTS, "partic\0ipa\0nts\0");

    for(int i = 0; i < PARTICIPANTS->number_identities;i++){
		printf("%s\n", PARTICIPANTS->identities[i]);
	}

    // Create participants byte stream message to send
	MBA = PARTICIPANTS->create_message(PARTICIPANTS);
	length = get_length_PARTICIPANTS(PARTICIPANTS);
	printf("Length of message = %d\n", length);

	for(int i = 0; i < length; i++){
		printf("%d, ", MBA->array[i]);
	}
    printf("\n");

	// free PARTICIPANTS and MBA
	free_pdu_participants(PARTICIPANTS);
	free_message_byte_array(MBA);

    //-------PARTICIPANTS using new struct-------
    pdu *participants = create_participants(3, 15);
    participants->add_identities(participants, "partic\0ipa\0nts\0");

    for(int i = 0; i < participants->number_identities;i++){
        printf("%s\n", participants->identities[i]);
    }

    MBA = participants->create_message(participants);
    length = get_length_participants(participants);
    printf("Length of message = %d\n", length);
    for(int i = 0; i < length; i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    participants->free_pdu(participants);
    free_message_byte_array(MBA);

	/*
	 * QUIT
	 */
	printf("\n\nQUIT\n");

	// Create QUIT struct
	pdu_QUIT *QUIT = create_QUIT();

	// Create GETLIST byte stream message to send
	MBA = QUIT->create_message(QUIT);
	length = get_length_QUIT(QUIT);
    printf("Length of message = %d\n", length);
	for(int i = 0;i < length;i++){
		printf("%d, ", MBA->array[i]);
	}
    printf("\n");

	// free QUIT and MBA
	free_pdu_quit(QUIT);
	free_message_byte_array(MBA);

    //------QUIT using new struct----------
    pdu *quit = create_quit();

    MBA = quit->create_message(quit);
    length = get_length_quit(quit);
    printf("Length of message = %d\n", length);
    for(int i = 0;i < length;i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    quit->free_pdu(quit);
    free_message_byte_array(MBA);

	/*
	 * MESS
	 */
    printf("\n\nMESS\n");

	pdu_MESS *MESS = create_MESS(8, 99);
	MESS->add_message(MESS, 13, 1505933137, "Test Message.");
	MESS->add_client_identity(MESS, "identity");

	printf("%s\n", MESS->message);

	MBA = MESS->create_message(MESS);
	length = get_length_MESS(MESS);
	printf("Length of message = %d\n", length);

	for(int i = 0;i < length;i++){
		printf("%d, ", MBA->array[i]);
	}
    printf("\n");

	free_pdu_mess(MESS);
	free_message_byte_array(MBA);

    //--------MESS using new struct--------
    pdu *mess = create_mess(8, 99);
    mess->add_message(mess, 13, 1505933137, "Test Message.");
    mess->add_identity(mess, "identity"); // add identity vs add_client_identity
    //mess->add_client_identity(mess, "identity");

    printf("%s\n", mess->message);

    MBA = mess->create_message(mess);
    length = get_length_mess(mess);
    printf("Length of message = %d\n", length);

    for(int i = 0;i < length;i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    mess->free_pdu(mess);
    free_message_byte_array(MBA);

	/*
	 * PJOIN
	 */
    printf("\n\nPJOIN\n");

	//Create PJOIN and add some identities

	pdu_PJOIN *PJOIN = create_PJOIN(8);
	PJOIN->add_client_identity(PJOIN, 1505933137, "identity");

	printf("%s\n", PJOIN->client_identity);

	// Create PJOIN byte stream for send
	MBA = PJOIN->create_message(PJOIN);
	length = get_length_PJOIN(PJOIN);
	printf("Length of message = %d\n", length);

	for(int i = 0;i < length;i++){
		printf("%d, ", MBA->array[i]);
	}
    printf("\n");

	free_pdu_pjoin(PJOIN);
	free_message_byte_array(MBA);

    //---------PJOIN using new struct--------
    pdu *pjoin = create_pjoin(8);
    pjoin->add_client_identity(pjoin, 1505933137, "identity");

    printf("%s\n", pjoin->identity);

    MBA = pjoin->create_message(pjoin);
    length = get_length_pjoin(pjoin);
    printf("Length of message = %d\n", length);

    for(int i = 0;i < length;i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    pjoin->free_pdu(pjoin);
    free_message_byte_array(MBA);

	/*
	 * PLEAVE
	 */
    printf("\n\nPLEAVE\n");

	//Create PLEAVE and add some identities
    pdu_PLEAVE *PLEAVE = create_PLEAVE(8);
	PLEAVE->add_client_identity(PLEAVE, 1505933137, "identity");

	printf("%s\n", PLEAVE->client_identity);

	// Create PLEAVE byte stream for send
	MBA = PLEAVE->create_message(PLEAVE);
	length = get_length_PLEAVE(PLEAVE);
	printf("Length of message = %d\n", length);

	for(int i = 0;i < length;i++){
		printf("%d, ", MBA->array[i]);
	}
    printf("\n");

	free_pdu_pleave(PLEAVE);
	free_message_byte_array(MBA);

    //---------PLEAVE using new struct-----------
    pdu *pleave = create_pleave(8);
    pleave->add_client_identity(pleave, 1505933137, "identity");

    printf("%s\n", pleave->identity);

    MBA = pleave->create_message(pleave);
    length = get_length_pleave(pleave);
    printf("Length of message = %d\n", length);
    for(int i = 0;i < length;i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    pleave->free_pdu(pleave);
    free_message_byte_array(MBA);

	return 0;
}

