CC=gcc
RM=rm -f

TARGET=3132
SOURCES=$(TARGET).c
OBJS=$(SOURCES:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

clean:
	$(RM) $(OBJS)

clobber: clean
	$(RM) $(TARGET)

run: $(TARGET)
	@echo "********** expected output **********"
	@cat $(TARGET).out
	@echo "**********  actual output  **********"
	@./$(TARGET) < $(TARGET).in
