#!/usr/bin/perl
use strict;

# Declare Constants
use constant MONOCENTER  => 0;
use constant ALTERNATING => 1;
use constant RANDOM      => 2;


# Declare Scalars and Arrays
my $instr        = 100;          # The instrument number
my $nBands       = 10;           # Number of bands
my $carMin       = 100;          # Minimum carrier frequency
my $carMax       = 8000;         # Maximum carrier frequency
my $modMin       = 200;          # Minimum modulator frequency
my $modMax       = 16000;        # Maximum modulator frequency
my $nChannels    = 2;            # 1 = mono, 2 = stereo
my $stereoMethod = ALTERNATING;  # MONOCENTER, ALTERNATING or RANDOM


my @stereoMethods;
my @nChannels;
my $left;
my $right;
my $random;
my $n;
my $freq;
my $coef;


# Begin Generating Instrument
print << "END";
instr $instr
	idur  = p3
	igain = p4
	ibw   = p5
	
	acar zar \$ZCARRIER
	amod zar \$ZMODULATOR
END


# Carrier Bandpass Array
print "\n\n\t; Process carrier with bandpass array\n\n"; 

$freq = $carMin;
$coef = ( $carMax / $carMin ) ** ( 1 / ( $nBands - 1 ) );  # Expon Interp 

foreach $n ( 0 .. ( $nBands - 1 ) )
{
	printf ( "\tabpc%d butterbp acar, %.2f, %.2f * ibw\n", $n, $freq, $freq );  
	$freq = $freq * $coef;
}


# Modulator Bandpass Array
print "\n\n\t; Process modulator with bandpass array\n\n"; 

$freq = $modMin;
$coef = ( $modMax / $modMin ) ** ( 1 / ( $nBands - 1 ) ); 

foreach $n ( 0 .. ( $nBands - 1 ) )
{
	printf ( "\tabpm%d butterbp amod, %.2f, %.2f * ibw\n", $n, $freq, $freq );  
	$freq = $freq * $coef;
}

																																
# Balance
print "\n\n\t; Modulate the carrier with the balance array\n\n"; 

foreach $n ( 0 .. ( $nBands - 1 ) )
{
	printf ( "\tav%d balance abpc%d, abpm%d\n", $n, $n, $n );  
}


# Mix Balance Array
print "\n\n\t; Mix and out\n\n"; 

if( $nChannels == 1 )
{
	print "\tamix = av0";

	foreach $n ( 1 .. ( $nBands - 1 ) )
	{
		printf ( "+av%d", $n );	
	}

	print "\n\tamix = amix * igain\n\n\tout amix\n";
}
elsif( $stereoMethod == MONOCENTER )
{
	print "\tamix = av0";

	foreach $n ( 1 .. ( $nBands - 1 ) )
	{
		printf ( " + av%d", $n );	
	}

	print "\n\tamix = amix * igain\n\n\touts amix, amix\n";

}
elsif( $stereoMethod == ALTERNATING )
{
	foreach $n ( 2 .. ( $nBands - 1 ) )
	{
		if( $n % 2 == 0 )
		{
			$left .= sprintf ( "+av%d", $n );
		}
		else
		{
			$right .= sprintf ( "+av%d", $n );
		}
	}
	
	print "\taleft  = (av0$left)*igain\n\taright = (av1$right)*igain\n";
	print "\n\touts aleft, aright\n";
	
}
elsif( $stereoMethod == RANDOM )
{
	foreach $n ( 0 .. ( $nBands - 1 ) )
	{
		$random = rand( );
		$left  .= sprintf ( "av%d*%.2f", $n, sqrt( $random ) );
		$right .= sprintf ( "av%d*%.2f", $n, sqrt( 1 - $random ) );

		unless( $n == ( $nBands - 1 ) )
		{
			$left  .= "+";
			$right .= "+";
		}
	}
	
	print "\taleft  = $left\n\taright = $right\n\n\touts aleft * igain, aright * igain\n";
}

print "endin\n";

