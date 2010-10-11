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

i1 0 1 50.000000 3
i1 1 1 100.000000 3
i1 2 1 200.000000 2
i1 3 1 400.000000 2
i1 4 1 800.000000 1
i1 5 1 1600.000000 1
e
</CsScore>

</CsoundSynthesizer>

