#!/usr/bin/perl
use strict;

my $bar = "klaatu barada nikto";

if( $bar =~ m/barada/ )
{
	print '$bar contains the word "barada"' . "\n";
}

