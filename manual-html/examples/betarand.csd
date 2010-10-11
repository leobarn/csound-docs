<CsoundSynthesizer>
<CsOptions>
-odac           -iadc    ;;;RT audio I/O
; -o betarand.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
ksmps = 441
nchnls = 1

; Instrument #1.
instr 1
  ; Generate a number between 0 and 1 with a 
  ; uniform distribution.
  ; krange = 1
  ; kalpha = 1
  ; kbeta = 1

  ; ** every run time same value
  i1 betarand 1, 1, 1

  print i1
endin

; Instrument #2.
instr 2
  ;  Same of above but with
  ;  seed from system time

  seed 0

  ; ** every run time different value
  i1 betarand 1, 1, 1

  print i1
endin

</CsInstruments>
<CsScore>

; Play Instrument #1 for one second.
i 1 0 1
; Play Instrument #2 for one second.
i 2 1 1
e

</CsScore>
</CsoundSynthesizer>
