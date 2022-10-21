.Id $Id: mcfile.r,v 1.4 1997/04/11 14:53:12 axel Exp $
.SH "File format"
.B mc
uses a fully transparent and easy-to-understand file format to store worksheets.
It is organized like a table and is compatible with
.B /rdb
table database format.
The columns of the table are separated with \fB<tab>\fP,
the rows with \fB<newline>\fP.
At the top of each column is a column name and a dashed row to separate the 
names from the data.
The columns are:
.ST
.IP Cell 16
Cell address
.IP Attrib 16
Cell attribute code
.IP Format 16
Cell format code
.IP Value 16
Cell value
.IP Text 16
Cell string with justification sign
.ET
.P
.BP NOTE
Files produced by 
.B mc
V1.x must be transformed by the utility
.B mc2conv
with the command:
.DS
mc2conv \fI<mc1file >mc2file\fP
.DE
.P
This command transforms the old file into a form that can be read by
.B mc
V2.x and its successors.
It is also a example of processing an 
.B mc
file with \fBawk\fP.
