/* CS11CUE.C - create n event blocks at specified time cues, with cresc/dim */

/* CS11CUE V0.8TT A Endrich,  17 August 1995 */
/* Developed from Cscore example in Csound manual */

/* 24 Dec 1997 - Atari drand48() replaced by rand() for use on PC's */

/*
 * Derived from example of applying diminuendo in CDP Csound Manual, p.91
 * Illustrates the way the 'i' events of a score can be separated out,
 *	copied, re-shaped, re-copied etc.; in this case the emphasis is on
 *	start times (p2) and amplitude (p4).
 * Extended (AE) to allow inputting time cues from a file, to control the
 *	number of blocks from the command line, and to cresc/dim to/from the
 *	center of each event block;  also several start time processing
 *	options, each in a separate function;  cries out for a transposition
 *	option.
 * Note that the nature of the inscore.sc greatly affects the results, and
 *	that the blocks of events can be separated in time or overlap,
 *	depending on whether the cues are > or < the time interval between
 *	events in inscore.sc.
 * I've left the diagnostic messages in for now because it's useful to know
 *	what the program thinks these values are.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cscore.h"

/* DECLARE #DEFINES */
#define	ADD		0
#define	MULTIPLY	1
#define	RAND		2
#define	SAME		0
#define	DIFFERENT	1

/* DECLARE USER-DEFINED FUNCTIONS */
void phelp(void);

/* name(pval, cueval, process_type, score_type, counter) */
double stt_up(double, double, int, int, int);	/* fn to shape s_times incr'g */
double stt_dwn(double, double, int, int, int);	/* fn to shape s_times decr'g */

/* name(pval, ampfactor, process_type, score_type, counter) */
int vol_up(int, int, int, int, int);		/* fn to shape amps incr'g */
int vol_dwn(int, int, int, int, int);		/* fn to shape amps decr'g */

/* GLOBAL VARIABLES */
double	*cues;		/* array to hold user's time cues */
double	mult;		/* amplitude change factor multiplier */
int	holdamp;	/* to hold high amp for case SAME down */
int	range;		/* sets range for random function */

/* USAGE / HELP */
void
phelp(void) {
	printf("\nCS11CUE: create n event-blocks at specified time cues, \
with cresc/dim\n");
	printf("\nUsage:\n");
printf("cs11cue  -nnumcues -aampfactor [-mmultiplier] -tprocesstype \n");
	printf("\t\t[-rrange] -iscoretype cuefile inscore.sc outscore.sc\n");
	printf("\t-nnumcues\t(int) number of cues (temp measure!)\n");
	printf("\t-aampfactor\t(int) amp change factor (+: 0->x000)\n");
	printf("\t-mmultiplier\t(double) amp scaling ratio (*: >0\n");
	printf("\t-tprocesstype\t(int) type is way to process times\n");
	printf("\t\t0: ADD fixed value\n");
	printf("\t\t1: MULTIPLY by cueval & multiplier\n");
	printf("\t\t2: RANDOM (randval(0-1)*timecue*range)\n");
	printf("\t-rrange\t\t(int) numerical range for rand fn\n");
	printf("\t\tWARNING: Start times can be excessively high\n");
	printf("\t\tif both start time & cueval are rather large.\n");
	printf("\t-iscoretype\ttype of input scorefile:\n");
	printf("\t\t0: SAME\tvals in event pfields are the same\n");
	printf("\t\t1: DIFFERENT\tvals in ev pfields are different\n");
	printf("\tcuefile\t\tinfile of cue times (sec) (double)\n");
	printf("\tinscore.sc	input Csound score file\n");
	printf("\toutscore.sc	Csound score file output by program\n");
	exit(1);
}

void
main(int argc, char *argv[])
int	argc;
char	*argv[];
{
	EVENT	 *e, **f;	/* **f points to event struc in evlist struc */
	EVLIST	 *a, *b;	/* pointers to the event list structures */
	FILE	 *fp1;		/* input file of cue times */
	int	 i, j, k, n, holdn, numcues;
	int	 eof_flag = 0, ptype, stype, ampf, holdamp = 0;
	char     message[81];	/* storage for diagnostic messages */
	

	/* READ USER-DEFINED FLAGS/PARAMETER INPUT FROM COMMAND LINE */	
	while(argc > 1 && argv[1][0] == '-' && argv[1][1] != 0) {
		switch(argv[1][1]) {
		case ('n'):
			numcues = atoi(&argv[1][2]);
			printf("\n file has %d time cues\n", numcues);
			/* this is a fudge:  should be able to read this from */
			/*   the file itself, but can't program this yet */
			argc--, argv++;
			break;
		case ('a'):
			ampf = atoi(&argv[1][2]);
			printf("\n amp factor is: %f \n", ampf);
			argc--, argv++;
			break;
		case ('m'):
			mult = atof(&argv[1][2]);
			printf("\n amp multiplier is: %f \n", mult);
			argc--, argv++;
			break;
		case ('r'):
			range = atoi(&argv[1][2]);
			printf("\n rand range is: %f \n", range);
			argc--, argv++;
			break;
		case ('t'):
			ptype = atoi(&argv[1][2]);
			printf("processing type is: %d\n", ptype);
			argc--, argv++;
			break;
		case ('i'):
			stype = atoi(&argv[1][2]);
			printf("input score type is: %d\n", stype);
			argc--, argv++;
			break;
		default:
			fprintf(stderr, "CS11CUE: '%s' is not one of the \
options\n", argv[1]);
			phelp();
			exit(1);
		}
	}


	/* READ USER'S INPUT FILE OF CUE TIMES;  OPEN INSCORE & OUTSCORE */
	if((fp1 = fopen(argv[1], "r"))==NULL) {
		fprintf(stderr, "\nUnable to open your cuefile %s \n", argv[1]);
		phelp();
		exit(1);
	}
	argc--, argv++;	/* inscore.sc & outscore.sc now remaining on cmdline */
	
	if((cues = (double *)malloc(numcues * sizeof(double)))==NULL) {
		fprintf(stderr,"\n\tcannot allocate memory for cues array\n");
	}
	
	for(i = 0; i < numcues && eof_flag != EOF; ++i) {
		eof_flag = fscanf(fp1, "%lf", &cues[i]);
		if(eof_flag != EOF) {
			fprintf(stderr,"cue %d read as %lf\n", i, cues[i]);
		}	
	}

	initial(argc, argv);	/* re-check args & open inscore & outscore */

	if(argc < 2) {		/* Usage if not enough filenames */
		phelp();
		exit(1);
	}

	/* PROCEED TO CREATE THE OUTPUT SCORE FILE */
	a = lget();		/* read in the inscore */
	b = lsepf(a);		/* separate out the f statements */
	lput(b);		/* write f statements to outscore */
	lrelev(b);		/* release space used */
	e = defev("t 0 76");	/* define tempo event */
	putev(e);		/* write this tempo event to outscore */
/*	lput(a);		 put orig events in score for comparison */
/*	putstr("s\n");		end-of-section;  new times begin at 0 */
/*	putev(e);		write new tempo event for next section */
	n = a->nevents;		/* set n to number of events in list 'a' */
	printf("diagnostic:  n = a->nevents = %d\n", n);
	holdn = n;		/* second variable which won't change */
	i = 0;			/* counter through cue array set to 0 */
	j = 0;			/* inner loop counter */
	k = 0;			/* counter for decrementing values */

	/* THE NUMBER OF TIME CUES SPECIFIES THE NUMBER OF BLOCKS: */
	for(i = 0; i < numcues; i++) { /* numcues OUTER loops = event blocks */
		printf("\ndiagnostic:  Event block %d, time cue=%lf\n",\
i, cues[i]);
		f = &a->e[1];	/* reset pointer to first event of list a */
				/* i.e. make a new block */
		n = holdn;	/* reset n to original event count */

		/* THIS HANDLES THE EVENTS OF EACH BLOCK, CALLING FUNCTIONS: */
		for(j = 0; j < n; j++) {  /* INNER loop: adjust event pfields */
		   if(i < abs(n/2)) {
			(*f)->p[2] = stt_up((*f)->p[2],cues[i],ptype,stype,j);
			(*f)->p[4] = vol_up((*f)->p[4],ampf,ptype,stype,j);
			holdamp = (*f)->p[4];	/* hold amplitude peak */
			f++;			/* move on to next event */
		   }
			/* NEXT CODE HANDLES CRESC/DIM, WITH OVERFLOW CONTROL */
			/* NOTE THAT INCREASE ADDED TO VALS OF PREVIOUS BLOCK */
		   else {
			(*f)->p[2] = stt_up((*f)->p[2],cues[i],ptype,stype, k);
			(*f)->p[4] = vol_dwn((*f)->p[4], ampf, ptype, stype, k);
			f++;			/* move on to next event */
			k++;
		   }
		}
		sprintf(message, "\n;diagnostic: cue = %f", cues[i]);
		putstr(message); /*note method of writing diagnostic to .sc */
		lput(a);	/* write this event-list-block to outscore */
	}			/* back to OUTER FOR loop & do next block */
	putstr("e");		/* write end of score marker */
}

/* SUBROUTINES TO ADD TIME CUES TO ALL START TIMES & ALTER AMPLITUDES */
/* NB - The next time cue is added to the start time of the previous block. */

double
stt_up(double p2, double cueval, int tflag, int sflag, int counter)
/* double p2;		current p2 value (start time ) */
/* double cueval; 	current time cue value to use in calculations */
/* int	tflag;  	processing type flag */
/* int	sflag;		inscore type flag */
/* int	counter;	loop counter */
{
	if(tflag == ADD && sflag == SAME) {		/* type 0 0 */
		p2 = p2 + (counter * cueval);
	}
	if(tflag == ADD && sflag == DIFFERENT) {	/* type 0 1 */
		p2 = p2 + cueval;
	}
	if(tflag == MULTIPLY && sflag == SAME) {	/* type 1 0 */
		p2 = p2 + (counter * cueval * mult);
	}
	if(tflag == MULTIPLY && sflag == DIFFERENT) {	/* type 1 1 */
		p2 = p2 + (cueval * mult);
	}
	if(tflag == RAND && sflag == SAME) {		/* type 2 0 */
		p2 = p2 + (int)(rand()*counter*cueval);
	}
	if(tflag == RAND && sflag == DIFFERENT) {	/* type 2 1 */
		p2 = p2 + (rand()*cueval);
	}
	return(p2);
}

/* SUBROUTINES TO INCREMENT AMOUNT BY WHICH THE AMPLITUDE IS CHANGED */
/* NB - change is added to/subtracted  from the values of the previous block */

int
vol_up(int p4, int af, int tflag, int sflag, int counter)
/* int	p4;		current p4 value (amplitude) */
/* int	af;		amplitude change factor */
/* int	tflag;		processing type flag */
/* int	sflag;		inscore type flag, 0: same vals, 1: diff. vals */
/* int	counter;	event loop count */
{
	if(tflag == ADD && sflag == SAME) {		/* type 0 0 */
		p4 = p4 + (counter * af);	/* amplitude change */
	}
	if(tflag == ADD && sflag == DIFFERENT) {	/* type 0 1 */
		if(counter == 0) af = 0;		/* 1st event zero */
		p4 = p4 + af;
	}
	if(tflag == MULTIPLY && sflag == SAME) {	/* type 1 0 */
		p4 = p4 + (counter * af * mult);	/* amplitude change */
	}
	if(tflag == MULTIPLY && sflag == DIFFERENT) {	/* type 1 1 */
		if(counter == 0) af = 0;		/* 1st event zero */
		p4 = p4 + (af * mult);
	}
	if(tflag == RAND && sflag == SAME) {		/* type 2 0 */
		p4 = p4 + (int)(rand()*counter*af); /* ampl change */
	}
	if(tflag == RAND && sflag == DIFFERENT) {	/* type 2 1 */
		if(counter == 0) af = 0;		/* 1st event zero */
		p4 = p4 + (rand()*af);		/* amplitude change */
	}
	return(p4);
}

int 
vol_dwn(int p4, int af, int tflag, int sflag, int counter)
/* int	p4;		current p4 value (amplitude) */
/* int	af;		amplitude change factor */
/* int	tflag;		processing type flag */
/* int	sflag;		inscore type flag, 0: same vals, 1: diff. vals */
/* int	counter;	event loop count */
{
	if(tflag == ADD && sflag == SAME) {		/* type 0 0 */
		p4 = holdamp - (counter * af);		/* amplitude change */
	}
	if(tflag == ADD && sflag == DIFFERENT) {	/* type 0 1 */
		if(counter == 0) af = 0;		/* 1st ev no change */
		p4 = p4 - af;
	}
	if(tflag == MULTIPLY && sflag == SAME) {	/* type 1 0 */
		p4 = holdamp - (counter * af * mult);
	}
	if(tflag == MULTIPLY && sflag == DIFFERENT) {	/* type 1 1 */
		if(counter == 0) af = 0;		/* 1st ev no change */
		p4 = p4 - (af * mult);			/* subtract */
	}
	if(tflag == RAND && sflag == SAME) {		/* type 2 0 */
		p4 = p4 - (int)(rand()*counter*af);
	}
	if(tflag == RAND && sflag == DIFFERENT) {	/* type 2 1 */
		if(counter == 0) af = 0;		/* 1st ev no change */
		p4 = p4 - (rand()*af);
	}
	return(p4);
}

