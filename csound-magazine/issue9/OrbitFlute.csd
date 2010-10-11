<CsoundSynthesizer>
<CsOptions>
; Select audio/midi flags here according to platform
; Audio out   Audio in    No messages
-odac          
-iadc     -d     ;;;RT audio I/O
; For Non-realtime ouput leave only the line below:
; -o planet.wav -W ;;; for file output any platform
</CsOptions>
<CsInstruments>

; Initialize the global variables.
sr = 44100
kr = 4410
ksmps = 10
nchnls = 2

; Instrument #1 - a planet oribiting in 3D space.
instr 1
; Create our basic tone, using the waveguide flute opcode
; to generate an interesting tone, adding 200 to our random
; pitch to make sure you can hear it.
kamp = 9000
kfreq = rnd(700)+100
kjet = rnd(0.9)+ 0.1
iatt = rnd(0.9)+ 0.1
idetk = rnd(0.9)+ 0.1
kngain = rnd(0.3)+ 0.1
kvibf = rnd(20)+ 1
kvamp = rnd(0.09)+ 0.01
ifn = 1

asnd wgflute kamp, kfreq, kjet, iatt, idetk, kngain, kvibf, kvamp, ifn

;Figure out its X, Y, Z coordinates.
km1 init 0.55
km2 init 0.535
ksep init 0.72
ix = 0
iy = 0.1
iz = 0
ivx = 0.5
ivy = 0
ivz = 0
ih = 0.0001
ifric = -0.01
ax1, ay1, az1 planet km1, km2, ksep, ix, iy, iz, ivx, ivy, ivz, ih, ifric

; Place the basic tone within 3D space.
kx downsamp ax1
ky downsamp ay1
kz downsamp az1
idist = 1
ift = 0
imode = 1
imdel = 1.018853416
iovr = 2
aw2, ax2, ay2, az2 spat3d asnd, kx, ky, kz, idist, ift, imode, imdel, iovr

;Convert the 3D sound to stereo.
aleft = aw2 + ay2
aright = aw2 - ay2

;Clip the output to prevent distortion
a1 clip aleft, 2, 40000
a2 clip aright, 2, 40000
outs a1,a2
endin
</CsInstruments>
<CsScore>
; Table #1 a sine wave.
f 1 0 16384 10 1
; Play Instrument #1 for 10 seconds.
i 1 0 100
i 1 1 100
i 1 2 100
e
</CsScore>
</CsoundSynthesizer>
