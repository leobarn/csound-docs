<CsoundSynthesizer>
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 2




; Instrument List
# define dseq          # 1 #
# define dseqTemplate  # 2 #
# define valueAmp      # 3 #
# define valuePitch    # 4 #
# define paramStandard # 5 #
# define paramGlobal   # 6 #
# define paramTable    # 7 #
# define durFixed      # 8 #
# define durAbsolute   # 9 #
# define durBeatTime   # 10 #
# define durDivision   # 11 #




; Globals for instr paramGlobal
giamp   = 0.5
gipitch = 9.00
gipan   = 0.9




; instr dseq
#include "./dseq.csinstr"




instr $dseqTemplate
	ikf    = p3   ; One k-frame in seconds
	ispb   = p4   ; Seconds per beat
	ires   = p5   ; The resolution of the beat.  One beat = 1
	ivalue = p6   ; Numeric value of the note trigger
	iudp1  = p7   ; User-defined parameter 1
	iudp2  = p8   ; User-defined parameter 2
	iudp3  = p9   ; User-defined parameter 3
	iudp4  = p10  ; User-defined parameter 4
	iudp5  = p11  ; User-defined parameter 5

	idur = 1            ; Set duration to one second
	xtratim idur - ikf  ; Extend time


	; Insert Synthesis Engine Here


endin




instr $valueAmp
	ikf    = p3
	inull  = p4
	inull  = p5
	ivalue = p6
	inull  = p7
	inull  = p8
	inull  = p9
	inull  = p10
	inull  = p11


	; Instrument duration
	idur = 0.5
	xtratim idur - ikf


	; Convert note trigger value to amplitude
	iamp = ( ivalue / 15 ) * 0dbfs


	; Oscillator amplitude controlled by dseq value
	aosc oscils iamp, 440, 0


	; Amplitude envelope
	aenv line 1, idur, 0
	aosc =    aosc * aenv


	; Output
	outs aosc, aosc
endin




instr $valuePitch
	ikf      = p3
	inull    = p4
	inull    = p5
	ivalue   = p6
	iamp     = p7  ; Amplitude
	ibasepch = p8  ; Base Pitch
	inull    = p9
	inull    = p10
	inull    = p11


	; Instrument duration
	idur = 0.5
	xtratim idur - ikf


	; Convert note trigger value to pitch
	ipch = cpspch( ibasepch + ( 0.01 * ivalue ) )


	; Scale amplitude
	iamp = iamp * 0dbfs


	; Oscillator amplitude controlled by dseq value
	aosc oscils iamp, ipch, 0


	; Amplitude envelope
	aenv line 1, idur, 0
	aosc =    aosc * aenv


	; Output
	outs aosc, aosc
endin




instr $paramStandard
	ikf    = p3
	inull  = p4
	inull  = p5
	ivalue = p6
	iamp   = p7  ; Amplitude
	ipitch = p8  ; Pitch
	ipan   = p9  ; Pan
	inull  = p10
	inull  = p11


	; Instrument duration
	idur = 0.5
	xtratim idur - ikf


	; Oscillator
	iamp =      iamp * ( ivalue / 15 ) * 0dbfs
	aosc oscils iamp, cpspch( ipitch ), 0


	; Amplitude
	aenv line 1, idur, 0
	aosc =    aosc * aenv


	; Output with panning
	outs aosc * sqrt( 1 - ipan ), aosc * sqrt( ipan )
endin




instr $paramGlobal
	ikf    = p3
	inull  = p4
	inull  = p5
	ivalue = p6
	inull  = p7
	inull  = p8
	inull  = p9
	inull  = p10
	inull  = p11


	; Get global values
	iamp   = giamp
	ipitch = gipitch
	ipan   = gipan


	; Instrument duration
	idur = 0.5          ; Fixed time of 0.5 seconds
	xtratim idur - ikf  ; Extend time of current i-event to 0.5 seconds


	; Oscillator
	iamp =      iamp * ( ivalue / 15 ) * 0dbfs
	aosc oscils iamp, cpspch( ipitch ), 0


	; Amplitude
	aenv line 1, idur, 0
	aosc =    aosc * aenv


	; Output with panning
	outs aosc * sqrt( 1 - ipan ), aosc * sqrt( ipan )
endin




instr $paramTable
	ikf    = p3
	inull  = p4
	inull  = p5
	ivalue = p6
	iamp   = p7  ; Amplitude
	itable = p8  ; Table storing parameters
	inull  = p9
	inull  = p10
	inull  = p11


	; Get values from table
	ipitch   table 0, itable  ; Pitch
	iwave    table 1, itable  ; Wave shape
	ivibfreq table 2, itable  ; Vibrato frequency
	ivibamt  table 3, itable  ; Vibrato amount, in half steps
	ivibwave table 4, itable  ; Vibrato wave shape
	iatt     table 5, itable  ; Attack of envelope
	ipan     table 6, itable  ; Pan
	

	; Instrument duration
	idur = 0.5
	xtratim idur - ikf


	; Pitch
	ipitch = cpspch( ipitch )


	; Amplitude Envelope
	iamp  =     iamp * ( ivalue / 15 ) * 0dbfs
	aenv linseg 0, iatt * idur, iamp, idur * ( 1 - iatt ), 0


	; Vibrato lfo
	klfo oscil 0.5, ivibfreq, ivibwave, -1
	klfo =     2^(( klfo + 0.5 ) * ivibamt / 12)


	; Oscillator
	aosc oscil aenv, ipitch * klfo, iwave, -1


	; Output with panning
	outs aosc * sqrt( 1 - ipan ), aosc * sqrt( ipan )
endin




instr $durFixed
	ikf   = p3
	inull = p4
	inull = p5
	inull = p6
	inull = p7
	inull = p8
	inull = p9
	inull = p10
	inull = p11

	; Instrument duration
	idur = 1
	xtratim idur - ikf  ; Extend time to idur


	; Oscillator
	aosc oscil 0dbfs, 262, 1


	; Output with panning
	outs aosc, aosc
endin




instr $durAbsolute
	ikf   = p3
	inull = p4
	inull = p5
	inull = p6
	iamp  = p7  ; Amplitude
	idur  = p8  ; Duration in seconds
	inull = p9
	inull = p10
	inull = p11

	; Instrument duration
	xtratim idur - ikf  ; Extend time to idur


	; Amplitude
	iamp =      iamp * 0dbfs
	aenv linseg iamp, idur - 0.005, iamp, 0.005, 0  ; Remove click


	; Oscillator
	aosc oscil aenv, 440, 1


	; Output with panning
	outs aosc, aosc
endin




instr $durBeatTime
	ikf   = p3
	ispb  = p4  ; Seconds per beat
	inull = p5
	inull = p6
	iamp  = p7  ; Amplitude
	idur  = p8  ; Duration in beats
	inull = p9
	inull = p10
	inull = p11


	; Instrument duration
	idur = idur * ispb  ; Convert to relative time
	xtratim idur - ikf  ; Extend time to idur


	; Amplitude
	iamp =      iamp * 0dbfs
	aenv linseg iamp, idur - 0.005, iamp, 0.005, 0  ; Remove click


	; Oscillator
	aosc oscil aenv, 440, 1


	; Output with panning
	outs aosc, aosc
endin




instr $durDivision
	ikf   = p3
	ispb  = p4  ; Seconds per beat
	ires  = p5  ; Resolution of trigger
	inull = p6
	iamp  = p7  ; Amplitude
	inull = p8
	inull = p9
	inull = p10
	inull = p11


	; Instrument duration
	idur = ispb * ires  ; Set duration to note division
	xtratim idur - ikf  ; Extend time to idur


	; Amplitude
	iamp =      iamp * 0dbfs
	aenv linseg iamp, idur - 0.005, iamp, 0.005, 0  ; Remove click


	; Oscillator
	aosc oscil aenv, 440, 1


	; Output with panning
	outs aosc, aosc
endin




</CsInstruments>
<CsScore>

; Instrument List
# define dseq          # 1 #
# define dseqTemplate  # 2 #
# define valueAmp      # 3 #
# define valuePitch    # 4 #
# define paramStandard # 5 #
# define paramGlobal   # 6 #
# define paramTable    # 7 #
# define durFixed      # 8 #
# define durAbsolute   # 9 #
# define durBeatTime   # 10 #
# define durDivision   # 11 #


; Wave shapes
f 1 0 [2^16+1] 10 1                          ; Sine
f 2 0 [2^16+1] -7 -1 [2^15] 1 [2^15] -1      ; Triangle
f 3 0 [2^16+1] -7 1 [2^16] -1                ; Saw
f 4 0 [2^16+1] -7 1 [2^15] 1 0 -1 [2^15] -1  ; Square


; Parameter Table
f 100 0 8 -2 8.00 2 7 5 4 0.1 0.24 0


; Tempo
t 0 90



i $dseq 0  1 $valueAmp      0   0    0   0 0 "f.2. 4.6. 8.a. 0.f." 
i $dseq 4  1 $valuePitch    0.5 8.00 0   0 0 "0.2. 4.5. 7.2. 7.0." 
i $dseq 8  1 $paramStandard 0.5 8.07 0.1 0 0 "f.2. 4.6. 8.a. 0.f." 
i $dseq 12 1 $paramGlobal   0   0    0   0 0 "f.2. 4.6. 8.a. 0.f." 
i $dseq 16 1 $paramTable    0.5 100  0   0 0 "2... 6... c84. f..." 
i $dseq 20 1 $durFixed      0   0    0   0 0 "f... .... f... ...." 
i $dseq 24 1 $durAbsolute   0.5 0.5  0   0 0 "f... f... f... f..." 
i $dseq 28 1 $durBeatTime   0.5 0.5  0   0 0 "f... f... f... f..." 
i $dseq 32 1 $durDivision   0.5 0.5  0   0 0 "f... f... f... f..." 

e 36

</CsScore>
</CsoundSynthesizer>

