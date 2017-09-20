/*
 * message_byte_array.h
 *
 *  Created on: Sep 20, 2017
 *      Author: lgerber
 */

#ifndef MESSAGE_BYTE_ARRAY_H_
#define MESSAGE_BYTE_ARRAY_H_
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "pdu_templates.h"

typedef struct message_byte_array {
	uint8_t *array;
	uint32_t current_position;
	int (*add_uint8)(struct message_byte_array *self, uint8_t);
	int (*add_uint16)(struct message_byte_array *self, uint16_t);
	int (*add_uint32)(struct message_byte_array *self, uint32_t);
	int (*add_chars)(struct message_byte_array *self, char*, uint32_t);

} message_byte_array;


int message_byte_array_add_uint8(message_byte_array *self, uint8_t data);
int message_byte_array_add_uint16(message_byte_array *self, uint16_t data);
int message_byte_array_add_uint32(message_byte_array *self, uint32_t data);
int message_byte_array_add_chars(message_byte_array *self, char* data, uint32_t legnth);


message_byte_array* create_message_byte_array(uint32_t length);


#endif /* MESSAGE_BYTE_ARRAY_H_ */
