.Id $Id: mcunit.r,v 1.5 1997/06/26 04:38:45 axel Exp $
.SH Units
.B mc
has a unique feature called automatic unit conversion.
If you use the
.Hr -symbolic Option_u "\fI-u\fP option"
.Hr "\fI-u\fP option"
at startup, then
.B $ARX/mcunits
acts as a background process to convert units.
You can assign a unit to each constant value in a formula by placing it in curly
brackets after the constant.
.DS
ex. 2.3{g} -- makes this 2.3 grams.
.DE
.P
This will put the string of the unit in the cell right next to the cell with
the constant.
.B mc
keeps track of all units you use in formulas and will refuse to calculate
senseless expressions.
When adding a unit (in curly brackets) at the end of a formula,
the calculated value is converted to that unit.
.DS
ex. A1: value:   12.00{m}
    A2: value:    1.00{sec}
    A3: value:   12.00{mile/hour}
    A4: formula: a1/a2       = 12.00 m/sec
    A5: formula: a3+a4       = 38.84 mile/hour
    A6: formula: a5+6{pound} = ERROR  Unit conformity
    A7: formula: a5{m/min}   = 1041.87 m/min
.DE
.P
The syntax of the units is the same as in \fBmcunits\fP.
To add units to the database see the documentation of \fBmcunits\fP.
.BP LIMITATIONS
Units are not implemented for the ^ (power) operator and for all
nonstatistical functions like SQRT!
