.Id mcuser.r
.SH "User functions"
User functions are functions developed in the C programming language.
They must be compiled with the C compiler and linked with the kernel of
\fBmc\fP.
Everything that is needed to build a special version with user functions
is included in
\fBmc\fP.
To build a new
.B mc
the following steps must be completed:
.IP 1.
.ig
Copy the file
.I $ARX/bin/mcdevel.tar
to a directory where sources are stored (i.e. \fI/usr/src\fP).
.IP 2.
Go to that directory and untar the file with:
.DS
tar xvf mcdevel.tar
.DE
.P
This creates a new directory in the current directory named \fImcdevel\fP.
.IP 3.
Change into the new directory \fImcdevel\fP.
..
Change into the directory \fI$ARX/mc/src\fP.
This directory contains following files:
.ST
.IP makefile 16
Makefile for the new mc
.IP mc_kern.o 16
kernel of mc
.IP libmcf.a 16
archive with mc functions
.IP f.tab.h 16
header for function types
.IP mcfunc.h 16
header for functions
.IP mcftab.c 16
function table
.IP mcuser.c 16
user functions source code
.ET
.ig
.IP 4.
Activate your license with
.DS
arxactiv mckernel.o
.DE
..
.IP 2.
Develop your functions in C in the module \fImcuser.c\fP.
Every function must be a double returning function with up to four double
parameters or a single string parameter.
.IP 3.
Add description entries in the files
.I mcftab.c
and
.I mcfunc.h
for every developed function.
Carefully read the comments in these files that explain how to describe
your function correctly.
.IP 4.
Simply compile and link the new
.B mc
with
.I make
.IP 5.
Test the new
.B mc
and the new functions with
.I ./mc
.IP 6.
Copy the new
.B mc
into the bin directory with
.br
.I cp $ARX/mc/src/mc $ARX/bin
.ig
.IP 8.
To add new functions restart the process with 5.
.LP
..
.ig
.BP LIMITATIONS
Since
.B mc
is created on Unixware,
the development of user-functions is also only supported on that system.
The produced binary should also run on other ELF/iBCS2-compatible
operating systems.
..
