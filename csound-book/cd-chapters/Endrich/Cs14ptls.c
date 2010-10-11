/* CS14PTLS.C - partials evolve and recede during a single long tone
 *		This is a GEN10 timbral processor!
 *		CS14PTLS V0.9TT, A Endrich, 17 August 1995
 *
 * 24 Dec 1997 - Atari drand48() replaced by rand() for use on PC's */
 *
 * This program creates a different f-statement for every note event.  The
 *	information for the partials is contained in a two-dimensional array
 *	within the program -- later to be an implemented as an external file.
 * Start times, durations and amplitude can be handled algorithmically, but
 *	all the frq's are the same:  the idea is to have one tone with
 * 	a timbral evolution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cscore.h"

/* DECLARE #DEFINES */
#define	FIXED		0
#define	RAND		1
#define	LIN_SERIES	2
#define LOG_SERIES	3
#define	EXP_SERIES	4

#define	FPFIELDS  	14
#define	EPFIELDS   	8

/* DECLARE USER-DEFINED FUNCTIONS */
double stt_up(double,double,double,int,int);	/* reset start times incr'g */
double dur_up(double,double,double,int,int);	/* reset durations incr'g */
int    amp_up(int, int, int, int, int);		/* reset amplitude incr'g */
double stt_dwn(double,double,double,int,int);	/* reset start times decr'g */
double dur_dwn(double,double,double,int,int);	/* reset durations decr'g */
int    amp_dwn(int, int, int, int, int);	/* reset amplitude decr'g */

/* DECLARE GLOBAL VARIABLES */
			/* global start times: because no sc to read from */
double	st_nfst = 0;	/* initial 'f' & 'i' st_time, fixed, for type 0 */
double	st_dur = 1;	/* initial duration, fixed, for type 0 */
int	st_amp = 15000;	/* initial amplitude, fixed, for type 0 */
double	holddur;	/* to hold highest value to decrement from */
int	holdamp;	/* to hold highest value to decrement from */

/* HELP / USAGE */
void
phelp(void)
{
	printf("CS14PTLS - unfold & remove partials during a single \
long tone\n");
	printf("Usage: cs14ptls  -nnumftables [-sstartfactor]\n");
	printf("\t[-ddurfactor] [-aamplitude] [-vampfactor]\n");
	printf("\t\t-ffrequency [-brisetime] [-edecaytime]\n");
	printf("\t\t\t[-tprocesstype]  outscore.sc\n");
	printf("\t-nnumftables	(int) num f-s'ments (= num notes)\n");
	printf("\t-sstartfactor	(double) start time shaping factor\n");
	printf("\t\t\tDefault = 1: (no change)\n");
	printf("\t-ddurfactor	(double) event dur shaping factor\n");
	printf("\t\t\tDefault = 1: no change)\n");
	printf("\t-aamplitude	(int) start amp level, Def = 15000\n");
	printf("\t-vampfactor	(int) amplitude shaping factor\n");
	printf("\t\t\tDefault = 0: no change\n");
	printf("\t-ffrequency	(double) freq of ALL note ev's (Hz)\n");
	printf("\t-brisetime	(double) envel rise time, Def=0.5\n");
	printf("\t-edecaytime	(double) envel decay time, Def=0.5\n");
	printf("\t-tprocesstype	(int) factor processing type\n");
	printf("\t\t0 - ADD FIXED value to initial value\n");
	printf("\t\t1 - RANDOM values selected within range\n");
	printf("\t\t2 - LINEAR SERIES values expand/contract\n");
	printf("\t\t3 - LOGARITHMIC SERIES (not implemented)\n");
	printf("\t\t4 - EXPONENTIAL SERIES \n");
	printf("\toutscore.sc	score produced by the program\n");
	printf("Defaults produce smoothly changing timbres, with\n");
	printf("2 sec note events and st times advancing by 1 sec\n");
	printf("\nCommand line using defaults could be e.g.:\n");
	printf("  cs14ptls -n10 -f261.63 ptlssc.sc\n");
	printf("Example full command line:\n");
	printf("  cs14ptls -n10 -s1.2 -d1.5 -a24000 -v550 -f110 \
-b0.75 -e0.75 -t0 ptlssc.sc\n");
	exit(1);
}

void
main(int argc, char *argv[])
int	argc;
char	*argv[];
{
	EVENT	*e;		/* event pointer */
	EVLIST	*a;		/* event list */
	int	numftab;	/* number of f statements & note events */
	int	fnum = 1;	/* function number; initialized to 1 */
	int	ftabnum = 1;	/* f-table number for ea. note event */
	int	i,j = 0;	/* loop counters: outer, inner_up */
	int	k = 0;		/* loop counter for decreasing: inner_dwn */
	int	instrnum = 1;	/* instr number for note events */
	int	namp = 15000;	/* note event amplitude; user may change */
	int	ptype = 0; 	/* alagorithmic process types */

	int	ampf = 0;	/* amplitude increment/decrement factor */
	double	startf = 1;	/* f & i start time shaping factor */
	double	durf = 1;	/* note event duration shaping factor */

	double  fstartt = 0;	/* f statement start times - updated */
	double  nstartt = 0;	/* i (note) event start times - updated  */
	double	ndur = 1;	/* duration: starts at 1 & goes up/down */

	double	frequency;	/* note event pitch (in Hz), all ev's same */
	double	risetime=0.5;	/* note event envelope rise time */
	double	decaytime=0.5;	/* note event decay time */

	double	fhp2 = 0;	/* to hold previous p2 for 'f' statements */
	double	hp2 = 0;	/* to hold previous p2 for 'i' statements */
	double	hp3 = ndur;	/* to hold previous p3 */
	double	hp4 = namp;	/* to hold previous p4 */

	static double	str[10][10] = {
				{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 1,.55, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 1,.55,.6, 0, 0, 0, 0, 0, 0, 0 },
				{ 1,.55,.6,.65, 0, 0, 0, 0, 0, 0 },
				{ 1,.55,.6,.65,.7, 0, 0, 0, 0, 0 },
				{ 1,.55,.6,.65,.7,.75, 0, 0, 0, 0 },
				{ 1,.55,.6,.65,.7,.75,.8, 0, 0, 0 },
				{ 1,.55,.6,.65,.7,.75,.8,.85, 0, 0 },
				{ 1,.55,.6,.65,.7,.75,.8,.85,.9, 0 },
				{ 1,.55,.6,.65,.7,.75,.8,.85,.9,.95 }
			};
		/* two-dimensional array for rel str of the harmonics */

	char	message[60];	/* space for a comment message */

	while(argc > 1 && argv[1][0] == '-' && argv[1][1] != 0) {
		switch(argv[1][1]) {
		case ('n'):
		     numftab = atoi(&argv[1][2]);
		     printf("You have set %d f-tables\n", numftab);
		     argc--, argv++;
		     break;
		case('s'):
		     startf = atof(&argv[1][2]);
		     printf("\nuser's time factor = %f\n", startf);
		     argc--, argv++;
		     break;
		case ('d'):
		     durf = atof(&argv[1][2]);
		     printf("user's duration factor = %f\n", durf);
		     argc--, argv++;
		     break;
		case ('a'):
		     namp = atoi(&argv[1][2]);
		     printf("initial amplitude level = %d\n", namp);
		     st_amp = namp;
		     hp4 = namp;
		     argc--, argv++;
		     break;
		case ('v'):
		     ampf = atoi(&argv[1][2]);
		     printf("user's ampl factor = %d\n", ampf);
		     argc--, argv++;
		     break;
		case ('f'):
		     frequency = atof(&argv[1][2]);
		     printf("all notes are at = %fHz\n", frequency);
		     argc--, argv++;
		     break;
		case ('b'):
		     risetime = atof(&argv[1][2]);
		     printf("envelope rise time = %f\n", risetime);
		     argc--, argv++;
		     break;
		case ('e'):
		     decaytime = atof(&argv[1][2]);
		     printf("envelope decay time = %f\n", decaytime);
		     argc--, argv++;
		     break;
		case ('t'):
		     ptype = atoi(&argv[1][2]);
		     printf("processing type = %d\n", ptype);
		     argc--, argv++;
		     break;
		default:
		     fprintf(stderr,"CS14PTLS:  foxed by '%s'\n", argv[1]);
		     phelp();
		     exit(1);
		}
	}

	initial(argc, argv);	/* enable reading the command line */
	if(argc < 2) {		/* Test that user's commandline is ok */
		phelp();
		exit(1);
	}

	/* CREATE F-STATEMENTS */
	a = lcreat(numftab*2);	/* create numftab function statements */

	sprintf(message, ";CS14PTLS - controlled unfolding of partials\n");
	putstr(message);	/* write this comment to the score */
	
	/* CREATE F-STATEMENTS */
	for(i = 0; i <= numftab-1; i++) {
		e = createv(FPFIELDS);	/* create pfields for each f s'ment */
		e->op = 'f';		/* identifies function statement */
		e->p[1] = fnum;		/* f statement number, begins at 1 */
		e->p[2] = fstartt;	/* start time for f s'ment, start = 0 */
		e->p[3] = 257;		/* table size w. explicit guard point */
		e->p[4] = 10;		/* GEN10: sine with weighted harm's */
		e->p[5] = str[i][0];	/* relative strength of harmonic 1 */
		e->p[6] = str[i][1];	/* relative strength of harmonic 2 */
		e->p[7] = str[i][2];	/* relative strength of harmonic 3 */
		e->p[8] = str[i][3];	/* relative strength of harmonic 4 */
		e->p[9] = str[i][4];	/* relative strength of harmonic 5 */
		e->p[10] = str[i][5];	/* relative strength of harmonic 6 */
		e->p[11] = str[i][6];	/* relative strength of harmonic 7 */
		e->p[12] = str[i][7];	/* relative strength of harmonic 8 */
		e->p[13] = str[i][8];	/* relative strength of harmonic 9 */
		e->p[14] = str[i][9];	/* relative strength of harmonic 10 */
		a = lappev(a,e);
		/* DO PROCESSING TO CALCULATE NEXT F-STATEMENT */
		fnum++;
		if(i < (numftab-1)/2) {		/* 1st half: expand values */
			fstartt = stt_up(fstartt, fhp2, startf, ptype, i+1);
		} else {			/* 2nd half: keep expand'g! */
			fstartt = stt_up(fstartt, fhp2, startf, ptype, i+1);
		}
		fhp2 = fstartt;	/* hold current p2 value for 'f' statements */
	}

	/* INSERT A TEMPO STATEMENT */
	e = defev("\n");	/* write blank line to score */
	lappev(a,e);		/* write this newline into list 'a' */
	e = defev("t 0 76");	/* write tempo statement */
	lappev(a,e);		/* write this tempo statement into list 'a' */


/* We now want 'numftab' note events, one for each f-statement, appending */
/*   them to list 'a' as we go */


	/* CREATE I-STATEMENTS */
	for(j = 0; j <= numftab-1; j++) {
		e = createv(EPFIELDS);	/* EPFIELDS note event pfields */
		e->op = 'i';		/* 'i' identifies note event */
		e->p[1] = instrnum;	/* instr number for note events */
		e->p[2] = nstartt;	/* note event start time, start = 0 */
		e->p[3] = ndur;		/* note event duration, Default = 1 */
		e->p[4] = namp;		/* note event amp, Default = 15000 */
		e->p[5] = frequency;	/* note event pitch in Hz */
		e->p[6] = risetime;	/* envelope rise time, Default = 0.5 */
		e->p[7] = decaytime;	/* envelope decay time, Default = 0.5 */
		e->p[8] = ftabnum;	/* ftable number for this event */
		a = lappev(a,e);
		/* DO PROCESSING TO CALCULATE NEXT I-STATEMENT */
		ftabnum += 1;
/*		if(ptype == 1) {  re random f-statement selection */
/*			ftabnum = (int)((rand()*numftab)+1); */
/*			if(ftabnum >= numftab) { */ /* must be numftab-1 max */
/*				ftabnum -= 1; */
/*			}	*/
/*		}	/* /* commented out because causes gaps in output */
		if(j < (numftab-1)/2) {		/* 1st half: expand values */
			nstartt = stt_up(nstartt, hp2, startf, ptype, j+1);
			ndur = dur_up(ndur, hp3, durf, ptype, j+1);
			namp = amp_up(namp, hp4, ampf, ptype, j+1);
			hp2 = nstartt;	/* hold current p2 value */
			hp3 = ndur;	/* hold current p3 value */
			hp4 = namp;
			holddur = ndur;	/* ends up at highest dur - global! */
			holdamp = namp;	/* ends up at highest amp - global! */
		} else {			/* 2nd half: compress values */
			nstartt = stt_up(nstartt, hp2, startf, ptype, j+1);
			ndur = dur_dwn(ndur, hp3, durf, ptype, k);
			namp = amp_dwn(namp, hp4, ampf, ptype, k);
			hp2 = nstartt;
			hp3 = ndur;
			hp4 = namp;
			k++;
		}
	}
	
	lput(a);		/* now write whole list of events to outscore */
	lrelev(a);		/* release memory space for list 'a' */
	putstr("e");		/* add end of score marker */

}	/* end of main */

double 
stt_up(double p2, double pp2, double sf, int typeflag, int counter)
/* double  p2;		current start time (both 'f' and 'i' events */
/* double  pp2;		previous p2 value */
/* double  sf;		start time change factor */
/* int	typeflag;	type flag */	
/* int	counter;	number of loop in main() */
{
	if(typeflag == FIXED) {				/* type 0 */
		p2 = st_nfst + (counter * sf);	/* adds to same start val */
	}
	if(typeflag == RAND) {				/* type 1 */
		p2 = p2 + (rand() * counter * sf);	
	}
	if(typeflag == LIN_SERIES) {			/* type 2 */
		p2 = pp2 + (counter * sf);	/* uses previous p2 value */
	}
	if(typeflag == LOG_SERIES) {			/* type 3 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES) {			/* type 4 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p2);
}

double		/* this function not debugged & implemented (not used) */
stt_dwn(double p2, double pp2, double sf, int typeflag, int counter)
/* double  p2;		current start time (both 'f' and 'i' events */
/* double  pp2;		previous p2 value */
/* double  sf;		start time change factor */
/* int	typeflag;	type flag */	
/* int	counter;	number of loop in main() */
{
	if(typeflag == FIXED) {				/* type 0 */
		p2 = p2 - (counter * sf);
	}
	if(typeflag == RAND) {				/* type 1 */
		p2 = p2 - (rand() * counter * sf);	
	}
	if(typeflag == LIN_SERIES) {			/* type 2 */
		p2 = pp2 - (counter * sf);	/* uses previous p2 value */
	}
	if(typeflag == LOG_SERIES) {			/* type 3 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES) {			/* type 4 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p2);
}

double
dur_up(double p3, double pp3, double df, int typeflag, int counter)
/* double  p3;		current p3 value */
/* double  pp3;		previous p3 value */
/* double  df;		user's duration factor value */
/* int	typeflag;	type flag */	
/* int	counter;	event loop count in main() */
{
	if(typeflag == FIXED) {				/* type 0 */
		p3 = st_dur + (counter * df);	/* adds to same start val */
	}
	if(typeflag == RAND) {				/* type 1 */
		p3 = p3 + (rand() * counter * df);	
	}
	if(typeflag == LIN_SERIES) {			/* type 2 */
		p3 = pp3 + (counter * df);	/* uses previous p3 value */
	}
	if(typeflag == LOG_SERIES) {			/* type 3 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES) {			/* type 4 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p3);
}

double 
dur_dwn(double p3, double pp3, double df, int typeflag, int counter)
/* double  p3;		current p3 value */
/* double  pp3;		previous p3 value */
/* double  df;		user's duration factor value */
/* int	typeflag;	process type flag */
/* int	counter;	loop count in main() */
{
	if(typeflag == FIXED) {				/* type 0 */
		p3 = holddur - (counter * df);
	}
	if(typeflag == RAND) {				/* type 1 */
		p3 = holddur - (rand() * counter * df);	
	}
	if(typeflag == LIN_SERIES) {			/* type 2 */
		p3 = pp3 - (counter * df);	/* uses previous p3 value */
	}
	if(typeflag == LOG_SERIES) {			/* type 3 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES) {			/* type 4 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p3);
}

int 
amp_up(int p4, int pp4, int af, int typeflag, int counter)
/* int	p4;		current p4 value */
/* int	pp4;		previous p4 value */
/* int	af;		user's amplitude change factor */
/* int	typeflag;	type flag */
/* int	counter;	loop count in main() */
{
	if(typeflag == FIXED) {				/* type 0 */
		p4 = st_amp + (counter * af);	/* adds to same start val */
	}
	if(typeflag == RAND) {				/* type 1 */
		p4 = p4 + (rand() * counter * af);	
	}
	if(typeflag == LIN_SERIES) {			/* type 2 */
		p4 = pp4 + (counter * af);	/* uses previous p4 value */
	}
	if(typeflag == LOG_SERIES) {			/* type 3 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES) {			/* type 4 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p4);
}

int 
amp_dwn(int p4, int pp4, int af, int typeflag, int counter)
/* int	p4;		current p4 value */
/* int	pp4;		previous p4 value */
/* int	af;		user's amplitude change factor */
/* int	typeflag;	type flag */
/* int	counter;	loop count in main() */
{
	if(typeflag == FIXED) {				/* type 0 */
		p4 = holdamp - (counter * af);
	}
	if(typeflag == RAND) {				/* type 1 */
		p4 = holdamp - (rand() * counter * af);	
	}
	if(typeflag == LIN_SERIES) {			/* type 2 */
		p4 = pp4 - (counter * af);	/* uses previous p4 value */
	}
	if(typeflag == LOG_SERIES) {			/* type 3 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES) {			/* type 4 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p4);
}

