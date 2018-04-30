CFLAGS += -Wall -nostdlib -ffreestanding
LDFLAGS += -nostdlib -ffreestanding
CC = gcc

HEADERS = ece391player_engine.h ece391game.h

ALL: shell ardutest GAME_C radiotest paint messenger

GAME_C: ece391player_engine.o ece391game.o
	$(CC) $(LDFLAGS) ece391player_engine.o  ece391support.o ece391syscall.o  ece391game.o -o game

%.o: %.c ${HEADERS}
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -Wall -o $@ $<

%.exe: ece391%.o ece391syscall.o ece391support.o
	$(CC) $(LDFLAGS) -o $@ $^

%: %.exe
	../elfconvert $<
	mv $<.converted to_fsdir/$@

rem:
	rm -f *.o

clean::
	rm -f *~ *.o

clear: clean
	rm -f *.converted
	rm -f *.exe
	rm -f to_fsdir/*
