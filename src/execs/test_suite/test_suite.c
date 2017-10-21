/*
 * test_suite.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define GREEN "\033[32m"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pdu_creator.h"
#include "message_byte_array.h"
#include "pdu_serializer.h"
#include "pdu_parser.h"
#include "socket_templates.h"
#include "socket_creator.h"
#include "dummy_sockets.h"

int main(int argc, char*argv[]){

	message_byte_array *MBA;
    int errors = 0;
    int passed_reg = 1;
    int passed_alive = 1;
    int passed_ack = 1;
    int passed_notreg = 1;
    int passed_getlist = 1;
    int passed_slist = 1;
    int passed_join = 1;
    int passed_participants = 1;
    int passed_quit = 1;
    int passed_mess = 1;
    int passed_mess_checksum = 1;
    int passed_pjoin = 1;
    int passed_pleave = 1;

    /*
	 * Test REG
	 */
	pdu *reg = create_reg(10,2000);
	reg->add_server_name(reg,"servername");
    uint8_t expected_reg[16] = {0,10,7,208,115,101,114,118,101,114,110,97,109,101,0,0};

	MBA = reg->create_message(reg);
    if(reg->get_message_length(reg) != 16){
        passed_reg = 0;
        errors++;
    }
    for(int i = 0;i < reg->get_message_length(reg);i++){
        if(MBA->array[i] != expected_reg[i]){
            passed_reg = 0;
            errors++;
            break;
        }
	}
    if(passed_reg){
        printf("REG pdu serializing :          ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("REG pdu serializing :          ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 16\n");
        printf("Actual length: %d\n", reg->get_message_length(reg));
        printf("Expected segments:0, 10, 7, 208, 115, 101, 114, 118, 101, 114, 110, 97, 109, 101, 0, 0,\n");
        printf("Actual segments  :");
        for(int i = 0;i < reg->get_message_length(reg);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
	reg->free_pdu(reg);
	free_message_byte_array(MBA);

    /*
	 * Test ALIVE
	 */
    pdu *alive = create_alive(100, 10000);
    uint8_t expected_alive[4] = {2,100,39,16};

    MBA = alive->create_message(alive);
    if(alive->get_message_length(reg) != 4){
        passed_alive = 0;
        errors++;
    }
    for(int i = 0;i < alive->get_message_length(alive);i++){
        if(MBA->array[i] != expected_alive[i]){
            passed_alive = 0;
            errors++;
        }
    }
    if(passed_alive){
        printf("ALIVE pdu serializing :        ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("ALIVE pdu serializing :        ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 4\n");
        printf("Actual length: %d\n", alive->get_message_length(alive));
        printf("Expected segments:2, 100, 39, 16,\n");
        printf("Actual segments  :");
        for(int i = 0;i < alive->get_message_length(alive);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }

    alive->free_pdu(alive);
    free_message_byte_array(MBA);

	/*
	 * Test ACK
	 */
    pdu *ack = create_ack(10000);
    uint8_t expected_ack[4] = {1,0,39,16};

    MBA = ack->create_message(ack);
    if(ack->get_message_length(ack) != 4){
        passed_ack = 0;
        errors++;
    }
    for(int i = 0;i < ack->get_message_length(ack);i++){
        if(MBA->array[i] != expected_ack[i]){
            passed_ack = 0;
            errors++;
        }
    }
    if(passed_ack){
        printf("ACK pdu serializing :          ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("ACK pdu serializing :          ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 4\n");
        printf("Actual length: %d\n", ack->get_message_length(ack));
        printf("Expected segments:1, 0, 39, 16,\n");
        printf("Actual segments  :");
        for(int i = 0;i < ack->get_message_length(ack);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }

    ack->free_pdu(ack);
    free_message_byte_array(MBA);

    /*
	 * Test NOTREG
	 */
    pdu *notreg = create_notreg(10000);
    uint8_t expected_notreg[4] = {100,0,39,16};

    MBA = notreg->create_message(notreg);
    if(notreg->get_message_length(notreg) != 4){
        passed_notreg = 0;
        errors++;
    }
    for(int i = 0;i < notreg->get_message_length(notreg);i++){
        if(MBA->array[i] != expected_notreg[i]){
            passed_notreg = 0;
            errors++;
        }
    }
    if(passed_notreg){
        printf("NOTREG pdu serializing :       ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("NOTREG pdu serializing :       ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 4\n");
        printf("Actual length: %d\n", notreg->get_message_length(notreg));
        printf("Expected segments:100, 0, 39, 16,\n");
        printf("Actual segments  :");
        for(int i = 0;i < notreg->get_message_length(notreg);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    notreg->free_pdu(notreg);
    free_message_byte_array(MBA);

	/*
	 * GETLIST
	 */
    pdu *getlist = create_getlist();
    uint8_t expected_getlist[4] = {3,0,0,0};

    MBA = getlist->create_message(getlist);
    if(getlist->get_message_length(getlist) != 4){
        passed_getlist = 0;
        errors++;
    }
    for(int i = 0;i < getlist->get_message_length(getlist);i++){
        if(MBA->array[i] != expected_getlist[i]){
            passed_getlist = 0;
            errors++;
        }
    }
    if(passed_getlist){
        printf("GETLIST pdu serializing :      ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("GETLIST pdu serializing :      ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 4\n");
        printf("Actual length: %d\n", getlist->get_message_length(getlist));
        printf("Expected segments:3, 0, 0, 0,\n");
        printf("Actual segments  :");
        for(int i = 0;i < getlist->get_message_length(getlist);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    getlist->free_pdu(getlist);
    free_message_byte_array(MBA);

    /*
	 * SLIST
	 */
    pdu *slist = create_slist(2);
    uint8_t expected_slist[44] = {4,0,0,2,127,0,0,1,7,208,4,10,115,101,114,118,101,114,110,97,109,101,0,0,127,0,0,2,7,209,4,11,115,101,114,118,101,114,110,97,109,101,50,0};

    uint8_t address3[4] = {127,0,0,1};
    pdu_server_entry* server3 = create_server_entry(address3,2000,4,10);
    server3->add_server_name(server3, "servername");
    slist->add_server_entry(slist, server3);

    uint8_t address4[4] = {127,0,0,2};
    pdu_server_entry* server4 = create_server_entry(address4,2001,4,11);
    server4->add_server_name(server4, "servername2");
    slist->add_server_entry(slist, server4);

    MBA = slist->create_message(slist);
    if(slist->get_message_length(slist) != 44){
        passed_slist = 0;
        errors++;
    }
    for(int i = 0; i < slist->get_message_length(slist); i++){
        if(MBA->array[i] != expected_slist[i]){
            passed_slist = 0;
            errors++;
        }
    }
    if(passed_slist){
        printf("SLIST pdu serializing :        ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("SLIST pdu serializing :        ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 4\n");
        printf("Actual length: %d\n", slist->get_message_length(slist));
        printf("Expected segments:4, 0, 0, 2, 127, 0, 0, 1, 7, 208, 4, 10, 115, 101, 114, 118, 101, 114, 110, 97, 109, 101, 0, 0, 127, 0, 0, 2, 7, 209, 4, 11, 115, 101, 114, 118, 101, 114, 110, 97, 109, 101, 50, 0,\n");
        printf("Actual segments  :");
        for(int i = 0;i < slist->get_message_length(slist);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    slist->free_pdu(slist);
    free_message_byte_array(MBA);

	/*
	 * JOIN
	 */
    pdu *join = create_join(8);
    join->add_identity(join, "identity");
    uint8_t expected_join[12] = {12, 8, 0, 0, 105, 100, 101, 110, 116, 105, 116, 121};

    MBA = join->create_message(join);
    if(join->get_message_length(join) != 12){
        passed_join = 0;
        errors++;
    }
    for(int i = 0; i < join->get_message_length(join); i++){
        if(MBA->array[i] != expected_join[i]){
            passed_join = 0;
            errors++;
        }
    }
    if(passed_join){
        printf("JOIN pdu serializing :         ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("JOIN pdu serializing :         ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 4\n");
        printf("Actual length: %d\n", slist->get_message_length(slist));
        printf("Expected segments:12, 8, 0, 0, 105, 100, 101, 110, 116, 105, 116, 121,\n");
        printf("Actual segments  :");
        for(int i = 0;i < join->get_message_length(join);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    join->free_pdu(join);
    free_message_byte_array(MBA);

	/*
	 * PARTICIPANTS
	 */
    pdu *participants = create_participants(3, 15);
    participants->add_identities(participants, "partic\0ipa\0nts\0");
    uint8_t expected_participants[20] = {19, 3, 0, 15, 112, 97, 114, 116, 105, 99, 0, 105, 112, 97, 0, 110, 116, 115, 0, 0};
    //TODO kollar egentligen inte serialisering men koden kan vara bra o ha till parsningstest kanske
    char expected_part1[6] = {'p','a','r','t','i','c'};
    char expected_part2[3] = {'i','p','a'};
    char expected_part3[3] = {'n','t','s'};
    char* part_pointer[3];
    part_pointer[0] = expected_part1;
    part_pointer[1] = expected_part2;
    part_pointer[2] = expected_part3;
    for(int i = 0; i < participants->number_identities;i++){
        for(int j = 0; j < strlen(participants->identities[i]); j++){
            if(part_pointer[i][j] != participants->identities[i][j]){
                passed_participants = 0;
                errors++;
            }
        }
    }

    MBA = participants->create_message(participants);
    if(participants->get_message_length(participants) != 20 ||
            participants->number_identities != 3){
        passed_participants = 0;
        errors++;
    }
    for(int i = 0; i < participants->get_message_length(participants); i++){
        if(MBA->array[i] != expected_participants[i]){
            passed_participants = 0;
            errors++;
        }
    }
    if(passed_participants){
        printf("PARTICIPANTS pdu serializing : ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("PARTICIPANTS pdu serializing : ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 20\n");
        printf("Actual length: %d\n", participants->get_message_length(participants));
        printf("Expected segments:19, 3, 0, 15, 112, 97, 114, 116, 105, 99, 0, 105, 112, 97, 0, 110, 116, 115, 0, 0,\n");
        printf("Actual segments  :");
        for(int i = 0;i < participants->get_message_length(participants);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    participants->free_pdu(participants);
    free_message_byte_array(MBA);

	/*
	 * QUIT
	 */
    pdu *quit = create_quit();
    uint8_t expected_quit[4] = {11, 0, 0, 0};

    MBA = quit->create_message(quit);
    if(quit->get_message_length(quit) != 4){
        passed_quit = 0;
        errors++;
    }
    for(int i = 0;i < quit->get_message_length(quit);i++){
        if(MBA->array[i] != expected_quit[i]){
            passed_quit = 0;
            errors++;
        }
    }
    if(passed_quit){
        printf("QUIT pdu serializing :         ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("QUIT pdu serializing :         ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 4\n");
        printf("Actual length: %d\n", slist->get_message_length(slist));
        printf("Expected segments:11, 0, 0, 0,\n");
        printf("Actual segments  :");
        for(int i = 0;i < quit->get_message_length(quit);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    quit->free_pdu(quit);
    free_message_byte_array(MBA);

	/*
	 * MESS
	 */
    pdu *mess = create_mess(8, 23);
    mess->add_message(mess, 13, 1505933137, "Test Message.");
    mess->add_identity(mess, "identity");
    uint8_t expected_mess[36] = {10, 0, 8, 150, 0, 13, 0, 0, 81, 183, 194, 89, 84, 101, 115, 116, 32, 77, 101, 115, 115, 97, 103, 101, 46, 0, 0, 0, 105, 100, 101, 110, 116, 105, 116, 121};

    mess->set_checksum(mess);
    if (mess->verify_checksum(mess)!=0){
        passed_mess_checksum = 0;
        errors++;
    }

    MBA = mess->create_message(mess);
    if(mess->get_message_length(mess) != 36){
        passed_mess = 0;
        errors++;
    }
    for(int i = 0;i < mess->get_message_length(mess);i++){
        if(MBA->array[i] != expected_mess[i]){
            passed_mess = 0;
            errors++;
        }
    }
    if(passed_mess && passed_mess_checksum){
        printf("MESS pdu serializing :         ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("MESS pdu serializing :         ");
        printf(RED"FAILED\n"RESET);
        if(!passed_mess_checksum){
            printf("checksum calculations failed!\n");
        }
        printf("Expected length: 36\n");
        printf("Actual length: %d\n", mess->get_message_length(mess));
        printf("Expected message: Test Message.\n");
        printf("Actual message  : %s\n", mess->message);
        printf("Expected segments:10, 0, 8, 150, 0, 13, 0, 0, 81, 183, 194, 89, 84, 101, 115, 116, 32, 77, 101, 115, 115, 97, 103, 101, 46, 0, 0, 0, 105, 100, 101, 110, 116, 105, 116, 121,\n");
        printf("Actual segments  :");
        for(int i = 0;i < mess->get_message_length(mess);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    mess->free_pdu(mess);
    free_message_byte_array(MBA);

	/*
	 * PJOIN
	 */
    pdu *pjoin = create_pjoin(8);
    pjoin->add_client_identity_timestamp(pjoin, 1505933137, "identity");
    uint8_t expected_pjoin[16] = {16, 8, 0, 0, 81, 183, 194, 89, 105, 100, 101, 110, 116, 105, 116, 121};

    MBA = pjoin->create_message(pjoin);
    if(pjoin->get_message_length(pjoin) != 16){
        passed_pjoin = 0;
        errors++;
    }
    for(int i = 0;i < pjoin->get_message_length(pjoin);i++){
        if(MBA->array[i] != expected_pjoin[i]){
            passed_pjoin = 0;
            errors++;
        }
    }
    if(passed_pjoin){
        printf("PJOIN pdu serializing :        ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("PJOIN pdu serializing :        ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 16\n");
        printf("Actual length: %d\n", pjoin->get_message_length(pjoin));
        printf("Expected segments:16, 8, 0, 0, 81, 183, 194, 89, 105, 100, 101, 110, 116, 105, 116, 121,\n");
        printf("Actual segments  :");
        for(int i = 0;i < pjoin->get_message_length(pjoin);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    pjoin->free_pdu(pjoin);
    free_message_byte_array(MBA);

	/*
	 * PLEAVE
	 */
    pdu *pleave = create_pleave(8);
    pleave->add_client_identity_timestamp(pleave, 1505933137, "identity");
    uint8_t expected_pleave[16] = {17, 8, 0, 0, 81, 183, 194, 89, 105, 100, 101, 110, 116, 105, 116, 121};

    MBA = pleave->create_message(pleave);
    if(pleave->get_message_length(pleave) != 16){
        passed_pleave = 0;
        errors++;
    }
    for(int i = 0;i < pleave->get_message_length(pleave);i++){
        if(MBA->array[i] != expected_pleave[i]){
            passed_pleave = 0;
            errors++;
        }
    }
    if(passed_pleave){
        printf("PLEAVE pdu serializing :       ");
        printf(GREEN"OK\n"RESET);
    }else{
        printf("PLEAVE pdu serializing :       ");
        printf(RED"FAILED\n"RESET);
        printf("Expected length: 16\n");
        printf("Actual length: %d\n", pleave->get_message_length(pleave));
        printf("Expected segments:17, 8, 0, 0, 81, 183, 194, 89, 105, 100, 101, 110, 116, 105, 116, 121,\n");
        printf("Actual segments  :");
        for(int i = 0;i < pleave->get_message_length(pleave);i++){
            printf("%d, ", MBA->array[i]);
        }
        printf("\n");
    }
    pleave->free_pdu(pleave);
    free_message_byte_array(MBA);

    printf("\nTest suite finished with %d errors", errors);

	return 0;
}

