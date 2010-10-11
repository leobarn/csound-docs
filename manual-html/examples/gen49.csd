<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in
-odac           -iadc    ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o gen01.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
kr = 4410
ksmps = 10
nchnls = 1

; Instrument #1.
instr 1
   kamp = 30000
   kcps = 1
   ifn = 1

   ; Play the audio sample stored in Table #1.
   a1 oscil kamp, kcps, ifn
   out a1
endin


</CsInstruments>
<CsScore>

; Table #1: read an audio file (using GEN49).
f 1 0 131072 49 "beats.mp3" 0 1

; Play Instrument #1 for 2 seconds.
i 1 0 2
e


</CsScore>
</CsoundSynthesizer>
