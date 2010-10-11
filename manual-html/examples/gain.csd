<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in    No messages
-odac           -iadc     -d     ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o gain.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
kr = 4410
ksmps = 10
nchnls = 1

; Instrument #1.
instr 1
  asrc buzz  30000, 440, sr/440, 1 ; band-limited pulse train.
  a1   reson asrc, 1000, 100       ; Sent through
  a2   reson a1, 3000, 500         ; 2 filters
  krms rms   asrc                  ; then balanced
  afin gain  a2, krms              ; with source
       out   afin
endin


</CsInstruments>
<CsScore>

; Table #1, a sine wave.
f 1 0 16384 10 1

; Play Instrument #1 for two seconds.
i 1 0 2
e

</CsScore>
</CsoundSynthesizer>
