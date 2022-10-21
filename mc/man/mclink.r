.Id $Id: mclink.r,v 1.4 1997/06/28 15:07:53 axel Exp $
.SH "Linking"
The worksheet currently in memory can be linked to worksheets on disk using the 
.I LINK
function of \fBmc\fP.
.DS
    LINK("filename",cell)

ex. LINK("/u/tom/sample.mc",A23)
.DE
.P
Since file linking is slow, linked cells are not recalculated.
However, if you enter a cell
.DS
ex. LINK("/u/tom/sample.mc",A23)+A12
.DE
.P
then the link is recalculated every time because it is a formula!
To avoid this, place only the link in one cell and put a formula in an
additional cell which refers to the link-cell.
Another advantage of this way is,
that the cell in the link is not adjusted
in column or row deletes or inserts or moves of the cell in the link.
To avoid this misbehavior,
it is also possible to use named cells and build the link with the name
instead of the absolute reference.
