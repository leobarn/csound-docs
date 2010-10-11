/* CS12CHRD.C - explore pitch/sound continuum with Fibonacci-based chords
 *		CS12CHRD  V0.8TT, A Endrich  17 August 1995"
 * Program to create microtonal Fibonacci harmonies within user-selected
 *	user-selected intervals.
 * This version simplifies the approach as suggested by MCA.
 */

/* HEADERS  - delete as appropriate */
#include <stdio.h>
#include <stdlib.h>	/* for atof() ... */
#include <math.h>	/* for the higher maths functions */
#include <osbind.h>	/* for GEM applications */
#include "cscore.h"	/* omit if not using Cscore */

void phelp(void);

/* USAGE / HELP */
void
phelp(void) {	/* Adapt / omit the phelp() function for each program */
    printf("\nCS12CHRD - create Fibonacci harmonies\n");
    printf("\nUsage:  cs12chrd -sstartser -vnextval -ffreq -iinterval \
-nnumterms outscore\n");
    printf("\t-sstartser	first term of the series (> 0.0)\n");
    printf("\t-vnextval	second term of the series (>startser)\n");
    printf("\t-ffreq		root or central freq in Hz\n");
    printf("\t-iinterval	interval span within which to create the \
harmony\n");
    printf("\t\t\tharmonic complex;  express as a floating point number\n");
    printf("\t-nnumterms	number of terms in the Fibonacci series to\n");
    printf("\t\t\tbe used to generate harmonies (integer)\n");
    printf("\toutscore = name of Csound .sc created by the program\n");
    exit(1);
}

/* DECLARING MAIN */
void
main(int argc, char *argv[])
int	argc;
char	*argv[];
{

/* DECLARATIONS OF VARIABLES */
	EVENT	*e;
	EVLIST	*a;

	double	startser;  /* fp value of initial term in Fibonacci series    */
	double	nextval;   /* next (2nd) value in series */
	double	rootfreq;  /* root or central freq for the harmonic complex   */
	double	interval;  /* interval span within which the harmonic complex */
			   /*   is to be situated;  express as a ratio        */

	int	numterms;  /* number of terms in the Fibonnaci series to be   */
			   /*   used to generate harmonies within interval    */
	int	i;	   /* loop counter */

	double	*fibseries;  /* Array for fibonacci series      */
	double	*harms;    /* Array for frqs which make up the harm */
	double  nextharm;  /* second frequency of the harmony */
	double	fibnvaln;  /* nth value of series */

	double  afreq;	   /* temp variable for 1st two freqs of series */
	double  finalfrq;  /* frequency at nth term of the series */
	double  span;	   /* interval span from freq to finalfrq */
	double  scalingf;  /* scaling factor for frequencies      */
	double  term2temp; /* temp variable for 2nd term of series */
	double  stime1;	   /* start time for 1st event */
	double  stime2;	   /* start time for 3rd event */
	
/*	long    _mem;  	   for a memory check */

/* DISPLAY AVAILABLE RAM WHILE RUNNING THE PROGRAM (written for Atari TT) */
/*  _mem = (long)Malloc(-1); */
/*  printf("\n\nMEMORY CHECK:  _mem is %ld\n\n", _mem); */

/* READ USER-DEFINED FLAGS/PARAMETER INPUT FROM COMMAND LINE */
while(argc > 1 && argv[1][0] == '-' && argv[1][1] != 0) {
	switch(argv[1][1]) {
	case ('s'):
		startser = atof(&argv[1][2]);
		printf("start of series = %f\n", startser);
		argc--, argv++;
		break;
	case ('v'):
		nextval = atof(&argv[1][2]);
		printf("2nd term of series = %f\n", nextval);
		argc--, argv++;
		break;
	case ('f'):
		rootfreq = atof(&argv[1][2]);
		printf("root frequency = %f\n", rootfreq);
		argc--, argv++;
		break;
	case ('i'):
		interval = atof(&argv[1][2]);
		printf("interval ratio = %f\n", interval);
		argc--, argv++;
		break;
	case ('n'):
		numterms = atoi(&argv[1][2]);
		printf("number of terms in series is %d\n", numterms);
		argc--, argv++;
		break;
	default:
		fprintf(stderr, "CS12CHRD: '%s' is not an available \
option\n", argv[1]);
		phelp();
		exit(1);
	}
}

if(argc > 2) {
	fprintf(stderr, "\nYou appear to have too many arguments in\
 the command line.  Check usage below:\n");
	phelp();
	exit(1);
}

/* CDP VERSION OF INITIAL() NB - initial() handles opening infile & outfile */
initial(argc, argv); /* when using cscore.lib only */

/* any other variables to be used need to go here */

/*  DYNAMIC MEMORY ALLOCATION FOR THE ARRAYS */
if((fibseries = (double *)malloc(numterms * sizeof(double)))==NULL) {
	fprintf(stderr,"\n\tcannot allocate memory for fibseries[]\n");
}
if((harms = (double *)malloc(numterms * sizeof(double)))==NULL) {
	fprintf(stderr,"\n\tcannot allocate memory for harms[]\n");
}

/* CALCULATE TERMS OF THE FIBONACCI SERIES ACC. TO USER INPUT */
/* NB - term 1 is input for -sstartser and term 2 is input for -vnextval */
term2temp = nextval;  	   /* temp variable for nextval (which gets updated) */
fprintf(stderr, "\tFibn term 0, value: %f\n", startser);
fprintf(stderr, "\tFibn term 1, value: %f\n", nextval);
for(i = 2; i <= numterms-1;  i++) {
	fibseries[i] = startser + nextval;  /* = the 3rd term of the series */
	startser = nextval;
	nextval = fibseries[i];
   	fprintf(stderr, "\tFibn term %d, value: %f\n", i, fibseries[i]);
}

/* DO CALCULATIONS NEEDED TO DETERMINE THE EQUIVALENT FREQUENCY VALUES */
/* FORMULA: freq[i] = rootfreq + (fibseries[i] * scaling_factor) */
finalfrq = rootfreq * interval;		/* startfrq * interval_ratio */
span     = finalfrq - rootfreq;		/* calculate interval span in Hz */
fibnvaln = nextval;	       /* nextval now set to last val of fibseries[i] */
scalingf = (span/fibnvaln);		/* frequency scaling factor */
nextharm = rootfreq + (term2temp * scalingf); /* 2nd frequency of harmony */

for(i = 2; i <= numterms-1; i++) {
	harms[i] = rootfreq + (fibseries[i] * scalingf); /* starts at 3rdval */
fprintf(stderr, "\ndiagnostic: term %ld, fibnnum %ld", i, fibseries[i]);
}

/* WRITE INITIAL PART OF THE CSOUND SCOREFILE */
putstr(";FIBNHARM.SC\n\n");	/* generic name of outfile */
putstr("f1 0 256 10 1 0.7 0 0.9 0 0.8 0 0.6 ;sine wave function statement\n");
e = defev("t 0 60");		/* define a tempo statement event */
putev(e);	        	/* write this event to the score */
a = lcreat(numterms);

stime1 = 0.0;
/* WRITE THE FIRST TWO SCOREFILE EVENTS (terms 1 & 2 of the series): */
afreq = rootfreq;		/* set first freq to user's -ffreq */
for (i =0; i < 2; i++) {
	e = createv(7);  	/* create space for 7 pfields */
	e->op = 'i';		/* define op code for 'instrument' */
	e->p[1] = 1;		/* set this to instrument 1 */
	e->p[2] = stime1;	/* start at time  */
	e->p[3] = 3;		/* 1st event lasts 2 sec. */
	e->p[4] = 30000/numterms; /* amplitude is scaled */
	e->p[5] = afreq;	/* 1st event: user input in Hz */
	e->p[6] = 0.5;		/* 1/2 sec attack envelope */
	e->p[7] = 0.5;		/* 1/2 sec decay envelope */
	a = lappev(a,e);	/* append event to list of events */
	afreq = nextharm; 	/* set 2nd freq as calculated above */
	stime1 += 0.0;		/* change this to produce temporal offsets */
}
stime2 = 0.0; 
/* WRITE THE REST OF THE SCOREFILE EVENTS: */
for(i = 2; i <= numterms-1; i++) {
	e = createv(7);  	/* create space for 7 pfields */
	e->op = 'i';		/* define op code for 'instrument' */
	e->p[1] = 1;		/* set this to instrument 1 */
	e->p[2] = stime2; 	/* start at time  */
	e->p[3] = 3;		/* 1st event lasts 2 sec. */
	e->p[4] = 30000/numterms;  /* amplitude is scaled */
	e->p[5] = harms[i];
	e->p[6] = 0.5;		/* 1/2 sec attack envelope */
	e->p[7] = 0.5;		/* 1/2 sec decay envelope */
	a = lappev(a,e);	/* append event to list of events */
	stime2 += 0.0;		/* change this to produce temporal offsets */
}

/* COMPLETE THE CSOUND SCORE FILE */
lput(a);			/* write to outfile */
lrelev(a);			/* release memory */
putstr("e");			/* end scorefile marker */

}  /* end of main */

/*
 * Ideas for further development:
 *	gliss from root freq to filled interval;
 *	gliss from n notes in interval a to n notes in interval b;
 *	have rootfreq as central to chord going both up & down.
 */

