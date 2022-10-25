.Id Id: mcsample.r,v 1.4 1997/04/11 14:53:12 axel
.SH Examples
There are example worksheets in the directory
\fI$ARX/exm\fP.
.ig
Go to a directory of your choice (i.e. \fI/usr/src/examples\fP)
and untar the file with:
.DS
tar xvf $ARX/lib/mcsample.tar
.DE
.P
Change into the new directory \fImcsample\fP.
..
This directory contains the following files:
.ST
.IP sort.mc 8
.br
Sorting a database.
Must be loaded with:
.DS
mc sort.mc
.DE
.IP how_sort 8
.br
Description for sorting a database within \fBmc\fP.
.IP unit.mc 8
.br
Unit conversion.
Must be loaded with:
.br
.DS
mc -u unit.mc
.DE
.IP purchase 8
.br
Shellscript-Example of \fB/rdb\fP integration with \fBmc\fP.
\fB/rdb\fP must be installed in $RDB
(usually is $RDB=/usr/rdb, set $RDB in /etc/profile).
The \fB/rdb\fP example project \fB/act\fP must be installed in $RBD/act.
.IP purchase.mc 8
.br
Worksheet for purchase.
.IP purchase.mcr 8
.br
Macro for purchase.
.ET
