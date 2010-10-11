eg4.csd
<CsoundSynthesizer>
<CsOptions>
    -o dac 
</CsOptions>
<CsInstruments>

sr = 48000
kr = 4800
ksmps = 10    
nchnls = 2

    instr 1		

kaz line 0, p3, 90
ain soundin "sample48k.wav"
aleft,aright hrtfmove ain, kaz,0, "hrtf-48000-left.dat","hrtf-48000-right.dat", 1, 0, 48000

    outs	aleft, aright

    endin

</CsInstruments>
<CsScore>
  
    i1 0 2 

</CsScore>
</CsoundSynthesizer>


