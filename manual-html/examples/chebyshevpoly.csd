<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in
-odac           -iadc    ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o abs.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

sr = 44100
kr = 441

; time-varying mixture of first six harmonics
instr 1
	; According to the GEN13 manual entry,
	; the pattern + - - + + - - for the signs of 
	; the chebyshev coefficients has nice properties.
	
	; these six lines control the relative powers of the harmonics
	k1         line           1.0, p3, 0.0
	k2         line           -0.5, p3, 0.0
	k3         line           -0.333, p3, -1.0
	k4         line           0.0, p3, 0.5
	k5         line           0.0, p3, 0.7
	k6         line           0.0, p3, -1.0
	
	; play the sine wave at a frequency of 256 Hz
	ax         oscili         1.0, 256, 1
	
	; waveshape it
	ay         chebyshevpoly  ax, 0, k1, k2, k3, k4, k5, k6
	
	; avoid clicks, scale final amplitude, and output
	adeclick   linseg         0.0, 0.05, 1.0, p3 - 0.1, 1.0, 0.05, 0.0
	           out            ay * adeclick * 10000
endin

</CsInstruments>
<CsScore>
f1 0 32768 10 1	; a sine wave

i1 0 5
e

</CsScore>
</CsoundSynthesizer>
