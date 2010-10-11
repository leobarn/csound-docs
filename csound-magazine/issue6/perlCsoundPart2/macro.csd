<CsoundSynthesizer>
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 1

# define sine        #1#
# define puretone    #16#
# define fourfourty  #440#

instr $puretone
	a1 oscil 20000, $fourfourty, $sine

	out a1
endin

</CsInstruments>

<CsScore>
# define sine        #1#
# define puretone    #16#
# define fourfourty  #440#

f$sine 0 8192 10 1

i$puretone 0 8

</CsScore>
</CsoundSynthesizer>

