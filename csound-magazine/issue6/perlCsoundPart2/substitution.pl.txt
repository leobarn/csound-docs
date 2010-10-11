#!/usr/bin/perl
use strict;

my $foo = "Have you upgraded to Csound version 5.04, yet?\n";

$foo =~ s/5\.04/5.05/;

print $foo;

