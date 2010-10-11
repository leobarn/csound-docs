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

i1 0 1 8.03
i1 1 1 8.07
i1 2 1 8.07
i1 3 1 8.09
i1 4 1 8.02
i1 5 1 8.07
i1 6 1 8.00
i1 7 1 8.09
i1 8 1 8.03
i1 9 1 8.03
e
</CsScore>

</CsoundSynthesizer>

