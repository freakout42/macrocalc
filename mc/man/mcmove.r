.Id $Id: mcmove.r,v 1.6 1997/06/23 19:17:46 axel Exp $
.SH "Moving around"
The following keys move through the worksheet.
Moving is simply changing the
.I "current cell,"
which is displayed highlighted.
.DS
Function-Key Control-Key Description
\fI[Up]\fP         Ctrl+P      Move up one row
\fI[Down]\fP       Ctrl+N      Move down one row    
\fI[Left]\fP       Ctrl+B      Move left one column    
\fI[Right]\fP      Ctrl+F      Move right one column  
\fI[Tab]\fP        Ctrl+I      Move right one page 
\fI[Shift-Tab]\fP  Ctrl+G      Move left one page
\fI[Home]\fP       Ctrl+A      Move to cell A1
\fI[End]\fP        Ctrl+E      Move to last cell
\fI[Pgdn]\fP       Ctrl+V      Move down 20 rows
\fI[Pgup]\fP       Ctrl+U      Move up 20 rows  
.DE
.P
.BP NOTE
The Ctrl+Letter alternative keys are for terminals
without special function keys.
Most of them are the same as in the \fIEMACS\fP-Editor.
