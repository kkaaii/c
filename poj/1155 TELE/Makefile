CC=gcc
RM=rm -f

TARGET=1155
SOURCES=$(TARGET).c
CFLAGS +=
LDFLAGS +=
LDLIBS =

OBJS=$(SOURCES:.c=.o)

.PHONY: all clean clobber run

all: run clobber

$(TARGET): $(OBJS)

clean:
	-$(RM) $(OBJS)

clobber: clean
	-$(RM) $(TARGET)

run: $(TARGET)
	@echo "********** expected output **********"
	@cat $(TARGET).out
	@echo "**********  actual output  **********"
	@./$(TARGET) < $(TARGET).in
	@echo "**********  end of output  **********"
