mc - MacroCALC is a Lotus 1-2-3 compatible character based spreadsheet that
supports 100000 rows, 700 columns, 40 functions, display formats, file
linking, macro programming and user definable functions. MacroCALC is fully
integrated with [nt]roff, units, man, awk, perl and sh.

As it should be in the UNIX-environment, mc can act as a filter enabling users
to use pipes to perform complex transforms on streams of data. It contains a
C programming interface as well as Lotus WKS and dBASE file support. mc's own
file-format is flat ASCII, easy to understand and can therefore be directly
manipulated with awk, perl or other standard UNIX-tools, or even with editors
like vi.

MacroCALC is the only spreadsheet that manages units: 1 mile + 1 km = 1.62
miles; 1 km / 1 hour * 30 sec = 8.33 m. This can be seen as a physics engine.

It's git-maintained, lint-clean ANSI-C source is based on yacc, lex and
[n]curses. It runs on every terminal or emulation that has a correct
terminfo-entry. The user-interface is very convenient and well-known by 1-2-3
users. Function- and cursor-keys and extended video attributes are supported.

The menus and messages are in separate editable files and can be easily
translated into any language. Every command in the package has a detailed
man-page.

mc was a professional commercial product, sold a couple hundred times for AIX,
UnixWare, SCO, Coherent and Interactive during the 1990-2005 years - now going
open-source under the "Simplified BSD License".

### build

mc was born in 1992 on Coherent (16-bit 286 Unix-Clone) - there was no
networking and a 64k (kilobyte!) address-room. So no automake. The
portability lies in a flexible Makefile and a portability-layer (like apaches
portable runtime). This layer lives in arx/. mc is an os-independent ansi-c
source and lives in mc/.

Building mc needs a ansi-C environment with curses-lib, lex and yacc (ncurses,
flex and bison for GNU-systems). Run "./configure" followed by "make". All
programs are built into ./dist/arx and can run without sudo-installation
directly from the build-driectory. To start the spreadsheet simply:

    dist/arx/bin/321

### install (system-wide)

"sudo make install" will install systemwide in /opt/arx/ after a successfull
build by simply untar'ing ./dist/arx.tar.gz into /opt. Nothing outside
/opt/arx/ will be touched in any way by the installation. Removing mc is
simply done by a recursive "rm -r /opt/arx". For convenience /opt/arx/bin
should be added to the PATH in a system appropriate manner. mc can also be
run by its alias "321" if "mc" is used for another program. If /opt/arx is
not appropriate for the admin the arx-directory can be moved or renamed
anywhere. In this case an environment-variable ARX must be exported with
the full path of the moved directory. MANPATH should also get the
man-directory. Here is an example for Linux/bash:

    # /etc/profile.d/arx.sh
    ARX=/opt/arx
    export ARX
    PATH=$PATH:$ARX/bin
    MANPATH=$MANPATH:$ARX/man

### license

> Simplified BSD License
Copyright (c) 2022, Axel K. Reinhold
All rights reserved.
