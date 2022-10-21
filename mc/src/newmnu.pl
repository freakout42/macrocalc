#!/usr/bin/perl

($,, $\) = ("\t", "\n");

open(M, "<mcmenus.rdb") || die;
$a = 'fr';
$w = 'pl';
while(<M>) {
	chop;
	($n, $l, $m) = split($,);
	print ($n, $l, $m);
	if ($l eq $a) {
		$_ = <>;
		chop;
		print ($n, $w, $_);
		}
	}

exit;
