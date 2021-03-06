; mandel_hz.orc
; this instrument assumes a pitch/frequency table in f1 
; with frequency specified in Hz

sr = 44100
kr = 441
ksmps = 100
nchnls = 1


instr 1
	ipitchtable = 1							; pitch table in score		
	ipitchndx = p5							; p5=pitch index from table
	
   	ipitch 	table  ipitchndx, ipitchtable	
    kenv    expseg  1.0, 1.0, 1.0, 11.5, .0001		      	
    asig    pluck  ampdb(p4)*kenv, ipitch, ipitch, 0, 1
    out     asig
endin




