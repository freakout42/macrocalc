.Id mcopt.r
.SH Options
.Ha Option_l ""
.IP -l
Sets alternative screen layout with input line at the bottom.
.Ha Option_b ""
.IP -b
Suppress displaying the borders with column and row numbering.
.Ha Option_s ""
.IP -s
Suppress displaying current cell in line 1 of the screen.
This is especially interesting on slow terminals,
since moving around is much faster.
.Ha Option_m ""
.IP -m
Suppress displaying the menu, marked range and the recalculation message
for slow terminal lines.
.Ha Option_h ""
.IP -h
Suppress highlighting of the marked range for slow terminal lines.
.Ha Option_c ""
.IP -cw
Sets preset column width to \fBw\fP. Default is 11.
.Ha Option_d ""
.IP -dp
Sets preset decimal places to \fBp\fP. Default is 2.
.Ha Option_f ""
.IP -fF
Sets preset format to \fBF\fP. Default is General.
Possible formats are:
.ST
.IP F
Fixed
.IP S
Science
.IP C
Currency
.IP ,
Comma
.IP P
Percent
.IP D
Date
.IP T
Time
.IP G
General
.ET
.Ha Option_y ""
.IP -yc
Sets currency symbol to \fBc\fP. Default is dollar (\fB$\fP).
.Ha Option_a ""
.IP -a
No autocalc mode.
.Ha Option_r ""
.IP -r
Read-only mode.
Worksheets cannot be saved.
This is useful, when output is done by "cell>command" statements.
See shell-script
.Hr -symbolic mcrdb \fBmcrdb\fP
.Hr \fBmcrdb\fP 
for an example.
.Ha Option_o ""
.IP -omacrofile\fP
Auto-execute macro found in \fBmacrofile\fP after loading the worksheet.
This option also suppresses the interactive startup message.
The \fBmacrofile\fP "-" only suppresses the interactive startup message.
.Ha Option_n ""
.IP -nmacrocommands\fP
Auto-execute macros in \fBmacrocommands\fP after loading the worksheet.
This option also suppresses the interactive startup message.
.Ha Option_x ""
.IP -x
Sets command autoexec mode.
All
.Hr -symbolic Piping cell-commands
.Hr cell-commands
"cell<command" found in a new loaded worksheet and
"cell>command" found in a saved worksheet
will be executed automatically.
.Ha Option_u ""
.IP -u
Starts
.B $ARX/mcunits
as a background process for
.Hr -symbolic Units "unit conversion"
.Hr "unit conversion"
calculation.
The "units" function of
.B mc
can only work with this option!
Startup is slower with this option,
so only use it if unit conversion is needed.
.Ha Option_p ""
.IP -p
Pipes the standard input (a worksheet file) into the standard
output (\fB[nt]roff\fP source format) without interactive action.
This \fBmust\fP be the last option when present!
This is especially interesting because
.B mc
uses a fully transparent, editable
.Hr -symbolic "File format" "ASCII file format,"
.Hr "ASCII file format,"
which is compatible with the
.B /rdb
database system.
The worksheet file can easily be edited or manipulated with almost any UNIX
command such as
.B sed
or \fBawk\fP.
You can also use \fB/rdb\fP's
100-plus powerful database commands.
.Ha Option_q ""
.IP -q
Pipes the standard input (a worksheet file) into a
"comma-separated-values" file
without interactive action.
This \fBmust\fP be the last option when present!
.Ha Option_v ""
.IP -v
Prints version information.
.P
Options are also taken from the environment variable "MACROCALC".
