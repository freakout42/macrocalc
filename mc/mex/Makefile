# mex/Makefile

CFLAGS=-Os -Wall -Wno-error
LIBS=-lcurses
OBJ=buffer.o cursor.o display.o files.o line.o main.o random.o search.o term.o window.o word.o help.o

mex: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LIBS)
	strip $@

$(OBJ):	ed.h

main.o: main.c curkeys.h
	$(CC) $(CFLAGS) -Wno-missing-braces -c main.c

curkeys.h: curkeys
	./curkeys >$@

help.c: help_c.eng
	cp help_c.eng $@

clean:
	rm -f *.o core mex me curkeys.h help.c curkeys
