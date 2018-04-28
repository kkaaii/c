TARGET=app
CC=gcc
RM=rm

SOURCES=\
	main.c\
	host.c\
	device.c\
	pcie.c\
	../nvme_queue.c\
	../host/host_nvme.c\

CFLAGS+=-I ..
LFLAGS+=-lpthread

OBJS=$(SOURCES:.c=.o)

.PHONY = all clean clobber

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	-$(RM) $(OBJS)

clobber: clean
	-$(RM) $(TARGET)
