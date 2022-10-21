#!/usr/bin/perl

($,, $\) = ("\t", "\n");

open(M, "<mcmessag.rdb") || die;
$a = 'fr';
$w = 'pl';
$i = 0;
$x = <>;
$x = <>;
while(<M>) {
	chop;
	($n, $l, $m) = split($,);
	print ($n, $l, $m);
	if ($l eq $a) {
		$_ = <>;
		chop;
		($j, $m) = split($,);
		die if $i++ != $j;
		$m = '{NULL}' if $m eq '';
		print ($n, $w, $m);
		}
	}

exit;
