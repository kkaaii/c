TARGET=json

CC=gcc
RM=rm -f

SOURCES=json.c main.c
OBJS=$(SOURCES:.c=.o)
CFLAGS=-g

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

clean:
	$(RM) $(OBJS)

clobber: clean
	$(RM) $(TARGET)

