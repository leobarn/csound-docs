; Code for installation 'Marginalia Part 2' at Cardiff School of Psychology with some minor revisions.
; Written by Richard Bowers, 2008.


<CsoundSynthesizer>
<CsOptions>

-i devaudio1 -o devaudio1 -b4000 -M0


</CsOptions>
<CsInstruments>


sr=44100
kr=441
ksmps=100
nchnls=8
gkrms init 0
gkrmsmean init 0
gkmakenoisedur init 3
gklistendur init 1
gkswitch init 0
gkdistance init 0
gifloor	init	-40; after high pass filtering 100hz
gilevel1 init -37
gilevel2 init -35
gilevel3 init -25
gkvol_scale init 1
gkdB	init -40
gkdBinv init 40

;*************************************************
instr	1; LISTENING PATCH
; LISTENING SECTION
kfloor init gifloor
kpadding=kfloor*.1
kthresh=gkrmsmean+abs(kpadding)
printk2	kthresh
;use ins for live installation, diskin for development
;asigl, asigr	ins
asigl, asigr	diskin	"foyer4.wav", 1, 0, 1
asigl	butterhp	asigl, 100; filter to emphasise significant noises
;outs	asigl, asigl
kblock init 0
krmsacc init 0
gkrms	rms	asigl
kdB=dbfsamp(gkrms)
gkdB=kdB
gkdBinv=abs(kdB)
;kmeter=(kdB < kthresh ? 0 : kdB)

; CREATE SWITCHING MECHANISM
kphase	phasor	1/(gklistendur+gkmakenoisedur)
gkswitch	table	kphase, 1, 1; VALUES 0 AND 1: proportion 3 noise 1 listen
gkdistance	tablei	kphase, 70, 1
printk2	gkswitch		

; WHEN SWITCH = 0 START LISTENING
if gkswitch == 1 kgoto calc
krmsacc=kdB+krmsacc; accumulate the dB values
kblock=0
kgoto skip
calc:
if kblock=1 kgoto skip
gkrmsmean=krmsacc/(kr*gklistendur); get the mean rms over one ilistendur of listening
krmsacc=0; reset accumulator to 0
kblock=1
skip:
kfloor = gkrmsmean
printk2 gkrmsmean
endin
;************************************************
instr	10; CONDITIONAL BRANCHES

level1:
if gkrmsmean > gilevel1 kgoto level2
gkmakenoisedur=17
gklistendur=gkmakenoisedur/3
gktranspindex1=.3
gkspeedindex1=.01
gkvol_scale=1
kregion=1
kgoto jump

level2:
if gkrmsmean > gilevel2 kgoto level3
gkmakenoisedur=11
gklistendur=gkmakenoisedur/3
gktranspindex1=.8
gkspeedindex1=.05
gkvol_scale=1.5
kregion=2

kgoto jump

level3:
if gkrmsmean > gilevel3 kgoto level4
gkmakenoisedur=7*abs(rnd(1))
gklistendur=gkmakenoisedur/3
gktranspindex1=1
gkspeedindex1=1
gkvol_scale=2
kregion=3

kgoto jump

level4: 
gkmakenoisedur=5*abs(rnd(1))
gklistendur=gkmakenoisedur/3
gktranspindex1=1
gkspeedindex1=10
gkvol_scale=3
kregion=4

jump:
printk2	kregion
	endin
;****************************************************************
instr	30; FILE PLAYBACK

ktransp=gktranspindex1
ktransp_mobile=gktranspindex1+(gkdB*.005)
kspeed=gkspeedindex1
iseed=2					

idur1  	filelen   	"stanza_w01.pvx"   ; get dur of speech file
idur2  	filelen   	"stanza_w02.pvx"
idur3  	filelen   	"stanza_w03.pvx"         	
idur4  	filelen   	"stanza_w04.pvx"

kphase	phasor	.1
krandh	randh	1, 1, iseed
krand	randi	1, .1, iseed
krand=abs(krand)
kshuffle	randi idur1*.01*kphase, 10, iseed

kpos1	phasor	((krand/idur1)-abs(kshuffle)*krandh)*kspeed
kpos2	phasor	((krand/idur2)-abs(kshuffle))*kspeed
kpos3	phasor	(((krand*.3)/idur3)-abs(kshuffle)*krandh)*kspeed
kpos4	phasor	(((krand*.01)/idur4)-abs(kshuffle))*kspeed

kpos_1=kpos1*idur1
kpos_2=kpos2*idur2
kpos_3=kpos3*idur3
kpos_4=kpos4*idur4

flivew1	pvsfread	kpos_1, "stanza_w01.pvx", 0
flivew2	pvsfread	kpos_2, "stanza_w02.pvx", 0
flivew3	pvsfread	kpos_3, "stanza_w03.pvx", 0
flivew4	pvsfread	kpos_4, "stanza_w04.pvx", 0


kratioa=ktransp
kratiob=ktransp+krand
kratioc=ktransp
kratiod=ktransp_mobile

flivew01	pvscale	flivew1, kratioa
flivew02	pvscale	flivew2, kratiob
flivew03	pvscale	flivew3, kratioc
flivew04	pvscale	flivew4, kratiod


alivewA	pvsynth	flivew01; whispered
alivewB	pvsynth	flivew02; whispered
alivewC	pvsynth	flivew03; whispered
alivewD	pvsynth	flivew04; whispered


;****************************
;SPATIALISATION
;Outer speakers receive the reverberated signal only
;Inner speakers are dry
;+1 on distance prevents 0 NOISE!!
kreverbsend1=.05
kdist=((1-gkdistance)*10)+1
katt=1-gkdistance

aA1, aA2, aA3, aA4	locsig	alivewA, kpos1*360, kdist, kreverbsend1
asendaA1, asendaA2, asendaA3, asendaA4	locsend

aB1, aB2, aB3, aB4	locsig	alivewB, (kpos2*360)+90, kdist, kreverbsend1
asendaB1, asendaB2, asendaB3, asendaB4	locsend

aC1, aC2, aC3, aC4	locsig	alivewC, (kpos3*360)+180, kdist, kreverbsend1
asendaC1, asendaC2, asendaC3, asendaC4	locsend

aD1, aD2, aD3, aD4	locsig	alivewD, (kpos4*360)+270, kdist, kreverbsend1
asendaD1, asendaD2, asendaD3, asendaD4	locsend


achan1=(aA1+aB1+aC1+aD1)*gkvol_scale; dry to channels 1-4
achan2=(aA2+aB2+aC2+aD2)*gkvol_scale;
achan3=(aA3+aB3+aC3+aD3)*gkvol_scale;
achan4=(aA4+aB4+aC4+aD4)*gkvol_scale;

arev5=(asendaA1+asendaB1+asendaC1+asendaD1); wet to channels 5-8
arev6=(asendaA2+asendaB2+asendaC2+asendaD2)
arev7=(asendaA3+asendaB3+asendaC3+asendaD3)
arev8=(asendaA4+asendaB4+asendaC4+asendaD4)

achan5	nreverb	arev5*katt, 10, .5; 
achan6	nreverb	arev6*katt, 10, .5; 
achan7	nreverb	arev7*katt, 10, .5;
achan8	nreverb	arev8*katt, 10, .5; 


outo	achan1, achan2, achan3, achan4, achan5, achan6, achan7, achan8


	endin
;*************************************************
</CsInstruments>
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
<CsScore>
f0	4000000; 46 days at kr=441
f1	0	16	-2	1 1 1 1 1 1 1 1	 1 1 1 1 0 0 0 0 ; switching table
f70	0	2048	-27	0 0 100 1 1500 1 1600 0 2047 0; envelope for makenoise/listen + used for distance and reverb

i1	0	4000000; listening
i10	0	4000000; selecting
i30	0	4000000; phase vocoding + spatialisation

e



</CsScore>
</CsoundSynthesizer>
