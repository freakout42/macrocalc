.Id $Id: mcattr.r,v 1.2 1997/04/11 14:03:26 axel Exp $
.SH Attributes
The  appearance  of  a cell on printed output can be set with the
.I "/Range Attrib"
command.
Every cell can have a
.B Bold
or
.I Italic
attribute.
.B mc
puts
.B nroff
font controlling commands in the output, which is piped through \fB[nt]roff\fP.
.if \nV>4 \{\
On a terminal with enhanced capabilities bold cells are
displayed highlighted and italic cell are displayed underlined. \}
