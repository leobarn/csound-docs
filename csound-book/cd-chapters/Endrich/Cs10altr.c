/* CS10ALTR.C - use functions to alter several pfields, with cmndline input
 *		  of scaling factors
 * Developed from Cscore example in Csound manual
 * CS10ALTR V0.7TT A Endrich 17 August 1995
 * This program originally transposed a block of score events down an octave
 * 	and effected a gradual diminuendo.
 * Now it allows user inputs for several pfields and calls separate functions
 *	to alter each one.
 * Note that you need a suitable input score.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cscore.h"

/* DECLARE USER-DEFINED FUNCTIONS */
double newstt(double, double);
double newdur(double, double);
int newamp(int, int);
double newpch(double, double);

void
phelp(void) {
	printf("\nCS10ALTR - alter 4 pfields, with cmdline inputs,\n");
	printf("\tscaling by fixed (non-accumulating) factors\n");
	printf("\nUsage:  cs10altr  -sstartf -ddurf -aampf -ffreqf \
inscore.sc outscore.sc\n");
	printf("\t  -sstartf\tstart_time scaling ratio (*: 0.0->)\n");
	printf("\t  -ddurf\tdur scaling ratio (double) (*: > 0.0)\n");
	printf("\t  -aampf\tamp scaling integer	(+: 0-> x000)\n");
	printf("\t  -ffreqf\tfrq scaling ratio (double)(*: > 0.0)\n");
	printf("\t\tWhen multiplying, vals <1 compress, >1 expand\n");
	printf("\t\tNB1: vals <1 * scaling factors <1 become very small!\n");
	printf("\t\tNB2: if startf = 0, a simultaneity will result\n");
	printf("\t  inscore.sc\tassumed to have diff values for \n");
	printf("\t     each event, but = inscore values will also \n");
	printf("\t     all be = in the outscore, though rescaled\n");
	printf("\t  outscore.sc\tCsnd scorefile produced by prg\n");
		exit(1);
}

void
main(int argc, char *argv[])
int	argc;
char	*argv[];
{
	EVENT *e, **f;
	EVLIST *a, *b;
	int n, i;		/*declare the int counters to be used */
	int ampf = 1000;	/* user input for amplitude scaling factor */

	double startf = 0;	/* user input for start time scaling factor */
	double durf = 1;	/* user input for duration scaling factor */
	double freqf = 1.01;	/* variable for pitch scaling factor */

	/* READ USER-DEFINED FLAGS/PARAMETER INPUT FROM COMMAND LINE */
	while(argc > 1 && argv[1][0] == '-' && argv[1][1] != 0) {
		switch(argv[1][1]) {
		case('s'):
			startf = atof(&argv[1][2]);
			fprintf(stderr,"\nuser's time factor = %f\n", startf);
			argc--, argv++;
			break;
		case ('d'):
			durf = atof(&argv[1][2]);
			printf("user's duration factor = %f\n", durf);
			argc--, argv++;
			break;
		case ('a'):
			ampf = atoi(&argv[1][2]);
			printf("user's ampl factor  = %d\n", ampf);
			argc--, argv++;
			break;
		case ('f'):
			freqf = atof(&argv[1][2]);
			printf("user's freq factor  = %f\n", freqf);
			argc--, argv++;
			break;
		default:
			fprintf(stderr, "CS10ALTR: '%s' is not one of the \
options\n", argv[1]);
			phelp();
			exit(1);
		}
	}

	/* OPEN INSCORE & OUTSCORE */
	initial(argc, argv);	/* check reading the arguments and open files */

	if(argc < 2) {		/* Test that user's commandline is ok */
		phelp();
		exit(1);
	}

	/* PROCEED TO CREATE OUTPUT SCORE FILE */
	putstr(";Csound scorefile output by CS10ALTR:");
	a = lget();		/* read input score into event list "a"  */
	b = lsepf(a);		/* separate out the f statements */
	lput(b);		/* write f statements into the score */
	lrelev(b);		/* and release the spaces used */
	e = defev("t0 76");	/* define event for tempo statement */
	putev(e);		/* write tempo statement to score */
	lput(a);		/* write the list of events "a" */
	putstr("s");		/* write section end marker */
	putev(e);		/* write tempo statement again */
	b = lcopyev(a);		/* make a copy of events  in list "a" */
	n = b->nevents;		/* and count the number copied */
	f = &a->e[1];		/* let f = the address of the first event */
	/* adapt inscore according to user's inputs */
	/* CALL USER-DEFINED FUNCTIONS TO ALTER EACH PFIELD */
	for(i=0; i<=n-1; i++) {
		(*f)->p[2] = newstt((*f)->p[2],startf);	/* alter start times */
		(*f)->p[3] = newdur((*f)->p[3],durf);	/* alter durations */
		(*f)->p[4] = newamp((*f)->p[4],ampf);	/* alter amplitudes */
		(*f)->p[5] = newpch((*f)->p[5],freqf);	/* alter pitches */
		f++;					/* move -> next event */
	}

	a = lcat(b,a);		/* join "a" list events to "b" list events */
	lput(a);		/* write the complete event list to outscore */
	putstr("e");		/* write end-of-score marker */
}				/* end of main */

/* USER-DEFINED FUNCTIONS */
double
newstt(double st, double sf)
/* double	st;	start time variable as in pfield 2 */
/* double	sf;	start time factor input on command line */
{
	st *= sf;	/* start time increases by user's factor */
	fprintf(stderr, "\ndiagnostic, newst(): new start time = %.3f\n", st);
	return (st);
}

double
newdur(double du, double df)
/* double du;	duration variable as in pfield 3 */
/* double df;	duration factor input on command line */
{
	du *= df;	/* duration scaled by user's factor */
	fprintf(stderr, "diagnostic, newst(): new duration = %.3f\n", du);
	return (du);
}

int
newamp(int am, int af)
/* int am;	amplitude variable as in pfield 4 */
/* int af; 	amplitude factor input on command line */
{
	am += af;	/* amplitude increase by user's factor */
	if(am >= 30000) {	/* attempt setting amplitude limits */
		am -= af;	/* dim if reaches upper limit */
		if(am <= 500) {
			am += af; /* cresc if reaches lower limit */
		}
	}
	fprintf(stderr, "diagnostic, newst(): new amplitude = %.3d\n", am);
	return(am);
}

double
newpch(double pch, double ff)
/* double  pch;	pitch value as in pfield 5 */
/* double  ff;  scaling factor input on command line */
{
	pch *= ff;	/* pitch scaled by user's scaling factor */
	fprintf(stderr, "diagnostic, newst(): new pitch = %.3f\n", pch);
	return(pch);
}
	
