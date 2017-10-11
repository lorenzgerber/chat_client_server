#
#  Makefile for Chat client server
#
#	Project in 5DV197 Datakom
#   Created on: Sep 30, 2017
#       Author: lgerber
#
# 
# Usage: make EXEC=maindir   (e.g. make EXEC=test_parse)
#
# argument EXEC=maindir is used to choose which
# executable to build. It depicts a subdirectory
# of test/. 
 
CC = gcc
CFLAGS = -Wall -std=c99 -Werror -g 
LDFLAGS = -lpthread

TARGET_EXEC ?= $(EXEC)

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src/pdu
SRC_DIRS += ./src/netcom
SRC_DIRS += ./src/data_structures
SRC_DIRS += ./src/server
SRC_DIRS += ./src/client
SRC_DIRS += ./src/execs/$(EXEC)

SRCS := $(shell find $(SRC_DIRS) -name *.c )
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)


# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p

