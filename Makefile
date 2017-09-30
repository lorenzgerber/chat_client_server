#COMPILER
CC = gcc

VPATH = src:src/pdu:src/test

#CFlAGS AND LIBS
CFLAGS = -std=c99 -Wall -Werror

LIBS = -pthread -lm

#SOURCE FILES
SRCS := test_suite.c pdu_creator.c pdu_serializer.c pdu_templates.c message_byte_array.c pdu_parser.c

#HEADER FILES
HDRS = pdu_creator.h pdu_serializer.h pdu_templates.h message_byte_array.h pdu_parser.h

#Rules
OBJS := $(SRCS:.c=.o) $(HDRS)

DEPS := $(OBJS:.o=.d)

TARGET = test_suite

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@


#MISC CMDS:
clean:
	rm -vf *.o $(TARGET)
