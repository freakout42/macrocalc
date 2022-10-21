# $Id: mcuser.mk,v 1.20 2017/07/11 13:17:25 axel Exp $

CC=$Cc
CFLAGS=$Cflag
CURLIB=$Curslib
LEXLIB=$Llib
ARXLIB=../../arx/lib/libarx.a

PROGS=mc mc2wks mcread

all:	$(PROGS)

install: all
	-strip $(PROGS)
	mv $(PROGS) ../../bin

mc:	mc.o mcpary.o mc_kern.o mc_io.o libmcf.a mcftab.o mcuser.o
	$(CC) $(CFLAGS) mc.o mcpary.o mc_kern.o mc_io.o mcftab.o mcuser.o libmcf.a $(ARXLIB) $(CURLIB) $(LEXLIB) -lm -o $@

mc2wks:	mc2wks.o mc2pary.o mc_kern.o libmcf.a mcftab.o mcuser.o
	$(CC) $(CFLAGS) mc2wks.o mc2pary.o mc_kern.o mcftab.o mcuser.o libmcf.a $(ARXLIB) $(LEXLIB) -lm -o $@

mcread:	mcread.o mcpary.o mc_kern.o libmcf.a mcftab.o mcuser.o
	$(CC) $(CFLAGS) mcread.o mcpary.o mc_kern.o mcftab.o mcuser.o libmcf.a $(ARXLIB) $(LEXLIB) -lm -o $@
