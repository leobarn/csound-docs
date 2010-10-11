<CsoundSynthesizer>
<CsOptions>
    -o dac 
</CsOptions>
<CsInstruments>

    nchnls = 2

    instr 1		

    ain soundin "sample.wav"
    aleft,aright hrtfstat ain, -90,0, "hrtf-44100-left.dat","hrtf-44100-right.dat"

    outs	aleft, aright

    endin

</CsInstruments>
<CsScore>
  
    i1 0 2 

</CsScore>
</CsoundSynthesizer>

