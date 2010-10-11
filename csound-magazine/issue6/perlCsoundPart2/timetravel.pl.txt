#!/usr/bin/perl
use strict;

# Stores the score code to be processed
my $score = << "END";
f1 0 8192 10 1
t 0 120

i1 0   1   10000 8.00
i1 1   1   10000 7.09
i1 2   0.5 10000 7.07
i1 2.5 1.5 10000 7.04
i1 3   1   10000 7.02

i2 0   1   10000 8.00
i2 1   1   10000 7.09
i2 2   0.5 10000 7.07
i2 2.5 1.5 10000 7.04
i2 3   1   10000 7.02
END


# Process score
print timetravel( $score, 2, 4 );


# Subroutine for shifting start times of score events
sub timetravel
{
	my $score       = shift;  # The score code to process
	my $instr       = shift;  # The instrument to process
	my $timeshift   = shift;  # The amount of time shift
	my $line        = '';     # Holds the current line of code being processed
	my $value       = 0;      # Stores the new start time
	my $scoreReturn = '';     # Stores the processed score

	foreach $line ( split( /\n/, $score ) )
	{
		# Shift the time for instances of the specified instrument
		if( $line =~ /^i$instr\s+(\d+\.\d+|\d+)/ )
		{
			# Shift the time
			$value =  $1 + $timeshift;
			
			# Substitute the old start time with the new start time
			$line  =~ s/(^i\d+\s+)(\d+\.\d+|\d+)/$1$value/;
		}
		
		# Append to $scoreReturn
		$scoreReturn .= $line . "\n";	
	}

	return $scoreReturn;
}

