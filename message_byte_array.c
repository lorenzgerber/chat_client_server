/*
 * message_byte_array.c
 *
 *  Created on: Sep 20, 2017
 *      Author: lgerber
 */
#include "message_byte_array.h"






int message_byte_array_add_uint8(message_byte_array *self, uint8_t data){

	return 0;
}


int message_byte_array_add_uint16(message_byte_array *self, uint16_t data){

	return 0;
}


int message_byte_array_add_uint32(message_byte_array *self, uint32_t data){

	return 0;
}


int message_byte_array_add_chars(message_byte_array *self, char* data, uint32_t legnth){

	return 0;
}


message_byte_array* create_message_byte_array(uint32_t length){
	message_byte_array *array = calloc(length, sizeof(uint8_t));
	array->add_uint8 = message_byte_array_add_uint8;
	array->add_uint16 = message_byte_array_add_uint16;
	array->add_uint32 = message_byte_array_add_uint32;
	array->add_chars = message_byte_array_add_chars;

	array->current_position = 0;

	return array;

}
