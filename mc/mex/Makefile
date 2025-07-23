# Makefile for MEX on Unix

ifneq (,$(wildcard /usr/include/ncursesw/curses.h))
  CURSESVARANT := ncursesw
else
  CURSESVARANT := ncurses
endif

CFLAGS=-Os -w
# -Os -w
# -Wall -Wextra -Wno-unused-parameter
# -w
# -g -O0
LIBS=-l$(CURSESVARANT)
SRC=utf82iso885915.c buffer.c cursor.c display.c files.c line.c random.c search.c term.c window.c word.c main.c
OBJ=utf82iso885915.o buffer.o cursor.o display.o files.o line.o random.o search.o term.o window.o word.o help.o

all: mex libmex.a

mex: main.o $(OBJ)
	$(CC) -o $@ $(LDFLAGS) main.o $(OBJ) $(LIBS)
	strip $@

libmex.a: mainloop.o $(OBJ)
	$(AR) rcs $@ mainloop.o $(OBJ)

$(SRC):	ed.h

main.o: main.c curkeys.h
	$(CC) $(CFLAGS) -Wno-missing-braces -c main.c

mainloop.o: main.c curkeys.h
	$(CC) $(CFLAGS) -DEMBEDDED -Wno-missing-braces -c main.c -o $@

term.o: term.c ed.h
	$(CC) $(CFLAGS) -I/usr/include/$(CURSESVARANT) -c term.c

curkeys.h: curkeys.c
	$(CC) -I/usr/include/$(CURSESVARANT) curkeys.c -o curkey2
	./curkey2 >$@
	rm curkey2

help.c: help_c.eng
	cp help_c.eng $@

clean:
	rm -f *.o core* mex me curkeys.h help.c curkey2 libmex.a mex.exe *.EXE *.OBJ HELP.C
