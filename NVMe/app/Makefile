TARGET=app
CC=gcc
RM=rm

SOURCES=\
	main.c\
	host.c\
	device.c\
	pcie.c\
	platform_linux.c\
	../nvme_queue.c\
	../host/nvme_host.c\
	../device/nvme_device.c\
	../host/admin/identify_host.c\
	../device/identify_device.c\
	identify_test.c\
	../host/admin/get_logpage_host.c\
	../device/get_logpage_device.c\
	get_logpage_test.c\

CFLAGS+=-I .. -I ../host -I ../device
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
