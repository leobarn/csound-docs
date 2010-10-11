#!/usr/bin/perl
use strict;

# Open a file for writing

open( CSD, "> array.csd" ) || die "Can't write file array.csd: $!";




# Write to the file

print CSD << 'END';
<CsoundSynthesizer>

<CsInstruments>

sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

instr 1
	ipch = cpspch( p4 )
	
	aosc oscil 30000, ipch, 1, -1
	out  aosc
endin

</CsInstruments>

<CsScore>

f1 0 8192 10 1

END




my $startTime;
my $index;
my $pitch;

my @pitchTable = ( 8.00, 8.02, 8.03, 8.07, 8.09 );

foreach $startTime ( 0 .. 9 )
{
	$index = int( rand() * scalar( @pitchTable ) );
	$pitch = $pitchTable[ $index ];
	
	printf CSD ( "i1 %d 1 %.2f\n", $startTime, $pitch ); 	
}




# Finish writing csound code to the file

print CSD << 'END';
e
</CsScore>

</CsoundSynthesizer>

END




# close the file
close CSD;

