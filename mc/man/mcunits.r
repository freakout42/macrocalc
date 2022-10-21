.Id $Id: mcunits.r,v 1.4 1995/04/30 13:11:33 axel Rel $
.TH mcunits 1 \*(Dt "/ARX cleanware" "mcunits"
.UC 3
.SH Name
.B mcunits
- Command
.br
Convert measurements.
.SH Synopsis
mcunits [ -u ]
.SH Description
.B mcunits
is an interactive program that tells you how
to convert one unit of measurement into another.
It prompts you for two quantities with the same dimension
(e.g., two measurements of weight, or two of size).
It first prints the prompt ``You have:''
to ask for the unit you wish to convert from,
and then prints the prompt ``You want:''
for the unit you wish to convert to.
.SH Example
The following example returns the formula for convert fortnights into days:
.DS
You have: fortnight
You want: days
* 14
/ 0.071428
.DE
.SH "Fundamental Units"
The following fundamental units are recognized:
meter, gram, second, coulomb, radian, bit, unitedstatesdollar,
sheet, candle, kelvin, and copperpiece (shillings and pence).
.SH Quantities
A quantity consists of an optional number (default, 1)
and a dimension (default, none).
.SH Numbers
Numbers are floating point with optional sign, decimal part and exponent.
.SH Dimensions
Dimensions may be specified by fundamental or derived units,
with optional orders.
.SH Evaluation
A quantity is evaluated left to right:
a factor preceded by a `/' is a divisor,
otherwise it is a multiplier.
For example, the earth's gravitational acceleration
may be entered as any of the following:
.DS
9.8e+0 m+1 sec-2
32 ft/sec/sec
32 ft/sec+2
.DE
.SH British
British equivalents of US units are prefixed with br, e.g., brpint.
Other units include c (speed of light),
G (gravitational constant),
R (gas-law constant),
phi (golden ratio) % (1/100),
k (1,024), and buck (United States dollar).
.SH "See Also"
bc, conv, units
.SH Files
.I "$ARX/lib/mcunits.rdb"
is the ASCII file that contains conversion tables.
The binary file
.I "$ARX/lib/binunits"
may be recreated by using the -u option.
.SH Diagnostics
If the ASCII file
.I "$ARX/lib/mcunits.rdb"
has changed more recently than the binary file
.I "$ARX/lib/binunits,"
units prints a message and regenerates the binary file before it continues;
this can take up to a few minutes, depending upon the speed of your system.
.LP
The error message ``conformability'' means that the quantities are not
dimensionally compatible, e.g., m/sec and psi.
.B mcunits
prints each quantity and its dimensions in fundamental units.
.SH Notes
.B mcunits
only does multiplicative scale changes;
thus it can convert Kelvin to Rankine, but not Celsius to Fahrenheit.
.LP
There are the inevitable name collisions: g for gram versus gee for Earth's
gravitational acceleration, exp for the base of natural logarithms versus e
for the charge of an electron, ms for (plural) meters versus millisecond,
and of course batman for the Persian measure of weight rather than the
Turkish.
.so ../../arx/man/author.r
