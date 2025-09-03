# Makefile for MEX on Unix

ifneq (,$(wildcard /usr/lib/libncursesw.a)$(wildcard /usr/include/ncursesw/curses.h))
  CURSESVARANT := ncursesw
else
  CURSESVARANT := ncurses
endif

ifneq (,$(wildcard /usr/lib64/libtinfow.so.6))
  TINFOLIB := -ltinfow
else
  TINFOLIB := 
endif

ifeq (small,$(MAKECMDGOALS))
  CFLAGS=-Os -w
else
  CFLAGS=--std=c99 -O3 -Wall -Wextra -Wno-unused-parameter
endif
# -Os -w
# -O3 -Wall -Wextra -Wno-unused-parameter
# -g -O0
LIBS=-l$(CURSESVARANT) $(TINFOLIB)
SRC=utf82iso885915.c buffer.c cursor.c display.c files.c line.c random.c search.c term.c window.c word.c main.c
OBJ=utf82iso885915.o buffer.o cursor.o display.o         line.o random.o search.o        window.o word.o

all: mex libmex.a

test: libmex.a

small: test

mex: main.o $(OBJ) files.o term.o help.o
	$(CC) -o $@ $(LDFLAGS) main.o $(OBJ) files.o term.o help.o $(LIBS)
	strip $@

libmex.a: mainloop.o filese.o terme.o $(OBJ)
	$(AR) rcs $@ mainloop.o filese.o terme.o $(OBJ)

$(SRC):	ed.h

main.o: main.c curkeys.h
	$(CC) $(CFLAGS) -Wno-missing-braces -c main.c

mainloop.o: main.c curkeys.h
	$(CC) $(CFLAGS) -DEMBEDDED -Wno-missing-braces -c main.c -o $@

filese.o: files.c
	$(CC) $(CFLAGS) -DEMBEDDED -c files.c -o $@

terme.o: term.c ed.h
	$(CC) $(CFLAGS) -DEMBEDDED -I/usr/include/$(CURSESVARANT) -c term.c -o $@

term.o: term.c ed.h
	$(CC) $(CFLAGS) -I/usr/include/$(CURSESVARANT) -c term.c

utf82iso885915.o: utf82iso885915.c
	$(CC) -c utf82iso885915.c

curkeys.h: curkeys.c
	$(CC) --std=c99 -I/usr/include/$(CURSESVARANT) curkeys.c -o curkey2
	./curkey2 >$@
	rm curkey2

help.c: help_c.eng
	cp help_c.eng $@

clean:
	rm -f *.o core* mex me curkeys.h help.c curkey2 libmex.a mex.exe *.EXE *.OBJ HELP.C
