.SH Terminal
Since this product is based on the
.B curses
library,
the program run on the console as well as on terminals.
.LP
The following table list the keys and their corresponding entries in
.I /etc/termcap
or
.I /usr/lib/terminfo/?/*.
COHERENT-3.x versions work with
.B termcap,
all others with
.B terminfo.
.LP
The table also lists the used screen attributes.
If you want color output you can change the entries to sequences
for colors colors on your terminal.
.LP
The keys and screen attributes can only work
if their correct entries are made in the terminals description,
which is identified by the environment variable
.B TERM.
.DS
Key      termcap terminfo Comment
-------- ------- -------- ------------------------------
UP       ku      kcuu1    Move one line/item up.
DOWN     kd      kcud1    Move one line/item down.
LEFT     kl      kcub1    Move one character/item left.
RIGHT    kr      kc       Move one character/item right.
PGUP     Kp      kpp      Move one screen up.
PGDOWN   KP      knp      Move one screen down.
BACKTAB  bt      cbt      Move one screen left.
HOME     kh      kc       Move to begin.
END      KE      kll      Move to end.
INSERT   Ku      kich1    Insert mode.
DELETE   Kd      kdch1    Delete item.
F1       k1      kf1      Function 1 = Help.
F2       k2      kf2      Function 2.
F3       k3      kf3      Function 3.
F4       k4      kf4      Function 4.
F5       k5      kf5      Function 5.
F6       k6      kf6      Function 6.
F7       k7      kf7      Function 7.
F8       k8      kf8      Function 8.
F9       k9      kf9      Function 9.
F10      k0      kf0      Function 10 = Exit.

Attribute termcap terminfo Comment
--------- ------- -------- ------------------------------
Blink     mb      blink    Blink mode on
Bold      md      bold     Bold mode on
Reverse   so      rev      Reverse mode on
Standout  so      smso     Standout mode on
Underlin  us      smul     Underline mode on
Normal    me      sgr0     Normal mode on
Standard  se      rmse     Standout mode off
Standard  se      rmso     Standout mode off
Standard  ue      rmul     Underline mode off
.DE
.P
.BP CAUTION
Since most terminal keys start with the <ESC>-character (octal 033),
the ESCAPE-Keys itself needs one second to be decoded!
