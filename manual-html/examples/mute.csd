<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in    No messages
-odac           -iadc     -d     ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o mute.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
ksmps = 10
nchnls = 1
0dbfs = 1

; Mute Instrument #2.
mute 2
; Mute Instrument three.
mute "three"

; Instrument #1.
instr 1
  a1 oscils 0.2, 440, 0
  out a1
endin

; Instrument #2.
instr 2
  a1 oscils 0.2, 880, 0
  out a1
endin

; Instrument #3.
instr three
  a1 oscils 0.2, 1000, 0
  out a1
endin


</CsInstruments>
<CsScore>

; Play Instrument #1 for one second.
i 1 0 1
; Play Instrument #2 for one second.
i 2 0 1
; Play Instrument three for one second.
i three 0 1
e


</CsScore>
</CsoundSynthesizer>
