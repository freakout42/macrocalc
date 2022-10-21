.Id $Id: mcprint.r,v 1.4 1997/06/28 15:07:53 axel Exp $
.SH Printing
.B mc
uses
.B [nt]roff
to do the hard work of text formatting.
The marked range is piped to the shell procedure
.I $ARX/lib/mcprint
with the command
.I /Print
This procedure passes its input to
.B nroff
as the default.
.I $ARX/lib/mcprint
can be modified
to the user's needs, for example to add a standard header to every printed
worksheet or to change
.B nroff
to
.B troff
and
.B lpr
to \fBhpr\fP.
.LP
.B [nt]roff
commands can be used by putting them directly into the
left side cells of the printing range.
For example, the menu command
.I "/Worksheet Page"
simply adds a ".bp"
.I "break page"
command into the worksheet.
.if \nV>4 \{\
.BP NOTE
Due to some rip-off policy of some UNIX-vendors,
to ask for extra money for the UNIX text-formatting-utilities,
.B nroff
is not available on all UNIX-sites.
For those sites
.\", which want to use \fBmc\fP,
.\"but have no text formatting subsystem,
the mini-nroff-implementation
.B nro
will be fine to do the job of printing with \fBmc\fP.
.B nro
comes with \fBmc\fP.
See the manual pages for
.B nro
for detailed information.
\}
