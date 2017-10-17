CC=gcc
RM=rm -f

TARGET=1014
SOURCES=knapsack.c 1014.c
OBJS=$(SOURCES:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

clean:
	$(RM) $(OBJS)

clobber: clean
	$(RM) $(TARGET)

