/*
 * message_byte_array.h
 *
 *  Created on: Sep 20, 2017
 *      Author: lgerber
 */

#ifndef MESSAGE_BYTE_ARRAY_H_
#define MESSAGE_BYTE_ARRAY_H_
#include "pdu_templates.h"

typedef struct message_byte_array {
	uint8_t *array;
	uint32_t current_position;

} message_byte_array;

message_byte_array* create_message_byte_array(uint32_t length);


#endif /* MESSAGE_BYTE_ARRAY_H_ */
