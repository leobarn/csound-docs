<CsoundSynthesizer>
<CsOptions>
    -o dac 
</CsOptions>
<CsInstruments>

    nchnls = 2

    instr 1		

    kaz line 0, p3, 90
    ain soundin "impulse.wav"
    aleft,aright hrtfmove ain, kaz,0, "hrtf-44100-left.dat","hrtf-44100-right.dat", 0,1

    outs	aleft, aright

    endin

</CsInstruments>
<CsScore>
  
    i1 0 1 

</CsScore>
</CsoundSynthesizer>
