<CsoundSynthesizer>
<CsOptions>
; no sound output
 -n
</CsOptions>
<CsInstruments>

sr = 44100	; audio rate is not important
kr = 10	; execute the statements in instr 1 ten times per second

instr 1
	; ax will vary from 1 to 10
	ax         init        1
	
	; ay = ax^3 + 2ax^2 + 3ax + 4
	ay         polynomial  ax, 4, 3, 2, 1
	
	; convert our a-rate signals to k-rate values so that we can print
	ky         downsamp    ay
	kx         downsamp    ax
	           printf      "%d:  %d\n", kx, kx, ky

	ax         =           ax + 1
endin

</CsInstruments>
<CsScore>

i1 0 1
e

</CsScore>
</CsoundSynthesizer>
