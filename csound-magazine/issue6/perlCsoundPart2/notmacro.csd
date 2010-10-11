<CsoundSynthesizer>
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

instr 16
	a1 oscil 20000, 440, 1
	
	out a1
endin

</CsInstruments>

<CsScore>
f1 0 8192 10 1

i16 0 8

</CsScore>
</CsoundSynthesizer>

