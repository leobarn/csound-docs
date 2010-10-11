/* defg.h */
/* general macros, typedefs */

#include "/csound/cscore.h"
#include "defd.h"	/* macros for pre-sco operations */
#include "defs.h"	/* macros for rsco and psco operations */

#define DATAFIELDL "(W)orking parameters, (M)inspec, spec(P)lex, or (F)ullplex"
#define DATAFIELDW "(W)orking parameters, (M)inspec, spec(P)lex, (F)ullplex,\n; (A)dsyn control file, (S)corefile for Csound, or more (R)eadable score"
#define DFABBL "wmpf"
#define DFABBW "wmpfasr"
#define PARS 1
#define MINSPEC 2
#define SPECPLEX 3
#define FULLPLEX 4
#define ASCF 5
#define FSCO 6
#define MRSCO 7

#define MAX_PATHLEN 50
#define MAX_FNLEN MAX_PATHLEN+50

#define FILETYPE "([B])inary or (T)ext file"
#define FTABB "bt"
#define BIN 1
#define TXT 2

#define GOT_MINSPEC			proc_flag[0]
#define GOT_SPECPLEX		proc_flag[1]
#define GOT_FULLPLEX		proc_flag[2]
#define SCALED_FULLPLEX		proc_flag[3]
#define GOT_SCO				proc_flag[4]
#define NUM_PROC_FLAGS		5

#define ISFRAC(a) ((a)<1 ? 1 : -1)
#define SIGN(a) ((a)>0 ? 1 : -1)
#define SQR(a) ((a)*(a))

#define LOG_2 0.3010299957F
#define DEG0 0.0F
#define DEG30 0.5235987756F
#define DEG60 1.047197551F
#define DEG90 1.570796327F
#define DEG120 2.094395102F
#define DEG150 2.617993878F
#define DEG180 3.141592654F
#define DEG210 3.665191429F
#define DEG240 4.188790205F
#define DEG270 4.712388980F
#define DEG300 5.235987756F
#define DEG330 5.759586532F
#define DEG360 6.283185307F

#define POSHALFPI DEG90
#define NEGHALFPI -DEG90

typedef unsigned int iun;

typedef struct coordlist {
	float x;
	float y;
} clist;

typedef struct parameters {
	int i;
	float f;
	char fname[80];
	int mod;
	char dex[80];
} pars;

typedef struct coordplex {
	double x;
	double y;
	double hd;
	double hr;
} cplex;

int proc_flag[NUM_PROC_FLAGS];
char i1pf[NUM_I1PFIELDS+1][15], i2pf[NUM_I2PFIELDS+1][15], i4pf[NUM_I4PFIELDS+1][15];
pars dpa[MAX_PARAMS];
clist specxy[MAX_POI];
cplex specplex[MAX_POI], fullplex[MAX_POI];
EVLIST *rsco, *sco, *f;
EVENT **i1ev, **i2ev, **i4ev;

void display(int what, FILE *fp), show_params(FILE *fp),
  init(int what, int which), copy_specxy(void), make_fullplex(void),
  s_and_q(void), calc_hd_hr(void), adj_params(void), enteredit_specxy(void),
  show_minspec(FILE *fp), show_specplex(FILE *fp), show_fullplex(FILE *fp),
  show_sco(int what, FILE *fp), make_sco(void),
  load(int what), swrite(int what, int which),
  getmaxmin(cplex *fplex, int np);
char *getack(void), *get_filename(int what, int which);
int task(void), wh(char *chdx, char *chab), check_pause(int t),
  talmid(int *tally), talmin(int *tally), talmax(int *tally),
  comp(const void *i, const void *j), pc(double p), susum(int n),
  readlist(FILE *fp, char *name, clist *fld, int beg, int end),
  readplex(FILE *fp, char *name, cplex *fld, int beg, int end),
  writelist(FILE *fp, char *name, clist *fld, int beg, int end),
  writeplex(FILE *fp, char *name, cplex *fld, int beg, int end),
  instr_1(int nl), instr_2(int nl), instr_4(int nl);
double ampdb(double p), dbamp(double p), cpsoct(double p), octcps(double p),
  quant(double value, double quantum),
  gof(double value, double comp), lof(double value, double comp),
  betw(double value, double min, double max), 
  fbetw(double value, double min, double max), avg(double val1, double val2),
  linscale(double value, double imin, double imax, double smin, double smax),
  logscale(double value, double imin, double imax, double smin, double smax);
FILE *tryopen(FILE *fp, char *filename, char *mode);

