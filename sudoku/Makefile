CC=gcc
RM=rm -f

TARGET=sudoku

SOURCES=config.c core.c main.c print.c
OBJS=${SOURCES:.c=.o}

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} $^ -o $@

clean:
	$(RM) ${OBJS}

clobber: clean
	$(RM) ${TARGET}


