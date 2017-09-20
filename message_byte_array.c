/*
 * message_byte_array.c
 *
 *  Created on: Sep 20, 2017
 *      Author: lgerber
 */
#include "message_byte_array.h"


message_byte_array* create_message_byte_array(uint32_t length){
	message_byte_array array = calloc(length, sizeof(uint8_t));
	return array;

}
