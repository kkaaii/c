CC=gcc
RM=rm -f

SOURCES=\
	Log30h_IDENTIFY_DEVICE_data_log.c

CFLAGS += -I..
#ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
#endif

OBJS=$(SOURCES:.c=.o)

all: $(OBJS)

clean:
	$(RM) *.o
