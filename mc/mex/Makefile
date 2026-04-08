# Makefile for MEX on Unix

ifneq (,$(wildcard /usr/lib/libncursesw.*)$(wildcard /usr/include/ncursesw/curses.h))
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

LIBS=-l$(CURSESVARANT) $(TINFOLIB)
SRC=       utf82iso885915.c buffer.c cursor.c display.c files.c help.c line.c random.c search.c term.c window.c word.c main.c
OBJ=       utf82iso885915.o buffer.o cursor.o                   help.o line.o random.o search.o        window.o word.o
OBY=main.o                                    display.o files.o                                 term.o
OBE=mainloop.o                                display.o filese.o                                terme.o
OBV=mainvt1.o                                displayv.o filesv.o                                termv.o

all: mex me1 libmex.a

test: libmex.a

small: mex libmex.a

mex: $(OBY) $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBY) $(OBJ) $(LIBS)
	strip $@

me1: $(OBV) $(OBJ)
	$(CC) -static -o $@ $(LDFLAGS) $(OBV) $(OBJ) $(LIBS)
	strip $@

libmex.a: $(OBE) $(OBJ)
	$(AR) rcs $@ $(OBE) $(OBJ)

$(SRC):	ed.h

main.o: main.c curkeys.h
	$(CC) $(CFLAGS) -Wno-missing-braces -c main.c

mainloop.o: main.c curkeys.h
	$(CC) $(CFLAGS) -DEMBEDDED -Wno-missing-braces -c main.c -o $@

mainvt1.o: main.c curkeys.h
	$(CC) $(CFLAGS) -DVT100 -Wno-missing-braces -c main.c -o $@

displayv.o: display.c
	$(CC) $(CFLAGS) -DVT100 -c display.c -o $@

filese.o: files.c
	$(CC) $(CFLAGS) -DEMBEDDED -c files.c -o $@

filesv.o: files.c
	$(CC) $(CFLAGS) -DVT100 -c files.c -o $@

terme.o: term.c ed.h
	$(CC) $(CFLAGS) -DEMBEDDED -I/usr/include/$(CURSESVARANT) -c term.c -o $@

termv.o: term.c ed.h
	$(CC) $(CFLAGS) -DVT100 -c term.c -o $@

term.o: term.c ed.h
	$(CC) $(CFLAGS) -I/usr/include/$(CURSESVARANT) -c term.c

termvt1.o: term.c ed.h
	$(CC) $(CFLAGS) -DVT100 -c term.c -o $@

utf82iso885915.o: utf82iso885915.c
	$(CC) -c utf82iso885915.c

curkeys.h: curkeys.c
	$(CC) --std=c99 -I/usr/include/$(CURSESVARANT) curkeys.c -o curkey2
	./curkey2 >$@
	rm curkey2

help.c: help_c.eng
	cp help_c.eng $@

clean:
	rm -f *.o core* me[xs1v] me curkeys.h help.c curkey2 libmex.a mex.exe *.EXE *.OBJ HELP.C
