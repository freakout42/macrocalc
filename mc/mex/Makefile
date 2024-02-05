# Makefile for MEX on Unix

CFLAGS=-Os -Wall -Wno-error
LIBS=-lcurses
SRC=buffer.c cursor.c display.c files.c line.c main.c random.c search.c term.c window.c word.c
OBJ=buffer.o cursor.o display.o files.o line.o main.o random.o search.o term.o window.o word.o help.o

mex: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LIBS)
	strip $@

$(SRC):	ed.h

main.o: main.c curkeys.h
	$(CC) $(CFLAGS) -Wno-missing-braces -c main.c

term.o: term.c ed.h
	$(CC) $(CFLAGS) -I/usr/include/ncurses -c term.c

curkeys.h: curkeys.c
	$(CC) -I/usr/include/ncurses curkeys.c -o curkey2
	./curkey2 >$@
	rm curkey2

help.c: help_c.eng
	cp help_c.eng $@

clean:
	rm -f *.o core mex me curkeys.h help.c curkey2
