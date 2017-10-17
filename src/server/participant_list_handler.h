/*
 * participant_list_handler.h
 *
 *  Created on: Oct 16, 2017
 *      Author: lgerber
 */

#ifndef SRC_SERVER_PARTICIPANT_LIST_HANDLER_H_
#define SRC_SERVER_PARTICIPANT_LIST_HANDLER_H_

#include <stdint.h>
#include "linked_list.h"
#include "string.h"

uint8_t get_number_identities(list* name_list);

uint16_t calc_length_identities(list* name_list);

char* build_identities(list* name_list, uint16_t length_identities);

int remove_identity(list *identity_list, char* identity);

#endif /* SRC_SERVER_PARTICIPANT_LIST_HANDLER_H_ */