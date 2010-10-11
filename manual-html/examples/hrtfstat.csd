<CsoundSynthesizer>
  <CsOptions>
    ; Select flags here
    ; realtime audio out
    ; -o dac
    ; For Non-realtime ouput leave only the line below:
    -o hrtf.wav
  </CsOptions>
  <CsInstruments>

    sr = 44100
    kr = 4410
    ksmps = 10
    nchnls = 2

    gasrc init 0

    instr 1		;a plucked string

    kamp = p4
    kcps = cpspch(p5)
    icps = cpspch(p5)

    a1 pluck kamp, kcps, icps, 0, 1

    gasrc = a1

    endin

    instr 10	;uses output from instr1 as source

    aleft,aright hrtfstat gasrc, 90,0, "hrtf-44100-left.dat","hrtf-44100-right.dat"

    outs	aleft, aright

    endin

  </CsInstruments>
  <CsScore>

    ; Play Instrument 1: a plucked string
    i1 0 2 20000 8.00

    ; Play Instrument 10 for 2 seconds.
    i10 0 2

  </CsScore>
</CsoundSynthesizer>
