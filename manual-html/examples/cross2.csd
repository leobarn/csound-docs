<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in    No messages
-odac           -iadc     -d     ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o cross2.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
kr = 4410
ksmps = 10
nchnls = 1

; Instrument #1 - Play an audio file.
instr 1
  ; Use the "fox.wav" audio file.
  aout soundin "fox.wav"
  out aout
endin

; Instrument #2 - Cross-synthesize!
instr 2
  ; Use the "ahh" sound stored in Table #1.
  icps = p4
  ain1 oscil 30000, p4, 1
  ; Use the "beats.wav" audio file.
  ain2 soundin "fox.wav"

  isize = 4096
  ioverlap = 2
  iwin = 2
  kbias init 1

  aout cross2 ain1, ain2, isize, ioverlap, iwin, kbias

  out 0.8*aout  ;; scale down a bit
endin


</CsInstruments>
<CsScore>

; Table #1: An audio file.
f 1 0 128 1 "eee.aiff" 0 4 0
; Table #2: A windowing function.
f 2 0 2048 20 2

; Play Instrument #1 for 3 seconds.
i 1 0 3
; Play Instrument #2 with various frequencies for "carrier"
i 2 3 3  50
i 2 6 3  100
i 2 9 3  250
i 2 12 3  20
e

</CsScore>
</CsoundSynthesizer>

