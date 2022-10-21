.Id $Id: mcover.r,v 1.7 2004/03/05 19:03:52 axel Exp $
.SH "General Overview"
A worksheet is a table built from columns and rows.
Every point of intersection between a column and a row is called a cell.
A cell can be filled with text, constant values or formulas.
.LP
.B mc
supports:
.PD 0
.LP
- 702 columns named A..Z and AA..ZZ
.LP
- 99999 rows named 1..99999.
.PD 1
.LP
Every cell has a unique cell address with column and row name ranging from
A1..ZZ99999.
.LP
Since a monitor can only display 24 rows and 80 characters at any given time,
only a small
part of the worksheet is displayed.
One cell, the
.I "current cell,"
is highlighted and its contents is displayed in line 1 of the screen
(unless
.Hr -symbolic Option_s "option \fI-s\fP"
.Hr "option \fI-s\fP"
was given at startup).
.Hr -symbolic "Moving around" "Movement around"
.Hr "Movement around"
through the worksheet is possible with the cursor keys.
Line 2 of the screen is the edit line where cell input is displayed.
Line 3 is the status and message line where information is displayed.
The rest of the screen shows the worksheet.
