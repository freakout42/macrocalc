#!/usr/bin/perl -n
BEGIN {
print <<"EOF";
Cell\tAttrib\tFormat\tValue\tText
-------\t-\t----\t-----------------------\t--------------------------------------------------------------------------------
A1\t0\t0\t+1.0000000000000000e+00\twindowdef
EOF
}
chop; chop;
if (/^C;X(\d+)$/) {
  $x = $1-45;
}
if (/^C;(X(\d+))?Y(\d+);K([\d.-]*)$/) {
  if ($2+0>0) { $x = $2-45; }
  $coor = sprintf("%s%d", chr(ord('A')-1+$x), $3+0);
  printf "%s\t2\t2\t%+23.16e\t%s\n", $coor, $4+0.0, $4;
}
if (/^C;(X(\d+))?Y(\d+);K"(.*)"$/) {
  if ($2+0>0) { $x = $2-45; }
  $coor = sprintf("%s%d", chr(ord('A')-1+$x), $3+0);
  printf "%s\t1\t127\t%+23.16e\t'%s\n", $coor, 0.0, $4;
}
