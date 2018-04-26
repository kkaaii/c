TARGET=app
CC=gcc
RM=rm

SOURCES=\
	main.c\
	host_nvme.c\

CFLAGS+=-I ..

OBJS=$(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	-$(RM) $(OBJS)

clobber: clean
	-$(RM) $(TARGET)
