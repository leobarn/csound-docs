;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;;   Example Instrument Using ``base'' and ``supplementary'' heterodyne data
;;;   The base data is supplied by Violins
;;;   The supp data is supplied by Trumpet
;;;
;;;   The heterodyne data is stored in subdirectory ``HetroData''
;;;
;;;   fabio.bertolotti@dlr.de,  1997.
;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


   
sr     = 22050             ; samples per second                  
kr     = 2205              ; control signals update, per second   
ksmps  = 10                ; sr/kr                                 
nchnls = 1                 ; mono output channel               


garev   init    0          ; Initialize reverb


;-------
instr 99                   ; Reverberation is explained else where
;-------
  ar      reverb2 garev, 2.5, 0.4    
          out     ar
  garev   =       0
endin





;-------
instr 1                                                                
;-------


;;;
;;;  p-Fields:  p2 = start time
;;;             p3 = duration
;;;             p4 = frequency in pitch class
;;;             p5 = amplitude, linear, in the range [0, 1]
;;;             p6 = initial note amplitude (non-zero for legato effect)

       idur  =  (p3 > 0.10 ? p3 : 0.10)  ; duration  (sec), 0.1 sec. minimum    
       ifreq =   cpspch(p4)              ; frequency in pitch form 
       iamp  =   p5                      ; amplitude, linear, in range  0 to 1
       irise =   p6                      ; end value for amplitude envelope
                                         ; (non-zero for legato effect)         



;;;                                                                             
;;;  Frequency and duration of sampled sound:                         
;;;  (Note: use hetroPlot to get these values)
;;;
;;;    ``Trumpet_f4''   freq = 345 Hz,  duration = 4.093 seconds   
;;;    ``Trumpet_ds5''  freq = 622 Hz,  duration = 3.895 seconds   
;;;                                                                             
       idur1 = 4.093     ;  duartion of hetro files at f4
       ifre1 = 345       ;  base frequency of files at f4

       idur2 = 3.895     ;  duartion of hetro files at ds5
       ifre2 = 622       ;  base frequency of files at ds5


;;;                                                                             
;;;  Here we choose which of the two sampled sounds we use                      
;;;  and set the variable ``iper'' equal to the sampled sound duration          
;;;                                                                             
       iper  =    (ifreq < (ifre1 + ifre2)/2 ?  idur1 : idur2)         
 

;;;                                                                             
;;;  Overall amplitude envelope                                       
;;;                                                                             
       kresc linseg  iamp, 2*idur/3,  iamp, idur/3, p6



;;;                                                                         
;;;  No sample-period shortening
;;;                                                                             
       ksmod  =      1


;;;                                                                             
;;;  Now we have to chose the amplitude weights with which we mix the           
;;;  base + supplementary data.  Here, we amplify the trumpet's supplementary
;;;  data for short notes in order to obtain a staccato sound.
;;; (Note that iamp is in the range of 0 to 1)                                 
;;;                                                                             

       idd   =  idur/iper                   ;  Duration / sampled sound length
       id2   =  (idd < 1 ? idd : 1)
       kbamp =  1                           ;  Violin data constant at 1
       ksamp =  1                           ;  Violin data constant at 1
       kaamp =  2*(1.1 - id2)               ;  Trumpet data large for short
       kramp =  2*(1.1 - id2)               ;  sounds, small for long sounds

    

;;;                                                                             
;;;  Here we choose which of the two sampled sounds to use                      
;;;                                                                             
;;;
       if (ifreq < (ifre1 + ifre2)/2 ) goto sound1                                          


;;;                                                                             
;;;  Use ``Violins_ds5'' base data and
;;;      ``Trumpet_ds5'' supplemenatry data to synthesize the sound 
;;;  (Note: if the desired frequency is significantly above ds5,
;;;         one may use a low-pass filter to dampen the higher partials)  
;;;                                                                             
       kfreq =        ifreq/ ifre2
       ab    adsyn    kbamp, kfreq, ksmod, "HetroData/Violins_ds5_base.het" 
       aa    adsyn    kaamp, kfreq, ksmod, "HetroData/Trumpet_ds5_sup_a.het"
       as    adsyn    ksamp, kfreq, ksmod, "HetroData/Violins_ds5_sup_s.het"
       ar    adsyn    kramp, kfreq, ksmod, "HetroData/Trumpet_ds5_sup_r.het"
       atot  =        ab + aa + as + ar                                     
             goto     finish


;;;                                                                             
;;;  Use ``Violins_f4'' base data and
;;;      ``Trumpet_f4'' supplemenatry data to synthesize the sound 
;;;                                                                             

sound1:  kfreq =        ifreq/ ifre1
         ab    adsyn    kbamp, kfreq, ksmod, "HetroData/Violins_f4_base.het" 
         aa    adsyn    kaamp, kfreq, ksmod, "HetroData/Trumpet_f4_sup_a.het"
         as    adsyn    ksamp, kfreq, ksmod, "HetroData/Violins_f4_sup_s.het"
         ar    adsyn    kramp, kfreq, ksmod, "HetroData/Trumpet_f4_sup_r.het"
         atot  =        ab + aa + as + ar                              
               goto     finish                                        

finish:                                                                       
         garev =        garev  +  atot*kresc*0.1
               out      atot * kresc                                          

         endin                                                                






