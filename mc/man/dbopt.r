.SH Options
.ig
.IP \\fB2\\fP
.IP \\fBII\\fP
Force Dbase II file format.
.IP \\fB3\\fP
.IP \\fBIII\\fP
Force Dbase III file format.
.IP \\fB4\\fP
.IP \\fBIV\\fP
Force Dbase IV file format.
.IP \\fBC\\fP\\fIl\\fP
Sets default character field length to \fIl\fP. Default is 20.
.IP \\fBD\\fP\\fIl\\fP
Sets default date field length to \fIl\fP. Default is 8.
.IP \\fBN\\fP\\fIl\\fP
Sets default numeric field length to \fIl\fP. Default is 9.
.IP \\fBF\\fP\\fIl\\fP
Sets default floating point field length to \fIl\fP. Default is 15.
.IP \\fBL\\fP\\fIl\\fP
Sets default logical field length to \fIl\fP. Default is 1.
..
.IP \\fBv\\fP
Verbose mode.
Prints \fBDbase\fP header information on standard error output.
.IP \\fBi\\fP
Image mode.
Does not read data. Only the header is read.
Is only usable together with verbose mode.
.IP \\fBk\\fP
Strukture mode.
Sends a Dbase structure definition table to stdout.
This table can be used to create a Dbase-table to create tables from
within Dbase or Clipper programs.
.IP \\fBa\\fP\\fIfile\\fP
Append mode.
Instead of copying to stdout,
append the output to the given \fIfile\fP.
Only implemented in reverse \fB/rdb\fP \-> \fBDbase\fP direction.
The field names of the input-file must match the output-file's field-names.
(See also -x external option).
.IP \\fBx\\fP\\fIfile\\fP
External mode.
Use the record-format found in \fIfile\fP.
Only implemented in reverse \fB/rdb\fP \-> \fBDbase\fP direction.
The field names of the input-file must match the given file's field-names.
(See also -a append option).
.IP \\fBt\\fP
No Trim mode.
Trim mode trims leading and trailing spaces of character fields away.
Since
.B Dbase
fills character-fields with spaces,
this mode is used by default and recommended not to disable with this option.
.IP \\fBu\\fP
Uppercase mode.
Sets all field names to uppercase characters.
.IP \\fBl\\fP
Lowercase mode.
Sets all field names to lowercase characters.
.IP \\fBc\\fP
Capitalize mode.
Sets all field names to first character in uppercase and
the remaining field-name to lowercase characters.
.IP \\fBf\\fP\\fIc\\fP
Field separator.
Use \fIc\fP (instead of TAB) as the input field separation character.
Is only effective in \fBDbase\fP \-> \fB/rdb\fP direction,
in reverse mode ignored.
.IP \\fBh\\fP
Headoff mode.
Does not output a second "dash-line".
Is only effective in \fBDbase\fP \-> \fB/rdb\fP direction,
in reverse mode ignored.
.IP \\fBr\\fP
Reverse mode.
Reverses the conversion mode to \fB/rdb\fP \-> \fBDbase\fP file format.
.IP \\fB0\\fP
Nullpadding mode.
Fills all \fBDbase\fP fields with the Null-character (Dec 0)
instead of Space (Dec 32).
Is not recommended.
Is only effective in \fBDbase\fP \-> \fB/rdb\fP direction,
in reverse mode ignored.
.IP \\fBb\\fP
Boolean mode.
Translates all \fBDbase\fP logical field TRUE indicators
"TtYyJjOo1" to "1" and all others to "0" in the \fB/rdb\fP table.
Is only effective in \fBDbase\fP \-> \fB/rdb\fP direction,
in reverse mode ignored.
.IP \\fBd\\fP
Date mode.
Translates \fBDbase\fP date fields in the format YYYYMMDD to the
\fB/rdb\fP date format YYMMDD.
But caution: Year 2000 is not far away!
Is only effective in \fBDbase\fP \-> \fB/rdb\fP direction,
in reverse mode ignored.
.IP \\fBe\\fP
Erased mode.
\fBDbase\fP does not physically erase rows,
but sign them with an asterix "*" in the first character.
The \fBPACK\fP-command really deletes the erased rows.
With erased mode, also the erased marked rows in the \fBDbase\fP-file
are written into the output \fB/rdb\fP-table.
Is only effective in \fBDbase\fP \-> \fB/rdb\fP direction,
in reverse mode ignored.
.IP \\fBg\\fP
Length mode.
By default the length of the dashed columns in the second line,
which separates the column names from the data in a \fB/rdb\fP-table,
is determined from the length of the column names.
In length mode, this length is determined from the fields width.
Is only effective in \fBDbase\fP \-> \fB/rdb\fP direction,
in reverse mode ignored.
.ig
.IP \\fBn\\fP\\fI\\fP
..
.IP \\fB+\\fP
Plus mode.
Build exactly DbaseIII+ not DbaseIII file (see Dbase file format).
Default is no Plus mode which is fully compatible with DbaseIII and ADS.
Is only effective in reverse \fB/rdb\fP \-> \fBDbase\fP direction,
in normal mode ignored.
.IP \\fBs\\fP\\fIyymmdd\\fP
Sets the last update date in the \fBDbase\fP-file to the given date.
Years with 00<=yy<=50 go into 20yy, others into 19yy.
Is only effective in reverse \fB/rdb\fP \-> \fBDbase\fP direction,
in normal mode ignored.
