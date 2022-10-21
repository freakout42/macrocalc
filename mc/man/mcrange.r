.Id $Id: mcrange.r,v 1.4 1997/06/28 15:25:45 axel Exp $
.SH Range
A range is a rectangular sub-section of the worksheet which:
.LP
.PD 0
.LP
- is defined by two cells at two corners of the range
.LP
- encompasses all of the cells inside this sub-section.
.PD 1
.LP
Several commands act upon a range of cells.
The
.I [Mark]
key specifies a corner of the range.
There is always a marked range shown at the top of the worksheet.
The marked range is also displayed highlighted
(unless
.Hr -symbolic Option_h "option \fI-h\fP"
.Hr "option \fI-h\fP"
was given at startup).
Inside the marked range the current cell will be not highlighted.
Before a command that acts on a range can be used,
the correct range must be marked.
.LP
A range can be named with the
.I "/Range Name"
command.
This name can be used in
.Hr -symbolic Formulas formulas.
.Hr formulas.
A valid range name consists of a starting character and
at least one more alphanumeric character or the special characters
backslash (\fB\\\fP), underscore (\fB_\fP) and dollar (\fB$\fP).
When a named range is used for a cell,
the upper left cell of the range is used.
Named ranges are adjusted when moved or
after inserting or deleting columns or rows.
