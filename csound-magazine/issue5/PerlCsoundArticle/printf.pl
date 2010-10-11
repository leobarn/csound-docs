#!/usr/bin/perl
use strict;

# Open a file for writing

open( CSD, "> printf.csd" ) || die "Can't write file printf.csd: $!";




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




# Format the code using printf

my $frequency = rand() * 900 + 100;

printf CSD ( "i%d %.4f %.4f %.6f\n", 1, 0, 4, $frequency );



# Finish writing csound code to the file

print CSD << 'END';
e
</CsScore>
</CsoundSynthesizer>
END




# Close the file

close CSD;

