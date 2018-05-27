BUILD_HOME=../..

all: images

BINARY = main

CFLAGS += -I$(BUILD_HOME)/dsr_workshop/ws_morse/libmorse
LDFLAGS += -L$(BUILD_HOME)/dsr_workshop/ws_morse/libmorse
LDLIBS += -lmorse

include $(BUILD_HOME)/dsr_workshop/Makefile.include
