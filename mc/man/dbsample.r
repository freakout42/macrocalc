.SH Examples
The greatest advantage of the plain ASCII \fB/rdb\fP-table-format is
the possibility of direct processing them with standard-UNIX-tools
like
.B sed
and \fBawk\fP.
.P
With
.B awk
the fields of the database can be referenced through the
\fI$1\fP to \fI$n\fP tokens (where \fIn\fP is the field-number).
.P
To make this work the statement line:
.DS
BEGIN {FS=OFS="\\t"}
.DE
.P
must be the first line in every \fBawk\fP-program,
cause the
.B <tab>
is the
.B /rdb
field-separator.
.P
The following shell command converts the
\fBDBase\fP-file \fIdbasefile\fP to \fB/rdb\fP-format,
processes it with
.B awk
selecting all rows containing lower numbers than 20 in the second column.
The output forms a new
\fBDBase\fP-file \fInewdbfile\fP.
.DS
db2rdb <dbasefile | awk 'BEGIN {FS=OFS="\\t"}
$2>20' | db2rdb -r >newdbfile
.DE
.P
The following shell command converts the
\fBDBase\fP-file \fIdbasefile\fP to \fB/rdb\fP-format,
processes it with
.B sed
substituting all occurences of the string "1410" with "credits".
The output forms a new
\fBDBase\fP-file \fInewdbfile\fP.
.DS
db2rdb <dbasefile | sed s/1410/credits/w |
db2rdb -r >newdbfile
.DE
