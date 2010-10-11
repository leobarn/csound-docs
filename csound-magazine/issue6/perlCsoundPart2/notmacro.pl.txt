#!/usr/bin/perl
use strict;

my $sine       = 1;
my $puretone   = 16;
my $fourfourty = 440;

print << "END";
<CsoundSynthesizer>
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

instr $puretone
	a1 oscil 20000, $fourfourty, $sine
	
	out a1
endin

</CsInstruments>

<CsScore>
f$sine 0 8192 10 1

i$puretone 0 8

</CsScore>
</CsoundSynthesizer>

END

