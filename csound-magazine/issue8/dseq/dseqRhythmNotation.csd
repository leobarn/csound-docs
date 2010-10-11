<CsoundSynthesizer>
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 2




; Instrument List
# define dseq  # 1 #
# define snare # 2 #




; instr dseq
#include "./dseq.csinstr"


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




</CsInstruments>
<CsScore>

; Instrument List
# define dseq  # 1 #
# define snare # 2 #

; Tables
f 1 0 [2^16+1] 10 1                     ; Sine
f 2 0 8192     -7 -1 4096 1 4096 -1     ; Triangle

; Tempo
t 0 120

; Rhythms from section "Rhythm Notation"
i $dseq 0  1 $snare 0.5 0 0 0 0 "f... 8... f... 8..."          
i $dseq 4  1 $snare 0.5 0 0 0 0 "r4 f 8 f 8"                   
i $dseq 8  1 $snare 0.5 0 0 0 0 "r8t f88 f88 f88 f88"          
i $dseq 12 1 $snare 0.5 0 0 0 0 "r4d5 f8888 f8888 f8888 f8888" 
i $dseq 16 1 $snare 0.5 0 0 0 0 "f... r4 8 r8t f88 r8 8."      

e 20

</CsScore>
</CsoundSynthesizer>

