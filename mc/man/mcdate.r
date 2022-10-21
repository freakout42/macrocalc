.Id $Id: mcdate.r,v 1.2 1997/06/28 15:25:45 axel Exp $
.SH Dates
Dates can be entered as formulas like
.DS
@DATE(97,4,21)
.DE
.P
As a shortcut for this function call,
dates can be directly entered either the american way
.DS
04/21/97
04/21/1997
.DE
.P
or the european way
.DS
21.04.97
21.04.1997
.DE
.P
In both cases
.B mc
will determine the input as a constant date value,
call the @DATE-function with the appropriate values
and automatically
.Hr -symbolic Formatting "format the cell"
.Hr "format the cell"
with the date format,
unless the cell was formatted before with a different format.
