<CsoundSynthesizer>
<CsOptions>
; Audio out   Audio in    No messages
-odac           -iadc     -d     ;;;RT audio I/O
; -o diskin.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
ksmps = 10
nchnls = 1

; Instrument #1 - play an audio file.
instr 1
  ; Play the audio file
  asig diskin "beats.wav", 1
  out asig
endin

; Instrument #2 - play an audio file backwards.
instr 2
  ; Play the audio file backwards.
  asig diskin "beats.wav", -1
  out asig
endin

</CsInstruments>
<CsScore>

; Play Instrument #1, the audio file, for three seconds.
i 1 0 3
i 2 3 3
e

</CsScore>
</CsoundSynthesizer>
