/*
 * participant_list_handler.c
 *
 *
 *  Created on: Oct 1, 2017
 *     Authors: Lorenz Gerber, Niklas Königsson
 *
 *  Chat client server project
 *  5DV197 Datakom course
 *	GPLv3
 */

#include "participant_list_handler.h"

/**
 * get_number_identities
 *
 * function that calculates the number of
 * identities (client id's) in the name_list.
 * It is basically a list traversal.
 * @param name_list list that contains all client names
 * @return number of clients in list
 */
uint8_t get_number_identities(list* name_list){
	int length = 0;
	uint16_t clients_count = 0;

	list_position current_position = list_first(name_list);

	// determine length
	if(!list_is_empty(name_list)){
		do{
			length += strlen((char*)list_inspect(current_position))+1;
			if(!list_is_end(name_list, current_position)){
				current_position = list_next(current_position);
				clients_count++;
			}
		} while (!list_is_end(name_list, current_position));
		clients_count++;
	}

	return clients_count;
}

/**
 * calc_length_identities
 *
 * Function that traverses the name_list and
 * sums up the length of all names (client id's).
 * @param name_list list of all client id's
 * @return character length in uint16_t of
 * all client id's in list
 */
uint16_t calc_length_identities(list* name_list){
	uint16_t length = 0;

	list_position current_position = list_first(name_list);

	// determine length
	if(!list_is_empty(name_list)){
		do{

			if(!list_is_end(name_list, current_position)){
				length += strlen((char*)list_inspect(current_position))+1;
				current_position = list_next(current_position);
			}
		} while (!list_is_end(name_list, current_position));
		length += strlen((char*)list_inspect(current_position))+1;
	}

	return length;

}

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
char* build_identities(list* name_list, uint16_t length_identities){

	int length_of_name;
	int str_position = 0;

	list_position current_position = list_first(name_list);

	// allocate memory, iterate and collect all names
	char* identities = malloc(length_identities * sizeof(char));
	current_position = list_first(name_list);

	if(!list_is_empty(name_list)){
		do{
			if(!list_is_end(name_list, current_position)){

				length_of_name = strlen((char *)list_inspect(current_position));
				memmove(&identities[str_position], (char*) list_inspect(current_position), length_of_name+1);
				str_position += length_of_name+1;
				current_position = list_next(current_position);
			}

		} while (!list_is_end(name_list, current_position));
		length_of_name = strlen((char *)list_inspect(current_position));
		memmove(&identities[str_position], (char*) list_inspect(current_position), length_of_name+1);
	}

	return identities;
}


/**
 * remove_identity
 *
 * function to remove a specific client id from
 * the name list. Greedy matching is done using
 * strcmp.
 * @param identity_list list with client id's.
 * @param identity char string to be removed from the list
 */
int remove_identity(list *identity_list, char* identity){

	list_position current_position = list_first(identity_list);

	if(!list_is_empty(identity_list)){
		do{
			if(strcmp(identity, (char*)list_inspect(current_position))==0){
				list_remove(identity_list, current_position);
				return 0;
			}

			if(!list_is_end(identity_list, current_position)){

				current_position = list_next(current_position);
			}
		} while (!list_is_end(identity_list, current_position));

		if(strcmp(identity, (char*)list_inspect(current_position))==0){
			list_remove(identity_list, current_position);
			return 0;
		}

	}

	return 0;
}
