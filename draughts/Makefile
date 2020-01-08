CC=gcc

.PHONY = all run clean

all: player judger

player: player.o

judger: judger.o

run: player judger
	./judger ./player ./player

clean:
	rm -f *.o player judger
