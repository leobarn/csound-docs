/* defd.h */
/* macros for pre-sco operations */

#define DEX(str) strcpy(dpa[nu].dex,str)
#define MOKI dpa[nu].mod
#define MOD (((int)MOKI/10)*10)
#define KIND MOKI-MOD
#define INT 1
#define FLOAT 2
#define FNAME 3
#define YES 10
#define NO 0
#define NAME_IS(str) strcpy(dpa[nu].fname, str)
#define ONLY_ON_ALL if(which==ALL)
#define NEXT nu++
#define SET_NUMPARAMS PA_NUMPARAMS=nu

#define NA 0
#define ALL 0
#define TIND 1

#define MAX_PARAMS 40
#define MAX_NPC 50
#define MAX_POI 32767
#define ABS_XMAX 32767
#define ABS_YMAX 32767
#define ABS_MINAMPF .00006103516
#define ABS_MINAMPI 2
#define ABS_MAXAMPI 32766
#define ABS_MINFREQ 50
#define ABS_MAXFREQ 18000

#define PA_NUMPARAMS dpa[0].i
#define PA_PATH dpa[1].fname
#define PA_PARSFILE dpa[2].fname
#define PA_MINSPECFILE dpa[3].fname
#define PA_SPECPLEXFILE dpa[4].fname
#define PA_FULLPLEXFILE dpa[5].fname
#define PA_ASCFFILE dpa[6].fname
#define PA_FSCOFILE dpa[7].fname
#define PA_MRSCOFILE dpa[8].fname
#define PA_SPECPOI dpa[9].i
#define PA_FULLPLEXPOI dpa[10].i
#define PA_XMAX dpa[11].f
#define PA_XMIN dpa[12].f
#define PA_YMAX dpa[13].f
#define PA_YMIN dpa[14].f
#define PA_STEP dpa[15].i
#define PA_DURUNIT dpa[16].f
#define PA_DURMF dpa[17].i
#define PA_MAXDB dpa[18].f
#define PA_MINDB dpa[19].f
#define PA_MINPCH dpa[20].f
#define PA_MAXPCH dpa[21].f
#define PA_MAXWOB dpa[22].f
#define PA_NPC dpa[23].i
#define PA_MINDIST dpa[24].f
#define PA_MAXDIST dpa[25].f
#define PA_RVBTIME dpa[26].f
#define PA_TERMLINES dpa[27].i

#define DEF_PATH "/Home/noel/epocmucpasslat/sbsb/"
#define DEF_PARSFILE "data/working.pars"
#define DEF_MINSPECFILE "data/curve.mspec"
#define DEF_SPECPLEXFILE "data/curve.xspec"
#define DEF_FULLPLEXFILE "data/curve.fplex"

#define PA_XRANGE (PA_XMAX-PA_XMIN)
#define PA_YRANGE (PA_YMAX-PA_YMIN)
#define PA_PCHRANGE (PA_MAXPCH-PA_MINPCH)
#define PA_DBRANGE (PA_MAXDB-PA_MINDB)
#define PA_MINFREQ (cpsoct(PA_MINPCH))
#define PA_MAXFREQ (cpsoct(PA_MAXPCH))
#define PA_MINDUR gof((float)PA_DURUNIT/PA_DURMF,0.25)
#define PA_MAXDUR ((float)PA_DURUNIT*PA_DURMF)
#define PA_MINWOB (PA_MAXWOB/10)
#define PA_MINREACH PA_DURMF
#define PA_MAXREACH PA_DURMF*PA_DURMF
