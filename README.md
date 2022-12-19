[![MacroCALC](https://mc.freakout.de/assets/logo.jpg "")](https://mc.freakout.de/ "MacroCALC")

mc - MacroCALC is a Lotus 1-2-3 compatible character based spreadsheet that
supports 100000 rows, 700 columns, 40 functions, display formats, file linking,
macro programming, pipes and user definable functions.  It runs on every
terminal or emulation that has a correct terminfo-entry.  The user-interface is
very convenient and well-known by 1-2-3 users.  Function- and arrow-keys and
extended video attributes are supported.

MacroCALC is the only spreadsheet that manages units: 1 mile + 1 km = 1.62
miles; 1 km / 1 hour * 30 sec = 8.33 m.  This can be seen as a physics engine.
On the way is also the support of complex numbers with complex arithmetic and
functions.  So sqrt(-1) = i (coming soon).

### Build (and run)
Building mc needs an ANSI-C compiler with curses-lib, lex and yacc.  Run
"./configure" followed by "make".  All programs are build into ./dist/arx and
can run without sudo-installation directly from the build-driectory.  To start
the spreadsheet simply: "dist/arx/bin/321"

### Install (system-wide)
"sudo make install" installs mc systemwide in /opt/arx/.  Nothing outside
/opt/arx/ will be touched in any way by the installation.  Removing mc is simply
done by a recursive "rm -r /opt/arx".  For convenience /opt/arx/bin should be
added to the PATH in a system appropriate manner.

### Getting started
Navigate through the sheet with the arrow-keys. The dot (.) marks the
beginning and end of a range, which can be copied or manipulated with the
menu by the slash (/). The apostrophe (') starts a text-cell, any other
key a number or formula. Open "mc" and type (`|v|` is the down-arrow-key):

    . ' h e l l o |v|  

you have marked A1 as range starting point and set A1 to text "hello".
Lets enter some numbers and a formula:

    1 |v| 2 |v| . a 2 + a 3 $ |v|  

you have now A2=1 A3=2 A4=3 and A1..A4 as range. Now navigate to B1 and:

    / c |v| 3 |v| 4 |v|

you have copied the range A1..A4 to B1..B4 and B2=3 B3=4 and B4=5.
Why is B3=5 and not 3 or 7? Because you typed $ after a3 this makes it
a relative cell reference and refers to B3 in the copied formula.

### License
    Simplified BSD License
    Copyright (c) 2022, Axel K. Reinhold
    All rights reserved.
