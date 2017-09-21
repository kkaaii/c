TARGET=main

CC=gcc
RM=rm -f

SOURCES=arbitration.c main.c
OBJS=$(SOURCES:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

.c.o:
	$(CC) -c $< -o $@

clean:
	$(RM) *.o

dist_clean: clean
	$(RM) $(TARGET)
