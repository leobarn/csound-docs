#!/usr/bin/perl
use strict;


# Open a file for writing

open( CSD, "> whileloop.csd" ) || die "Can't write file whileloop.csd: $!";




# Write to the file

print CSD << 'END';
<CsoundSynthesizer>

<CsInstruments>

sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

instr 1
	kenv linseg 0, p3 * 0.5, 1, p3 * 0.5, 0
	aosc oscil 15000 * kenv, p4, 1, -1
	out aosc
endin

</CsInstruments>

<CsScore>
f1 0 8192 10 1

END




my $startTime = 0;
my $frequency;

while( $startTime < 4 )
{
	$frequency = rand() * 900 + 100;

	printf CSD ( "i1 %.4f 0.5 %.6f\n", $startTime, $frequency );

	$startTime = $startTime + rand() * 0.825 + 0.125;	
}




# Finish writing csound code to the file

print CSD << 'END';
e
</CsScore>

</CsoundSynthesizer>

END



# Close the file

close CSD;

