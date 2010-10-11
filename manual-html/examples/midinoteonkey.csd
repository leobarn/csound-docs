<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in   No messages  MIDI in
-odac           -iadc     -d         -M0  ;;;RT audio I/O with MIDI in
; For Non-realtime ouput leave only the line below:
; -o midinoteonkey.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
kr = 4410
ksmps = 10
nchnls = 1

; Instrument #1.
instr 1
  kkey init 0
  kvelocity init 0

  midinoteonkey kkey, kvelocity

  ; Display the key value when it changes.
  printk2 kkey
endin


</CsInstruments>
<CsScore>

; Play Instrument #1 for ten seconds.
i 1 0 10
e


</CsScore>
</CsoundSynthesizer>
