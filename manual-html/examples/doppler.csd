<CsoundSynthesizer>
<CsOptions>
-RWdo doppler.wav
</CsOptions>
<CsInstruments>
sr     = 44100
ksmps  = 20
nchnls = 1
                instr 1	
iattack         init        0.05
irelease        init        0.05
isustain        init        p3
p3              init        iattack + isustain + irelease
kdamping        linseg      0.0, iattack, 1.0, isustain, 1.0, irelease, 0.0	
kmic            init        4
                ; Position envelope, with a changing rate of change of position.
kposition       transeg	    4, 4, 0, 120, 4, 4, -3, 50, 2, 4, 0
ismoothinghz    init        6
ispeedofsound   init        340.29
asignal         vco2        10000, 440
aoutput         doppler     asignal, kposition, kmic, ispeedofsound, ismoothinghz
                out         aoutput * kdamping
                endin
</CsInstruments>
<CsScore>
i 1 1.0 20.0 
e 1
</CsScore>
</CsoundSynthesizer>
