<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
-+rtaudio=jack -i adc -o dac -+rtmidi=alsa -M hw:1
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
ksmps = 128
nchnls = 2


; Instrument #1.
instr 1
  ikoct     =     8     ;4 octave range
  kfratio     midic7     1, 1, ikoct ;mod.wheel controls transposition 
  ifn = 1
  i1 cpsmidi
  ivel veloc
  kamp = ivel*500
  kfreq = i1*kfratio
  kjet = rnd(0.5)
  iatt = 0.1
  idetk = 0.1
  kngain = rnd(0.05)
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
