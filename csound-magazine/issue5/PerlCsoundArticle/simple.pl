#!/usr/bin/perl

open( CSD, "> simple.csd" ) || die "Can't write file simple.csd: $!";

print CSD << "END";

<CsoundSynthesizer>

<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

instr 1
	aosc oscil 30000, 440, 1
	out  aosc
endin

</CsInstruments>

<CsScore>
f1 0 8192 10 1

i1 0 6
e

</CsScore>

</CsoundSynthesizer>

END

close CSD;



