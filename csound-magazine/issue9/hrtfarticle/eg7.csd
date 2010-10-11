<CsoundSynthesizer>   

<CsOptions>
-o dac 
</CsOptions>

<CsInstruments>

nchnls = 2   

opcode Binambi, aa, aaaaaaaaa
 
aw, ax, ay, az, ar, as, at, au, av xin
        
; decode B format for 8 channel circle loudspeaker setup
a1, a2, a3, a4, a5, a6, a7, a8 bformdec 4, aw, ax, ay, az, ar, as, at, au, av
       
; binaurally encode each channel
; (hrtf phase truncation...)
 al1, ar1 	hrtfmove  a2,  22.5,   0, "hrtf-44100-left.dat", "hrtf-44100-right.dat"
 al2, ar2 	hrtfmove  a1,  67.5,   0, "hrtf-44100-left.dat", "hrtf-44100-right.dat"
 al3, ar3 	hrtfmove  a8,  112.5,  0, "hrtf-44100-left.dat", "hrtf-44100-right.dat"
 al4, ar4 	hrtfmove  a7,  157.5,  0, "hrtf-44100-left.dat", "hrtf-44100-right.dat"
 al5, ar5 	hrtfmove  a6,  202.5,  0, "hrtf-44100-left.dat", "hrtf-44100-right.dat"
 al6, ar6 	hrtfmove  a5,  247.5,  0, "hrtf-44100-left.dat", "hrtf-44100-right.dat"
 al7, ar7 	hrtfmove  a4,  292.5,  0, "hrtf-44100-left.dat", "hrtf-44100-right.dat"
 al8, ar8 	hrtfmove  a3,  337.5,  0, "hrtf-44100-left.dat", "hrtf-44100-right.dat"
 
 aleft = (al1+al2+al3+al4+al5+al6+al7+al8)/8
 aright = (ar1+ar2+ar3+ar4+ar5+ar6+ar7+ar8)/8
      
 ; write binaural audio out
        xout aleft, aright
endop

instr 1

asrc	loscil 30000, 1, 1, 1, 2		;loop whole file, forward and back...

; two full turns
kangle line 0, p3, 720
        
; generate B format, second order...
aw, ax, ay, az, ar, as, at, au, av bformenc asrc, kangle, 0, 1, 1, 1
aleft,aright Binambi aw, ax, ay, az, ar, as, at, au, av
outs aleft, aright

endin

</CsInstruments>

<CsScore>

; Play Instrument #1 for 30 seconds.
f1 0 0 1 "sample.wav" 0 0 0
i 1 0 30
 
</CsScore>

</CsoundSynthesizer> 