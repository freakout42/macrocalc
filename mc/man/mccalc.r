.Id $Id: mccalc.r,v 1.2 1997/06/25 19:18:21 axel Exp $
.SH Recalculation
To save memory, formulas are stored as text without any additional
parsing information: therefore recalculation is slow.
For large worksheets, autocalc mode should be turned off by using
the menu command
.I "/Worksheet Autocalc"
or the
.Hr -symbolic Option_a "option \fI-a\fP."
.Hr "option \fI-a\fP."
Manual recalculation is started with
.I [Recalc]
after finishing with entries.
