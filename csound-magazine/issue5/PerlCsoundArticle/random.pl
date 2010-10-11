#!/usr/bin/perl
use strict;

# Open a file for writing

open( CSD, "> random.csd" ) || die "Can't write file random.csd: $!";




# Write to the file

print CSD << 'END';
<CsoundSynthesizer>

<CsInstruments>

sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

instr 1
	aosc oscil 30000, p4, 1, -1
	out aosc
endin

</CsInstruments>

<CsScore>

f1 0 8192 10 1

END




# Generate a score event with a random frequency

my $frequency = rand() * 900 + 100;

print CSD "i1 0 4 $frequency\n";




# Finish writing csound code to the file

print CSD << 'END';
e
</CsScore>
</CsoundSynthesizer>
END




# close the file

close CSD;

