<CsoundSynthesizer>
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 2




; Instrument List
# define dseq  # 1 #
# define kick  # 2 #
# define snare # 3 #
# define hihat # 4 #




; instr dseq
#include "./dseq.csinstr"


instr $kick
	ikf    = p3
	inull  = p4
	inull  = p5
	ivalue = p6
	iamp   = p7 * ( ivalue / 15 ) * 0dbfs * 0.5
	inull  = p8
	inull  = p9
	inull  = p10
	inull  = p11


	; Extend time of instrument
	idur = 0.35
	xtratim idur - ikf


	
	kenv1 expseg 900, 0.01, 50, idur - 0.01, 44
	asig1 oscil3 1, kenv1, 1
	kenv2 line   1, idur, 0
	asig1 =      asig1 * kenv2

	asig2  gauss  1
	kenv5  expseg 800, 0.1, 50, idur - 0.1, 44
	asig2  tone   asig2, kenv5
	
	amix  = asig1 + asig2
	
	kenv5 expseg 500, 0.05, 60, idur - 0.05, 44
	amix  rezzy  amix, kenv5, 10

	kenv6 linseg 55, idur, 44
	aosc  oscil3 1, kenv6, 1

	kenv4 expon 2, idur, 1
	kenv4 =      kenv4 - 1

	amix =  ( amix * 0.8 + aosc  * 1.2 ) * kenv4 * iamp

	outs amix, amix
endin




instr $snare
	ikf    = p3
	inull  = p4
	inull  = p5
	ivalue = p6 / 15
	iamp   = p7 * 0.5 * ivalue * 0dbfs
	inull  = p8
	inull  = p9
	inull  = p10
	inull  = p11

	; Extend time of instrument
	idur = 0.15
	xtratim idur - ikf


	
	atri         oscil3 1, 111 + ivalue * 5, 2	
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
	
	kenv1     line 1, 0.15, 0
	ashiftmix =    ( ashift1 + ashift2 ) * kenv1
	
	aosc1   oscil3 1, 180, 1
	aosc2   oscil3 1, 330, 1
	kenv2   linseg 1, 0.08, 0, idur - 0.08, 0
	aoscmix =      ( aosc1 + aosc2 ) * kenv2

	anoise gauss    1
	anoise butterhp anoise, 2000
	anoise butterlp anoise, 3000 + ivalue * 3000
	anoise butterbr anoise, 4000, 200
	kenv3  expon    2, 0.15, 1
	anoise =        anoise * ( kenv3 - 1 )
	
	amix = aoscmix + ashiftmix + anoise * 4
	amix = amix * iamp 

	out amix, amix
endin




instr $hihat
	ikf    = p3
	inull  = p4
	inull  = p5
	ivalue = p6 / 15
	iamp   = p7 * ivalue * 0dbfs
	inull  = p8
	inull  = p9
	inull  = p10
	inull  = p11


	; Extend time of instrument
	idur = 0.07
	xtratim idur - ikf
 

	ifreq =     125 + ( 2 * ivalue )
	a1    oscil 1, ifreq * 1, 5
	a2    oscil 1, ifreq * 2.333, 5
	a3    oscil 1, ifreq * 3.578, 5
	a4    oscil 1, ifreq * 5.123, 5
	a5    oscil 1, ifreq * 7.632, 5
	a6    oscil 1, ifreq * 9.843, 5
	amix1 = a1 + a2 + a5	
	amix2 = a3 + a4 + a6


	idecay1 =      0.08 + ( 0.03 * ( 1 - ivalue ) )
	kenv1   expseg 2, 0.01, 2, 0, 1.6, idecay1, 1, idur - idecay1 - 0.01, 1
	kenv1   =      kenv1 - 1
	amix1   =      amix1 * kenv1
	amix2   =      amix2 * kenv1

	
	idecay2 =      0.11 + 0.05 * ivalue
	kenv2   linseg 1, idecay2, 0, idur - idecay2, 0
	anoise  gauss  1
	

	amix1 =        ( anoise * kenv2 ) + amix1 * 0.5
	amix1 butterhp amix1, 7000	
	amix1 butterlp amix1, 9000 + ivalue * 3000


	amix2 =        ( anoise * kenv2 ) + amix2 * 0.5
	amix2 butterhp amix2, 7000	
	amix2 butterlp amix2, 9000 + ivalue * 3000
	
	amix1 = amix1 * iamp
	amix2 = amix2 * iamp

	out amix1, amix2
endin




</CsInstruments>
<CsScore>

; Instrument List
# define dseq  # 1 #
# define kick  # 2 #
# define snare # 3 #
# define hihat # 4 #

; Tables
f 1 0 [2^16+1] 10 1                     ; Sine
f 2 0 8192     -7 -1 4096 1 4096 -1     ; Triangle
f 5 0 8192     -7 1 200 1 0 -1 7912 -1

; Tempo
t 0 90


; Classic rock beat
i $dseq 0 1 $hihat 0.5 0 0 0 0 "f.f.  f.f.  f.f.  f.f." 
i $dseq 0 1 $snare 0.5 0 0 0 0 "....  f...  ....  f..." 
i $dseq 0 1 $kick  0.5 0 0 0 0 "f...  ....  f.f.  ...." 



; Classic rock beat with uppercase letters, triggers are case insenstive
i $dseq 4 1 $hihat 0.5 0 0 0 0 "F.F.  F.F.  F.F.  F.F." 
i $dseq 4 1 $snare 0.5 0 0 0 0 "....  F...  ....  F..." 
i $dseq 4 1 $kick  0.5 0 0 0 0 "F...  ....  F.F.  ...." 



; Classic rock beat with dynamics
i $dseq 8 1 $hihat 0.5 0 0 0 0 "c.6.  a.6.  c.6.  a.6." 
i $dseq 8 1 $snare 0.5 0 0 0 0 "....  8...  ....  8..a" 
i $dseq 8 1 $kick  0.5 0 0 0 0 "8...  ....  c.a.  ...." 



; Classic rock beat pattern rewritten with resolution function
i $dseq 12 1 $hihat 0.5 0 0 0 0 "r8 c6a6     c6      a   6 " 
i $dseq 12 1 $snare 0.5 0 0 0 0 "r4 . 8      .   r16 8.  .a" 
i $dseq 12 1 $kick  0.5 0 0 0 0 "r4 8 .   r8 a8      .   . " 



; Triplet hi-hats using resolution triplet function
i $dseq 16 1 $hihat 0.5 0 0 0 0 "r8t f88   a88   f88   a88 " 
i $dseq 16 1 $snare 0.5 0 0 0 0 "    ....  c...  ....  c..." 
i $dseq 16 1 $kick  0.5 0 0 0 0 "    f...  ....  f...  ...." 



; Triplet hi-hats using resolution subdivision function
i $dseq 20 1 $hihat 0.5 0 0 0 0 "r4d3 f88   a88   f88   a88 " 
i $dseq 20 1 $snare 0.5 0 0 0 0 "     ....  c...  ....  c..." 
i $dseq 20 1 $kick  0.5 0 0 0 0 "     f...  ....  f...  ...." 



; Irrational rhythms
i $dseq 24 1 $hihat 0.5 0 0 0 0 "r4d4 aaaa r4d5 aaaaa r4d6 aaaaaa r4d7 aaaaaaa" 
i $dseq 24 1 $snare 0.5 0 0 0 0 "     ....      f...       ....        f...   " 
i $dseq 24 1 $kick  0.5 0 0 0 0 "     f...      ....       f..f        ....   " 

e 28

</CsScore>
</CsoundSynthesizer>

