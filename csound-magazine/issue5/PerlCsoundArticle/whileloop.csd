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

i1 0.0000 0.5 894.709577
i1 0.4779 0.5 109.692073
i1 0.8381 0.5 289.274825
i1 0.9852 0.5 216.057307
i1 1.3320 0.5 280.710331
i1 1.6873 0.5 321.337319
i1 1.9777 0.5 115.167399
i1 2.2663 0.5 623.591924
i1 2.8348 0.5 918.851922
i1 3.7560 0.5 336.182481
e
</CsScore>

</CsoundSynthesizer>

