.Id $Id: mctext.r,v 1.3 1997/06/28 15:25:45 axel Exp $
.SH Text
Text can be entered in a cell simply by moving to the cell and typing in
the text.
With the
.I [Enter]
key,
.B mc
will put the text into the current cell and display its contents.
.Ha Justification ""
.BP JUSTIFICATION
Text cells can be displayed left or right justified in a cell depending on the
first character, the justification sign.
If the first character is an apostrophe (\fB\'\fP)
the text will be left justified.
If it is a quotation mark (\fB"\fP) right justified.
If no justification sign is specified at the start of the text,
.B mc
will add a left justification sign as default.
.Ha Repeating ""
.BP REPEATING
The special justification sign backslash (\fB\\\fP) will
cause the following character be repeated in the whole cell.
