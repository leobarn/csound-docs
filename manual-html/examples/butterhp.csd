<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in    Silent
-odac           -iadc     -d    ;;;realtime output
; For Non-realtime ouput leave only the line below:
; -o butterhp.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 22050
kr = 2205
ksmps = 10
nchnls = 1

; Instrument #1 - an unfiltered noise waveform.
instr 1
  ; White noise signal
  asig rand 22050

  out asig
endin


; Instrument #2 - a filtered noise waveform.
instr 2
  ; White noise signal
  asig rand 22050

  ; Filter it, passing frequencies above 250 Hz.
  ahp butterhp asig, 250

  out ahp
endin


</CsInstruments>
<CsScore>

; Play Instrument #1 for two seconds.
i 1 0 2
; Play Instrument #2 for two seconds.
i 2 2 2
e


</CsScore>
</CsoundSynthesizer>
