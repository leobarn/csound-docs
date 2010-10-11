#!/usr/bin/perl
use strict;

# Open a file for writing

open( CSD, "> ifelse.csd" ) || die "Can't write file ifelse.csd: $!";




# Write to the file

print CSD << 'END';
<CsoundSynthesizer>

<CsInstruments>

sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

instr 1
	idur  = p3
	ifreq = p4
	iwave = p5
	
	aosc oscil 30000, ifreq, iwave, -1
	out aosc
endin

</CsInstruments>

<CsScore>

f1 0 8192 10 1
f2 0 8192 -7 -1 4096 1 4096 -1
f3 0 8192 -7 -1 8192 1

END




# Generate score events with branching waveshape selector

my $frequency = 50;
my $startTime = 0;
my $waveshape;

while( $frequency <= 1600 )
{

	if( $frequency >= 800 )
	{
		# Sine wave
		$waveshape = 1;
    }
	elsif ( ( $frequency >= 200 ) && ( $frequency < 1000 ) )
	{
		# Triangle Wave
		$waveshape = 2;
	}
	else
	{
		# Saw wave
		$waveshape = 3;
	}
    
	printf CSD ( "i1 %d 1 %.6f %d\n", $startTime, $frequency, $waveshape );

	$startTime = $startTime + 1;
	$frequency = $frequency * 2;
}




# Finish writing csound code to the file

print CSD << 'END';
e
</CsScore>

</CsoundSynthesizer>

END




# Close the file

close CSD;



