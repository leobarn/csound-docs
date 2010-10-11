<CsoundSynthesizer>
<CsInstruments>
sr     = 44100
kr     = 4410
ksmps  = 10
nchnls = 2




; Instrument List
# define dseq       # 1 #
# define doNothing  # 2 #





; instr $dseq
#include "./dseq.csinstr"


instr $doNothing
	inull = p3
	inull = p4
	inull = p5
	inull = p6
	inull = p7
	inull = p8
	inull = p9
	inull = p10
	inull = p11 

	; Do Nothing

endin




</CsInstruments>
<CsScore>

; Instrument List
# define dseq       # 1 #
# define doNothing  # 2 #




; Display various errors with the dseq error messager
i $dseq 0 1 $doNothing 0 0 0 0 0 "f... e... f... 8..."        
i $dseq 0 1 $doNothing 0 0 0 0 0 "f... r 8... f... 8..."      
i $dseq 0 1 $doNothing 0 0 0 0 0 "r87f88 f88 f88 f88"         
i $dseq 0 1 $doNothing 0 0 0 0 0 "r0 f... 8... f... 8..."     
i $dseq 0 1 $doNothing 0 0 0 0 0 "f... 8... r0t f88 f88 "     
i $dseq 0 1 $doNothing 0 0 0 0 0 "f.f. r8t f88 r16f.8. f.8. " 




; dseq error messages
/*
dseq error.  Invalid character detected:
        f... r 8... f... 8...
              ^
        Expects 0-9 here.
        Interpreter terminated.


dseq error.  Invalid character detected:
        r87f88 f88 f88 f88
           ^
        Expects ' ', t or d here.
        Interpreter terminated.

dseq error.  Invalid value detected:
        r0 f... 8... f... 8...
         ^
        Expects a value greater than 0 here.
        Interpreter terminated.


dseq error.  Invalid value detected:
        f... 8... r0t f88 f88 
                   ^
        Expects a value greater than 0 here.
        Interpreter terminated.


dseq error.  Invalid character detected:
        f.f. r8t f88 r16f.8. f.8. 
                        ^
        Expects ' ', t or d here.
        Interpreter terminated.
*/

</CsScore>
</CsoundSynthesizer>

