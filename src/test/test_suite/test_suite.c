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

	message_byte_array *MBA;

    /*
	 * Test REG
	 */
	printf("\n\nREG\n");

    // -------REG using new struct-----------------
	pdu *reg = create_reg(10,2000);
	reg->add_server_name(reg,"servername");

	MBA = reg->create_message(reg);
    printf("Length of message = %d\n", reg->get_message_length(reg));
    for(int i = 0;i < reg->get_message_length(reg);i++){
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
    printf("Length of message = %d\n", alive->get_message_length(alive));
    for(int i = 0;i < alive->get_message_length(alive);i++){
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
    printf("Length of message = %d\n", ack->get_message_length(ack));
    for(int i = 0;i < ack->get_message_length(ack);i++){
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
    printf("Length of message = %d\n", notreg->get_message_length(notreg));
    for(int i = 0;i < notreg->get_message_length(notreg);i++){
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
    printf("Length of message = %d\n", getlist->get_message_length(getlist));
    for(int i = 0;i < getlist->get_message_length(getlist);i++){
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
    printf("Length of message = %d\n", slist->get_message_length(slist));

    for(int i = 0; i < slist->get_message_length(slist); i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    slist->free_pdu(slist);
    free_message_byte_array(MBA);

	/*
	 * JOIN
	 */
    printf("\n\nJOIN\n");


    //------JOIN using new struct---------
    pdu *join = create_join(8);
    join->add_identity(join, "identity");

    MBA = join->create_message(join);
    printf("Length of message = %d\n", join->get_message_length(join));
    for(int i = 0; i < join->get_message_length(join); i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    join->free_pdu(join);
    free_message_byte_array(MBA);

	/*
	 * PARTICIPANTS
	 */
    printf("\n\nPARTICIPANTS\n");


    //-------PARTICIPANTS using new struct-------
    pdu *participants = create_participants(3, 15);
    participants->add_identities(participants, "partic\0ipa\0nts\0");

    for(int i = 0; i < participants->number_identities;i++){
        printf("%s\n", participants->identities[i]);
    }

    MBA = participants->create_message(participants);
    printf("Length of message = %d\n", participants->get_message_length(participants));
    for(int i = 0; i < participants->get_message_length(participants); i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    participants->free_pdu(participants);
    free_message_byte_array(MBA);

	/*
	 * QUIT
	 */
	printf("\n\nQUIT\n");



    //------QUIT using new struct----------
    pdu *quit = create_quit();

    MBA = quit->create_message(quit);
    printf("Length of message = %d\n", quit->get_message_length(quit));
    for(int i = 0;i < quit->get_message_length(quit);i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    quit->free_pdu(quit);
    free_message_byte_array(MBA);

	/*
	 * MESS
	 */
    printf("\n\nMESS\n");


    //--------MESS using new struct--------
    pdu *mess = create_mess(8, 99);
    mess->add_message(mess, 13, 1505933137, "Test Message.");
    mess->add_identity(mess, "identity"); // add identity vs add_client_identity
    //mess->add_client_identity(mess, "identity");

    printf("%s\n", mess->message);

    MBA = mess->create_message(mess);
    printf("Length of message = %d\n", mess->get_message_length(mess));

    for(int i = 0;i < mess->get_message_length(mess);i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    mess->free_pdu(mess);
    free_message_byte_array(MBA);

	/*
	 * PJOIN
	 */
    printf("\n\nPJOIN\n");


    //---------PJOIN using new struct--------
    pdu *pjoin = create_pjoin(8);
    pjoin->add_client_identity_timestamp(pjoin, 1505933137, "identity");

    printf("%s\n", pjoin->identity);

    MBA = pjoin->create_message(pjoin);
    printf("Length of message = %d\n", pjoin->get_message_length(pjoin));

    for(int i = 0;i < pjoin->get_message_length(pjoin);i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    pjoin->free_pdu(pjoin);
    free_message_byte_array(MBA);

	/*
	 * PLEAVE
	 */
    printf("\n\nPLEAVE\n");


    //---------PLEAVE using new struct-----------
    pdu *pleave = create_pleave(8);
    pleave->add_client_identity_timestamp(pleave, 1505933137, "identity");

    printf("%s\n", pleave->identity);

    MBA = pleave->create_message(pleave);
    printf("Length of message = %d\n", pleave->get_message_length(pleave));
    for(int i = 0;i < pleave->get_message_length(pleave);i++){
        printf("%d, ", MBA->array[i]);
    }
    printf("\n");

    pleave->free_pdu(pleave);
    free_message_byte_array(MBA);

	return 0;
}

