<CsoundSynthesizer>
<CsOptions>
-odac8
;insert option flags here
</CsOptions>
<CsInstruments>

sr = 44100
kr = 44100
ksmps = 1
nchnls = 2 

gkx init 0
gky init 0
gkz init 0 
gkroll init 0 

;Initialization of OSC listening on port 50000
gilisten OSCinit 50000

;------------------
instr 1 
;------------------
;Initialization of OSC output variables
	ka 	init 0
	kb 	init 0
	kminus 	init 0 
	kx 	init 0 
	ky	init 0
	kz 	init 0  
        kroll  	init 0 

;OSC listen codes for specific variables
k1 OSClisten gilisten, "/wii/a", "f", ka
k2 OSClisten gilisten, "/wii/b", "f", kb
k3 OSClisten gilisten, "/wii/minus", "f", kminus
k4 OSClisten gilisten, "/wii/x", "f", kx
k5 OSClisten gilisten, "/wii/y", "f", ky
k6 OSClisten gilisten, "/wii/z", "f", kz
k7 OSClisten gilisten, "/wii/roll", "f", kroll

;Variable output scaling
gkx = kx
gky = ky
gkz = kz
kroll = (abs(kroll)*0.25)/45  ; scales kroll to 0-1 for panning 
gkroll = kroll
;Triggers
ktr1 trigger ka,0.9,0			; These triggers fire when 
ktr2 trigger kb,0.9,0			; the a,b or minus buttons
ktr3 trigger kminus,0.9,0		; are pushed

;Schedules
schedkwhen ktr1,3,1,2,0,3		; When the corresponding trigger
schedkwhen ktr2,3,1,3,0,3		; fires, these schedule the instruments
schedkwhen ktr3,3,1,4,0,3		; and limit them to one event at a time


endin

;----------------
instr 2   ;A-trigger
;---------------
kamp = 5000
kva  = i(gkx)		;Vibrato Amp controlled by X 
kvib oscil kva,5,1 
kpan = i(gkroll)		;Panning controlled by Roll

aosc oscil 1,400+kvib,1

al = aosc * kpan
ar = aosc * (1-kpan)

outs al * kamp,ar * kamp

endin

;----------------
instr 3   ;B-trigger
;---------------
kamp = 5000
kva  = i(gky)		;Vibrato Amp controlled by Y
kvib oscil kva,5,1 
kpan = i(gkroll)		;Panning controlled by Roll

aosc oscil 1,600+kvib,1

al = aosc * kpan
ar = aosc * (1-kpan)

outs al * kamp,ar * kamp

endin

;----------------
instr 4   ;Minus-trigger
;---------------
kamp = 5000
kva  = i(gkz)		;Vibrato amp controlled by Z
kvib oscil kva,5,1 
kpan = i(gkroll)		;Panning controlled by Roll

aosc oscil 1,500+kvib,1

al = aosc * kpan
ar = aosc * (1-kpan)

outs al * kamp,ar * kamp

endin



</CsInstruments>
<CsScore>
 
f 1 0 1024 10 1 

i1 0 100

e

</CsScore>
</CsoundSynthesizer>