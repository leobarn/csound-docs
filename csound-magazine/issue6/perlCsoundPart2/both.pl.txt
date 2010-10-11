#!/usr/bin/perl
use strict;

my $freq = sprintf( "%.4f", rand() * 440 + 440 );

print << "END"
# define sine #1#

instr 1
    a1 oscil 10000, $freq, \$sine

    out a1
endin
END
