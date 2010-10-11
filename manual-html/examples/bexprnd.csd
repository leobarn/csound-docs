<CsoundSynthesizer>
<CsOptions>
-odac           -iadc    ;;;RT audio I/O
; -o bexprnd.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
ksmps = 10
nchnls = 1

; Instrument #1.
instr 1
  ; Generate a random number between -1 and 1.
  ; krange = 1

  i1 bexprnd 1

  print i1
endin

; Instrument #2.
instr 2
  ; Generate a random number between -1 and 1.
  ; krange = 1

  ; *** Set seed from time system
  seed 0

  ; *** Every run time different value
  i1 bexprnd 1

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
