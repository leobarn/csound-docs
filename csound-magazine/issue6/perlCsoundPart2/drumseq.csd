<CsoundSynthesizer>
; This file generated by perl
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1


instr 1
	idur = p3
	iamp = p4
	
	kenv1 expseg 900, 0.01, 50, idur - 0.01, 20
	asig1 oscil3 1, kenv1, 1
	kenv2 line   1, idur, 0
	asig1 =      asig1 * kenv2

	asig2  gauss  1
	kenv5  expseg 800, 0.1, 50, idur - 0.1, 20
	asig2  tone   asig2, kenv5
	
	amix  = asig1 + asig2
	
	kenv5 expseg 500, 0.05, 60, idur - 0.05, 20
	amix  rezzy  amix, kenv5, 10

	kenv6 linseg 50, idur, 20
	aosc  oscil3 1, kenv6, 1

	kenv4 expseg 2, 0.15, 1, idur - 0.15, 1
	kenv4 =      kenv4 - 1

	amix =  ( amix * 0.8 + aosc  * 1.2 ) * 15000 * kenv4 * iamp

	out amix
endin


instr 2
	idur     = p3
	idynamic = p4
	
	atri         oscil3 1, 111 + idynamic * 5, 2	
	areal, aimag hilbert atri

	ifshift =      175
	asin    oscil3 1, ifshift, 1	
	acos    oscil3 1, ifshift, 1, .25	
	amod1   =      areal * acos
	amod2   =      aimag * asin
	ashift1 =      ( amod1 + amod2 ) * 0.7

	ifshift2 =      224
	asin     oscil3 1, ifshift2, 1	
	acos     oscil3 1, ifshift2, 1, .25	
 	amod1    =      areal * acos
	amod2    =      aimag * asin
	ashift2  =      ( amod1 + amod2 ) * 0.7
	
	kenv1     linseg 1, 0.15, 0, idur - 0.15, 0
	ashiftmix =      ( ashift1 + ashift2 ) * kenv1
	
	aosc1   oscil3 1, 180, 1
	aosc2   oscil3 1, 330, 1
	kenv2   linseg 1, 0.08, 0, idur - 0.08, 0
	aoscmix =      ( aosc1 + aosc2 ) * kenv2

	anoise gauss    1
	anoise butterhp anoise, 2000
	anoise butterlp anoise, 3000 + idynamic * 3000
	anoise butterbr anoise, 4000, 200
	kenv3  expseg   2, 0.15, 1, idur - 0.15, 1
	anoise =        anoise * ( kenv3 - 1 )
	
	amix = aoscmix + ashiftmix + anoise * 4
	
	out amix * 7000 * idynamic
endin


instr 3
	idur     = p3
	idynamic = p4

	ifreq =     125 + ( 2 * idynamic )
	a1    oscil 1, ifreq * 1, 5
	a2    oscil 1, ifreq * 2.333, 5
	a3    oscil 1, ifreq * 3.578, 5
	a4    oscil 1, ifreq * 5.123, 5
	a5    oscil 1, ifreq * 7.632, 5
	a6    oscil 1, ifreq * 9.843, 5
	amix  =     a1 + a2 + a3 + a4 + a5 + a6
	
	idecay1 =      0.08 + ( 0.03 * ( 1 - idynamic ) )
	kenv1   expseg 1, 0.01, 2, idecay1, 1, idur - idecay1 - 0.01, 1
	kenv1   =      kenv1 - 1
	amix    =      amix * kenv1
	
	idecay2 =      0.11 + 0.05 * idynamic
	kenv2   linseg 1, idecay2, 0, idur - idecay2, 0
	anoise  gauss  1
	
	amix =        ( anoise * kenv2 ) + amix * 0.5
	amix butterhp amix, 7000	
	amix butterlp amix, 9000 + idynamic * 3000
	
	out amix *  10000 * idynamic
endin


</CsInstruments>
<CsScore>
f1 0 65536 10 1
f2 0 8192 -7 -1 4096 1 4096 -1
f3 0 8192 -7 -1 8192 1
f5 0 8192 -7 1 200 1 0 -1 7912 -1

t 0 100

i3 0.0000 1 1.0000
i3 0.5000 1 1.0000
i3 1.0000 1 1.0000
i3 1.5000 1 1.0000
i3 2.0000 1 1.0000
i3 2.5000 1 1.0000
i3 3.0000 1 1.0000
i3 3.5000 1 1.0000
i2 1.0000 1 1.0000
i2 3.0000 1 1.0000
i1 0.0000 1 1.0000
i1 2.0000 1 1.0000
i1 2.5000 1 1.0000
i3 4.0000 1 0.8000
i3 4.5000 1 0.4000
i3 5.0000 1 0.6667
i3 5.5000 1 0.4000
i3 6.0000 1 0.8000
i3 6.5000 1 0.4000
i3 7.0000 1 0.6667
i3 7.5000 1 0.4000
i2 5.0000 1 0.5333
i2 7.0000 1 0.5333
i2 7.7500 1 0.6667
i1 4.0000 1 0.5333
i1 6.0000 1 0.8000
i1 6.5000 1 0.6667
i3 8.0000 1 0.8000
i3 8.5000 1 0.4000
i3 9.0000 1 0.6667
i3 9.5000 1 0.4000
i3 10.0000 1 0.8000
i3 10.5000 1 0.4000
i3 11.0000 1 0.6667
i3 11.5000 1 0.4000
i2 9.0000 1 0.5333
i2 11.0000 1 0.5333
i2 11.7500 1 0.6667
i1 8.0000 1 0.5333
i1 10.0000 1 0.6667
i1 10.5000 1 0.5333
i3 12.0000 1 1.0000
i3 12.3333 1 0.5333
i3 12.6667 1 0.5333
i3 13.0000 1 0.6667
i3 13.3333 1 0.5333
i3 13.6667 1 0.5333
i3 14.0000 1 1.0000
i3 14.3333 1 0.5333
i3 14.6667 1 0.5333
i3 15.0000 1 0.6667
i3 15.3333 1 0.5333
i3 15.6667 1 0.5333
i2 13.0000 1 0.8000
i2 15.0000 1 0.8000
i1 12.0000 1 1.0000
i1 14.0000 1 1.0000
i3 16.0000 1 1.0000
i3 16.3333 1 0.5333
i3 16.6667 1 0.5333
i3 17.0000 1 0.6667
i3 17.3333 1 0.5333
i3 17.6667 1 0.5333
i3 18.0000 1 1.0000
i3 18.3333 1 0.5333
i3 18.6667 1 0.5333
i3 19.0000 1 0.6667
i3 19.3333 1 0.5333
i3 19.6667 1 0.5333
i2 17.0000 1 0.8000
i2 19.0000 1 0.8000
i1 16.0000 1 1.0000
i1 18.0000 1 1.0000
i3 20.0000 1 0.6667
i3 20.2500 1 0.6667
i3 20.5000 1 0.6667
i3 20.7500 1 0.6667
i3 21.0000 1 0.6667
i3 21.2000 1 0.6667
i3 21.4000 1 0.6667
i3 21.6000 1 0.6667
i3 21.8000 1 0.6667
i3 22.0000 1 0.6667
i3 22.1667 1 0.6667
i3 22.3333 1 0.6667
i3 22.5000 1 0.6667
i3 22.6667 1 0.6667
i3 22.8333 1 0.6667
i3 23.0000 1 0.6667
i3 23.1429 1 0.6667
i3 23.2857 1 0.6667
i3 23.4286 1 0.6667
i3 23.5714 1 0.6667
i3 23.7143 1 0.6667
i3 23.8571 1 0.6667
i2 21.0000 1 1.0000
i2 23.0000 1 1.0000
i1 20.0000 1 1.0000
i1 22.0000 1 1.0000
i1 22.7500 1 1.0000
i3 24.0000 1 0.6667
i3 25.0000 1 0.6667
i3 26.0000 1 0.6667
i3 27.0000 1 0.6667
i2 25.0000 1 0.8000
i2 26.5000 1 0.8000
i1 24.0000 1 1.0000
i1 24.5000 1 1.0000
i1 25.5000 1 1.0000
i1 26.0000 1 1.0000
e
</CsScore>
</CsoundSynthesizer>
