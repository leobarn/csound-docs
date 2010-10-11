#!/usr/bin/perl
use strict;

# Open a file for writing

open( CSD, "> sub.csd" ) || die "Can't write file sub.csd: $!";




# Write to the file

print CSD << 'END';
<CsoundSynthesizer>

<CsInstruments>

sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

instr 1
	idur   = p3
	irise  = p6
	idecay = 1 - irise
	
	kenv linseg 0, irise * idur , 1, idecay * idur, 0
	aosc oscil p4 * kenv, p5, 1, -1
	out aosc
endin

</CsInstruments>

<CsScore>

f1 0 8192 10 1

END




# Generate Score Events with sineCluster()

print CSD sineCluster( 1,  0.00,  8.00,   8, 30000, 100,  300, 0.500 );
print CSD sineCluster( 1,  4.00,  8.00,  16, 20000, 200,  600, 0.125 );
print CSD sineCluster( 1,  8.00, 12.00,  32, 25000, 300, 1200, 0.350 );
print CSD sineCluster( 1, 12.00, 14.00,  64, 30000, 100, 2400, 0.900 );
print CSD sineCluster( 1, 24.00,  0.25,   2, 25000, 100, 2400, 0.100 );
print CSD sineCluster( 1, 24.25,  0.25,   4, 25000, 100, 1200, 0.200 );
print CSD sineCluster( 1, 24.50,  0.25,   8, 25000, 100,  800, 0.400 );
print CSD sineCluster( 1, 24.75,  0.25,  16, 25000, 100,  400, 0.800 );
print CSD sineCluster( 1, 25.00,  0.25,  32, 25000, 100,  800, 0.400 );
print CSD sineCluster( 1, 25.25,  0.25,  64, 25000, 100, 1600, 0.200 );
print CSD sineCluster( 1, 25.50,  0.25, 128, 25000, 100, 1200, 0.100 );
print CSD sineCluster( 1, 25.75,  0.25, 256, 25000, 100, 2400, 0.010 );




# Finish writing csound code to the file

print CSD << 'END';
e
</CsScore>

</CsoundSynthesizer>

END




# Close the file

close CSD;




# Random additive synthesis pad generator

sub sineCluster
{
	my $instr    = shift;  # instrument number
	my $start    = shift;  # start time of score event
	my $duration = shift;  # duration of score event
	my $nVoices  = shift;  # the number of voices to generate
	my $amp      = shift;  # the amplitude of the pad
	my $minFreq  = shift;  # minimum possible frequency for pad voices
	my $maxFreq  = shift;  # maximum possible frequency for pad voices
	my $rise     = shift;  # attack time of the pad
	my $score    = '';     # stores the score events
	

	for( 1 .. $nVoices )
	{
		$score .= sprintf( "i%d %.4f %.4f %.4f %.4f %.4f\n",
		                   $instr,
		                   $start,
		                   $duration,
		                   $amp / $nVoices,
		                   rand() * ( $maxFreq - $minFreq ) + $minFreq,
		                   $rise );
				
	}
	
	return $score;
}




