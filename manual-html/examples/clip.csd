<CsoundSynthesizer>
<CsOptions>
; Audio out   Audio in    No messages
-odac           -iadc     -d     ;;;RT audio I/O
; -o clip.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
ksmps = 10
nchnls = 1

; Instrument #1.
instr 1
  ; Generate a noisy waveform.
  arnd rand 44100
  ; Clip the noisy waveform's amplitude to 20,000
  a1 clip arnd, 2, 20000

  out a1
endin

; Instrument #2.
instr 2
  ; Generate a noisy waveform.
  arnd rand 44100
  ; Clip the noisy waveform's amplitude to 10,000
  a1 clip arnd, 2, 10000

  out a1
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
