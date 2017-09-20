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

	uint32_t value = data;
	uint8_t result[4];

	result[0] = (value & 0x000000ff);
	result[1] = (value & 0x0000ff00) >> 8;
	result[2] = (value & 0x00ff0000) >> 16;
	result[3] = (value & 0xff000000) >> 24;

	for(int i = 0; i < 4; i++){
		self->array[self->current_write_position] = result[i];
		self->current_write_position++;
	}

	return 0;
}


int message_byte_array_add_chars(message_byte_array *self, char* data, uint32_t legnth){

	return 0;
}


message_byte_array* create_message_byte_array(uint32_t length){
	message_byte_array *array = malloc(length*sizeof(uint8_t));
	array->add_uint8 = message_byte_array_add_uint8;
	array->add_uint16 = message_byte_array_add_uint16;
	array->add_uint32 = message_byte_array_add_uint32;
	array->add_chars = message_byte_array_add_chars;

	array->current_write_position = 0;

	return array;

}
