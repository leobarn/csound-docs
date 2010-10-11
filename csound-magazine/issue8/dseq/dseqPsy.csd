<CsoundSynthesizer>
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 2




/*
Musical example, psyKick and and psyBass by:
	Joe Maffei

Web:
	www.joemaffei.com
*/




; Instrument list
# define dseq     # 1 #
# define psyKick  # 2 #
# define psyBass  # 3 #




; instr dseq
#include "./dseq.csinstr"




instr $psyKick

; by Joe Maffei
; www.joemaffei.com

	ikf     = p3
	ispb    = p4
	inull   = p5
	ivalue  = p6 
	iamp    = p7 * 0dbfs * ( ivalue / 15 )  ; Amplitude
	ifreq   = p8                            ; Frequency
	ireverb = p9                            ; Reverb
	inull   = p10
	inull   = p11


	; Extend time
	idur = 0.5 * ispb  ; Duration in beat time
	xtratim idur - ikf

	k1 linseg 1.5, .200, .5
	k2 linseg 2, .060, 1
	k3 linseg 4, .010, 1
	k4 linseg 4, .002, 1
	klvl linseg 0, .0002, 1, .030, .8, .080, .7, idur-.1102, 0
	akick oscil iamp*klvl, ifreq*k1*k2*k3*k4, 1
	al,ar freeverb akick, akick, 0.8, 0.35, sr, 0
	al = (akick*(1-ireverb))+(al*ireverb)
	ar = (akick*(1-ireverb))+(ar*ireverb)
	outs al, ar
endin




instr $psyBass

; by Joe Maffei
; www.joemaffei.com

	ikf      = p3
	inull    = p4
	inull    = p5
	ivalue   = p6
	iamp     = p7 * 0dbfs
	ibasepch = p8
	ireverb  = p9
	inull    = p10
	inull    = p11

	; Extend Time
	idur = 0.5
	xtratim idur - ikf

	ipch = cpspch( ibasepch + ( ivalue * 0.01 ) )

	klvl expseg 1, .050, .5, .450, 0.00003
	aosc1 vco iamp*klvl, ipch, 1, .5, 1
	aosc2 vco iamp*klvl, ipch*1.01, 1, .5, 1
	aosc3 vco iamp*klvl, ipch*0.99, 1, .5, 1
	aoscall = aosc1+aosc2+aosc3
	kcut linseg 1, .030, .1
	afilt moogladder aoscall, 5000*kcut, .4
	al,ar freeverb afilt, afilt, 0.8, 0.35, sr, 0
	al = afilt+al*ireverb
	ar = afilt+ar*ireverb
	outs al, ar
endin




</CsInstruments>
<CsScore>

# define dseq     # 1 #
# define psyKick  # 2 #
# define psyBass  # 3 #


f 1 0 8192 10 1

t 0 150

; Musical example by Joe Maffei
; www.joemaffei.com

i $dseq 0 1 $psyKick 0.8 44   0.01 0   0 "f... f... f... f..." 
i $dseq 0 1 $psyBass 0.8 5.09 0.04 0.5 0 "..0. ..c. ..0a ..c." 

i $dseq 4 1 $psyKick 0.8 44   0.01 0   0 "f... f... f... f..." 
i $dseq 4 1 $psyBass 0.8 5.09 0.04 0.5 0 "..0. ..c. ..0a ..c." 

i $dseq 8 1 $psyKick 0.8 44   0.01 0   0 "f... f... f... f..." 
i $dseq 8 1 $psyBass 0.8 5.09 0.04 0.5 0 "..0. ..c. ..0a ..c." 

i $dseq 12 1 $psyKick 0.8 44   0.01 0   0 "f... f... f... f..." 
i $dseq 12 1 $psyBass 0.8 5.09 0.04 0.5 0 "..0. ..c. ..0a ..c." 


e 16

</CsScore>
</CsoundSynthesizer>

