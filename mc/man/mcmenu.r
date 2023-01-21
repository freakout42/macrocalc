.Id $Id: mcmenu.r,v 1.7 2004/07/04 19:04:45 axel Exp $
.SH "Menu"
With the
.I [Menu]
key,
.B mc
enters the command menu.
Within the menu there are powerful commands to alter the worksheet.
.\"The menu is hierarchical organized.
.\"Some menus have submenus.
To choose a menu item move to it with the cursor-keys until
it is highlighted and press the [ENTER] key,
or press the highlighted character of the item.
.IP \fIWorksheet\fP 8
.RS
.PD 0
.IP \fIInsert\fP 9
.RS
.PD 0
.IP \fIColumn\fP 9
Inserts columns at current position.
.IP \fIRow\fP 9
Inserts rows at current position.
.PD 0
.RE
.IP \fIDelete\fP 9
.RS
.PD 0
.IP \fIColumn\fP 9
Deletes current column.
.IP \fIRow\fP 9
Deletes current row.
.PD 0
.RE
.IP \fIColumn\fP 9
Sets current column width.
.IP \fIAutocalc\fP 9
Turns
.Hr -symbolic Recalculation "auto calculation"
.Hr "auto calculation"
mode on and off.
.IP \fIFormula\fP 9
Turns formula display mode on and off.
.IP \fIErase\fP 9
Erases entire worksheet.
.IP \fITitels\fP 9
.RS
.PD 0
.IP \fIHorizontal\fP 12
Freezes the rows above the current cell on the screen as a title.
.IP \fIClear\fP 12
Clears the title.
.PD 0
.RE
.IP \fIPage\fP 9
Inserts a page break.
.PD 1
.RE
.IP \fIRange\fP 8
.RS
.PD 0
.IP \fIFormat\fP 9
.Hr -symbolic Formatting Formats
.Hr Formats
a range.
.IP \fIAttrib\fP 9
.Hr -symbolic Display Attributes
.Hr Attributes
Set display attributes for a range.
.IP \fIColor\fP 9
.Hr -symbolic Colors
.Hr Colors
Set display color for a range.
.IP \fIErase\fP 9
Erases a range.
.IP \fIName\fP 9
.RS
.PD 0
.IP \fICreate\fP 9
Creates a named
.Hr -symbolic Range range.
.Hr range.
.IP \fIDelete\fP 9
Deletes a named
.Hr -symbolic Range range.
.Hr range.
.RE
.IP \fIProt\fP 9
Protects a
.Hr -symbolic Range range.
.Hr range.
.IP \fIUnprot\fP 9
Unprotects a
.Hr -symbolic Range range.
.Hr range.
.IP \fIValue\fP 9
Values a
.Hr -symbolic Range range.
.Hr range.
All
.Hr -symbolic Formulas Formulas
.Hr Formulas
in the range are substituted by there actual value.
.PD 1
.RE
.Ha Copy ""
.IP \fICopy\fP 8
Copies marked range to current position.
.Ha Move ""
.IP \fIMove\fP 8
Moves marked
.Hr -symbolic Range range
.Hr range
to current position.
.IP \fIFile\fP 8
.RS
.PD 0
.IP \fISave\fP 9
Saves current worksheet to file.
.IP \fIPart\fP 9
Saves only marked range of current worksheet to file.
.IP \fIRetrieve\fP 9
Retrieves file and replace current worksheet.
.IP \fIMerge\fP 9
Merges file with current worksheet.
.PD 1
.RE
.IP \fIPrint\fP 8
.Hr -symbolic Printing Prints
.Hr Prints
marked range of current worksheet.
.IP \fIDisplay\fP 8
Redisplays in case of a corrupted screen.
.IP \fISys\fP 8
Opens a subshell.
.IP \fIQuit\fP 8
Leaves \fBmc\fP.
