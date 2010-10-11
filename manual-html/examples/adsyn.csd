<CsoundSynthesizer>
<CsOptions>
-odac           -iadc     -d     ;;;RT audio I/O
; -o adsyn.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
ksmps = 10
nchnls = 1
0dbfs = 1

; Instrument #1.
instr 1
  ; If the modulation amounts are set to 1, adsyn
  ; will not perform any special modulation.
  kamod init 1
  kfmod init 1
  ksmod init 1

  ; Re-synthesizes the file "kickroll.het".
  a1 adsyn kamod, kfmod, ksmod, "kickroll.het"

  out a1
endin


</CsInstruments>
<CsScore>

; Play Instrument #1 for one second.
i 1 0 1
e


</CsScore>
</CsoundSynthesizer>

