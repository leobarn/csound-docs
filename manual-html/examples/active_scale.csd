<CsoundSynthesizer>
<CsOptions>
</CsOptions>
<CsInstruments>

sr= 44100
ksmps = 64
0dbfs = 1

;by Victor Lazzarini 2008

instr 1
kscal active  1
kamp port   1/kscal, 0.01
asig  oscili  kamp, p4, 1
kenv linseg 0, 0.1,1,p3-0.2,1,0.1, 0

        out asig*kenv
endin

</CsInstruments>
<CsScore>
f1 0 16384 10 1

i1 0 10 440
i1 1 3  220
i1 2 5  350
i1 4 3  700
</CsScore>
</CsoundSynthesizer>