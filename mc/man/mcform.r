.Id $Id: mcform.r,v 1.7 1997/06/26 18:43:04 axel Exp $
.SH Formatting
The formatting of a range of cells is set with the
.I "/Range Format"
command.
.DS
Type       Example
---------- -------------
Fixed      123.12
Science    1.234e+06
Currency   $533.89
,          2,400.90
Percent    87.4%
Date       May 31 1991
/rdb-Date  970531
Time       12:45:08
General    123.1299
Hidden     
.DE
.LP
After Fixed or Science formats,
.B mc
prompts for the number of decimal places to be displayed.
.P
The default format for all new cells is \fIGeneral\fP,
which is a generic format.
The default format and decimal places can be changed with the
.Hr -symbolic Option_f "\fI-f\fP"
.Hr "\fI-f\fP"
and
.Hr -symbolic Option_d "\fI-d\fP options"
.Hr "\fI-d\fP options"
at startup.
When a new cell is entered and the cell above
(one row higher) has a non-default format,
the new cell is auto-formatted with that format.
This helps setting up tables of data.
.P
Cells with no contents cannot be formatted.
The cell value does not change when formatted.
Bold and Italic are not formats, but are
.Hr -symbolic Attributes attributes.
.Hr "attributes (see below)."
