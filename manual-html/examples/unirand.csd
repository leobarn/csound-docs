<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in
-odac           -iadc    ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o unirand.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
kr = 4410
ksmps = 10
nchnls = 1

; Instrument #1.
instr 1
  ; Generate a random number between 0 and 1.
  ; krange = 1

  i1 unirand 1

  print i1
endin

; Instrument #2.
instr 2
  ; Generate a random number between 0 and 1.
  ; krange = 1

  seed 0

  i1 unirand 1

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
