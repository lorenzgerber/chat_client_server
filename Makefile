# Makefile
CC=gcc
CFLAGS= -Wall -std=c99 -Werror -g -pthread

all: node

node: pdu_parser.c pdu_creator.c test_suite.c pdu_templates.c message_byte_array.c pdu_parser.h pdu_creator.h pdu_templates.h message_byte_array.h
	$(CC) $(CFLAGS) -o pdu_parser pdu_parser.c pdu_creator.c test_suite.c pdu_templates.c message_byte_array.c
	
clean:
	rm pdu_parser
