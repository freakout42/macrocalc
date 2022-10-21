.SH Description
.B db2rdb
converts
\fBDbase\fP-files, produced by various popular PC-applications and,
of course the widely-used versions of the Dbase-database program itself,
to and from
.B /rdb
tables.
It also supports the Dbase-clone
\fBdBMAN V\fP,
which is available on various UNIX-platforms.
.P
Since
.B /rdb
tables use a flat ASCII-file format they can be processesed with many
UNIX-tools like
.B sed
and
.B awk
or with any editor like
.B vi
or \fBemacs\fP.
This file format is very interesting in the
.B UNIX
environment,
not only when
.B /rdb
is available.
The files can also be loaded into the
.B mc
spreadsheet with a simple
.DS
cell<db2rdb <dbasefile
.DE
.P
command.
.P
With the command
.DS
db2rdb [-option] <dbasefile >rdbfile
.DE
.P
almost every database produced with
.B Dbase
or other DOS-applications can be converted to a /rdb-file.
.P
With the command
.DS
db2rdb -r [-option] <rdbfile >dbasefile
.DE
.P
.B /rdb
files can be converted to a
.B Dbase
file.
.ig
.P
With the COHERENT
.B dos
command those Dbase-files can be copied to a MS-DOS filesystem or floppy disk,
and then loaded into any DOS application for further processing.
Most DOS applications support the Dbase-file-format.
.BP NOTE
Do not use the
.B a
ASCII option of the
\fBdos\fP-command
when transferring the Dbase-files!
Dbase-files are
.I no
transparent ASCII-files but are binary!
.P
.BP TIP
The COHERENT
.B dos
command (Version >=3.2) has a
.I pipe
option.
With the commands (they can be given on one line)
.DS
db2rdb -r </u/rick/clients.rdb |
dos rp /dev/dos /usr/mike/clients.dbf
.DE
.P
and
.DS
dos xp /dev/dos /usr/mike/clients.dbf |
db2rdb -r >/u/rick/clients.rdb
.DE
.P
copying to and from dos and converting to and from Dbase-file-format
can be done in one command and without intermediate files!
Another advantage is the independence of the directory structures on DOS
and COHERENT.
In this example the DOS file:
.DS
C:\\USR\\MIKE\\CLIENTS.DBF
.DE
.P
is converted to and from the COHERENT file
.DS
/u/rick/clients.rdb
.DE
.if \nV=2 \{\
.P
On COHERENT 4 and on SYSTEM V the commands
.B doscat
and
.B doscp
do similar things,
but with a different and more dos-like syntax. \}
..
.P
.B /rdb
is a trademark of RSW Inc,
Copyright (c) 1991 by Schaffer & Wright and available through
.br
RSW Inc., 131 Rathburn Way, Santa Cruz, CA 95062-1035.
.\"or from Mark Williams Company.
.P
.B "dBMAN V"
is Copyright (c) 1991 by VersaSoft Corporation and available through
.br
VersaSoft, P.O.Box 36078, San Jose, CA 95158.
.P
.B Dbase
is a trademark of dBase LLC.
.P
.B UNIX
is a trademark of UNIX Syst. Lab INC.
.ig
.P
.B mc
is Copyright (c) 1995 by /ARX cleanware.
..
