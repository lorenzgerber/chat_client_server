# Makefile
CC=gcc
CFLAGS= -Wall -std=c99 -Werror -g -pthread

all: node

node: pdu_parser.c parser_test.c pdu_templates.c pdu_parser.h pdu_templates.h
	$(CC) $(CFLAGS) -o pdu_parser pdu_parser.c parser_test.c pdu_templates.c
	
clean:
	rm pdu_parser
