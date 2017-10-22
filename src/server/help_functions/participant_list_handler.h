/*
 * participant_list_handler.h
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#ifndef SRC_SERVER_PARTICIPANT_LIST_HANDLER_H_
#define SRC_SERVER_PARTICIPANT_LIST_HANDLER_H_

#include <stdint.h>
#include "linked_list.h"
#include "string.h"

/**
 * get_number_identities
 *
 * function that calculates the number of
 * identities (client id's) in the name_list.
 * It is basically a list traversal.
 * @param name_list list that contains all client names
 * @return number of clients in list
 */
uint8_t get_number_identities(list* name_list);

/**
 * calc_length_identities
 *
 * Function that traverses the name_list and
 * sums up the length of all names (client id's).
 * @param name_list list of all client id's
 * @return character length in uint16_t of
 * all client id's in list
 */
uint16_t calc_length_identities(list* name_list);

/**
 * build_identities
 *
 * helper function to build char string with all
 * client identities from name_list. The function
 * requires the length of all identities to be
 * provided.
 * @param name_list list with client id's
 * @return char string with all client id's concatenated
 */
char* build_identities(list* name_list, uint16_t length_identities);

/**
 * remove_identity
 *
 * function to remove a specific client id from
 * the name list. Greedy matching is done using
 * strcmp.
 * @param identity_list list with client id's.
 * @param identity char string to be removed from the list
 */
int remove_identity(list *identity_list, char* identity);

#endif /* SRC_SERVER_PARTICIPANT_LIST_HANDLER_H_ */
