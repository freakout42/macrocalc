**mc** - MacroCALC is a Lotus 1-2-3 compatible character based spreadsheet that
supports 100000 rows, 700 columns, 40 functions, display formats, file linking,
macro programming, pipes and user definable functions. It runs on every
terminal or emulation that has a correct terminfo-entry. The user-interface is
very convenient and well-known by 1-2-3 users. Function- and arrow-keys and
extended video attributes are supported.

[![MacroCALC](https://mc.freakout.de/assets/logo.jpg "MacroCALC")](https://mc.freakout.de/ "MacroCALC")

MacroCALC is the only spreadsheet that manages units: 1 mile + 1 km = 1.62 miles;
1 km / 1 hour * 30 sec = 8.33 m. This can be seen as a physical engine.
It also has support of complex numbers with complex arithmetic and functions.
So sqrt(-1) = i and E ^ (I * PI) + 1 = 0.

6 Install options
-----------------

1. __Build from source und install for one user only without sudo__  
Building mc needs an ANSI-C compiler with curses-lib, lex and yacc. Clone and
`./configure` followed by `make`. All programs are build into `./dist/arx` and
can run without sudo-installation directly from the build-directory. To start
the spreadsheet simply: `dist/arx/bin/321` - for a user-install you are done.

2. __Install systemwide__  
After build (1.) `sudo make install` installs in `/opt/arx/`. Nothing outside
`/opt/arx/` will be touched in any way by the installation. Removing mc is simply
done by a recursive `rm -r /opt/arx`. For convenience `/opt/arx/bin` should be
added to the PATH in a system appropriate manner. `321` or `mc` will then work.

3. __Binary with precompiled tarball__  
Download the released asset `mc-<releasetag>-<platform>-<hardware>.tar.gz` for
your platform (lu-Linux l6-Linux64 fb-FreeBSD md-MacOS) and untar to `/opt/arx`.
Add `/opt/arx/bin` to $PATH and you are done to use the system by `321` or `mc`.

4. __RPM package install__  
Download `macrocalc-<releasetag>.i386.rpm` and  
`sudo rpm --nodigest --nofiledigest -i macrocalc-<releasetag>.i386.rpm`  
`sudo ldconfig`  
After a relogin macrocalc will start by `321` or `mc`.  
You need i686 glibc installed on 64bit machines: `sudo yum install glibc.i686`

5. __Docker image__  
`docker pull ghcr.io/freakout42/macrocalc:latest`  
`export COLUMNS="$(tput cols)"`  
`export LINES="$(tput lines)"`  
`docker run -e COLUMNS -e LINES -e TERM -ti ghcr.io/freakout42/macrocalc:latest`  

6. Windows .exe Download  
https://github.com/freakout42/macrocalc/releases/download/v10.51/mc-10.52-w3-i386.exe.zip  
and unzip to mc.exe which can be directly executed with doubleclick or
installed on the PATH (ex.: C:\Windows\System32) and then started from a powershell
or cmd console or from the new windows terminal with `mc`.

Getting started
---------------
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

you have copied the range A1..A4 to B1..B4 and set B2=3 B3=4 and B4=5.
Why is B4=5 and not 3 or 7? Because you typed $ after a3 this makes it
a relative cell reference [+0;-1] and refers to B3 in the copied formula.

Using units
-----------

**REMEMBER TO START MacroCALC WITH THE `-u` OPTION TO USE UNITS**

To enter a value with an unit add the unit at the end enclosed in
curly brackets. For example to enter 1 meter and 1 km and add both:

    1 { m } |v| 1 { k m } |v| a 1 + a 2 |v|

you will see `1001 m` as the result.

License
-------
    Simplified BSD License
    Copyright (c) 2024, Axel K. Reinhold
    All rights reserved.
