<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in    No messages
;-odac           -iadc     -d     ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o wgflute.wav -W ;;; for file output any platform
-+rtaudio=jack -i adc -o dac -+rtmidi=alsa -M hw:1
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
ksmps = 128
nchnls = 2

; Table #1 Kraig Grady's 14 tone mictotuned scale
;  numgrades = 14 (fourteen tones)
 ; interval = 2 (one octave)
 ; basefreq = 261.659 (Middle C)
;  basekeymidi = 60 (Middle C)
  gitemp ftgen 2, 0, 64, -2, 14, 2, 261.659, 60, 1.05, 1.125, 1.166666666666666667, 1.25, 1.3125, 1.333333333333333333, 1.4, \
             1.5, 1.575, 1.6875, 1.75, 1.875, 1.96875, 2

; Instrument #1.
instr 1
  ikoct     =     8     ;4 octave range
  kfratio     midic7     1, 1, ikoct ;mod.wheel controls transposition 
  i1 cpstmid ifn
  ivel veloc
  kamp = ivel*200
  kfreq = i1*kfratio
  kjet = rnd(0.5)
  iatt = 0.1
  idetk = 0.1
  kngain = 0.15
  kvibf = kfratio
  kvamp = 0.05
 
  kenv linsegr 0,0.001,ivel/128, 3, 0
  a1 wgflute kamp, kfreq, kjet, iatt, idetk, kngain, kvibf, kvamp, ifn
 
  outs a1*kenv,a1*kenv
endin
</CsInstruments>
<CsScore>
; Table #1, a sine wave.
f 1 0 16384 10 1
i 1 9999
e
</CsScore>
</CsoundSynthesizer>





