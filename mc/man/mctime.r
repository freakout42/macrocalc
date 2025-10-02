.Id $Id: mctime.r,v 1.3 1999/10/14 18:44:53 axel Exp $
.SH "Date/Time functions"
.IP DATE(YY,MM,DD) 3
Serial number of days from Jan 1 1900 to Year = YY,
Month = MM, Day = DD.
YY from 0 to 37 will be transformed automatically to 2000 to 2037.
To get a date before 1938 enter 19YY into the year value.
(Note: Cell need to be in date format to display dates)
.br
ex. DATE(87,5,20) = 31917
.IP TODAY() 3
Returns the serial number of days to now.
.IP TIME(HR,MIN,SEC)
Fraction of day corresponding to HR:MIN:SEC
.IP DAY(X) 3
The Day in X.
.br
ex. DAY(31917) = 20.
.IP HOUR(X) 3
The Hours in the fractional part of X
.IP MINUTE(X) 3
The Minutes in the fractional part of X
.IP MONTH(X) 3
The Month in X.MONTH(31917) = 5 (May)
.IP SECOND(X) 3
The Seconds in the fractional part of X
.IP YEAR(X) 3
The Year in X.
.br
ex. YEAR(31917) = 87
