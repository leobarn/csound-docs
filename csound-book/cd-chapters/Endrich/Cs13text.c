/* CS13TEXT.C - create a sound texture, with user inputs & processing options
 * 		CS13TEXT  V0.7TT, a Cscore program by A Endrich 17 Aug 1995
 *
 * 24 Dec 1997 - Atari drand48() replaced by rand() for use on PC's
 * 
 * Program to build a controlled texture of note events with expand-compress.
 * This version uses four change factors for four parameters (start time,
 *	duration, amplitude and pitch), though with appropriately different
 *	functions for each.
 * The pitch change factor is a frequency in Hz;  NB - it is NOT a ratio.
 * Starting point is an inscore.sc.:
 *	number of events is read from the inscore.sc;  there can be 2 types: 
 *	  type 0: an inscore with all = values for each of the 4 key params;
 *		start times are usually all 0;
 *	  	values expand & compress from these starting values;
 *		note that low values & high change factors may go below
 *		    zero;  the program doesn't prevent this: check the score!
 *	  type 1: an inscore with different values;  same warnings as above
 * Note how the logic & arithmetic of the functions differs for these two
 *	types of inscores.
 * Further development planned:  to combine with the time cues program, and
 *	implement more functions with which to massage the values.
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include  "cscore.h"

/* DECLARE #DEFINES */
#define	SAME	   0
#define	DIFFERENT  1
#define	FIXED	   0
#define	RAND	   1
#define	LIN_SERIES 2
#define	LOG_SERIES 3
#define EXP_SERIES 4

/* DECLARE USER-DEFINED FUNCTIONS */
/* name(current_pval,ppval, change_factor, process_type, score_type, counter) */

void   phelp(void);
double stt_up(double, double, double, int, int, int);   /* start times incr'g */
double dur_up(double, double, double, int, int, int);	/* durs increasing */
int    amp_up(int, int, int, int, int, int);		/* amps increasing */
double frq_up(double, double, double, int, int, int);	/* freqs increasing */
double stt_dwn(double, double, double, int, int, int);	/* start times decr'g */
double dur_dwn(double, double, double, int, int, int);	/* durs decreasing */
int    amp_dwn(int, int, int, int, int, int);		/* amps decreasing */
double frq_dwn(double, double, double, int, int, int);	/* freqs decreasing */

/* DECLARE GLOBAL VARIABLES */
double	holddur;	/* to hold high dur for case FIXED SAME down */
int	holdamp;	/* to hold high amp for case FIXED SAME down */
double	holdfrq;	/* to hold high freq for case FIXED SAME down */

/* USAGE MESSAGE */
void
phelp(void) {
	printf("CS13TEXT- create an event texture with density controls\n");
	printf("Usage:  cs13text   -sstartf -ddurf -aampf -ffreqf\n");
	printf("\t\t-ttype -iscoreflag inscore outscore.sc \n");
	printf(" -sstartf  (double) start time change per event (Default 0)\n");
	printf(" -ddurf    (double) duration change per event (Default 1)\n");
	printf(" -aamplf   (int) amplitude change per event (e.g 1000)\n");
	printf(" -ffreqf   (double) freq in Hz by which to change frequency\n");
	printf(" -ttype	   (int) texture processing algorithm type:\n");
	printf("\t\t0 - FIXED change \n");
	printf("\t\t1 - RAND change \n");
	printf("\t\t2 - LINEAR series: (count*change)+current_value=newval\n");
	printf("\t\t3 - LOGARITHMIC series (not implemented)\n");
	printf("\t\t4 - EXPONENTIAL series (not implemented) \n");
	printf(" -iscoreflag\ttype of input scorefile: \n");
	printf("\t\t0 - inscore is template with same values for ea. event\n"); 
	printf("\t\t1 - inscore is a 'real' score with varying values\n");
	printf(" inscore  name of input scorefile\n");
	printf(" outscore name of output scorefile\n");
	exit(1);
}

/* MAIN() */
void
main(int argc, char *argv[])
int	argc;	    /* counter for commandline arguments */
char	*argv[];    /* string array for commandline arguments */
{

	EVENT	*e, **f;	/* declare structure an event (incl pfields) */
	EVLIST  *a, *b;		/* declare structure for your list of events */

	/* any other variables to be used need to go here */
	int	i = 0, j = 0;	/* loop counters for increase & decrease */
	int	numev;		/* number of events in inscore  */
	int	half;		/* counter for two halves of the texture */
	int	type;		/* texture processing type: 0, 1, 2, 3, 4 ... */
	int	scoreflag;	/* inscore type: 0 (same vals), 1 (diff vals) */
	int	amplf;		/* amplitude change per event (0-32767 scale) */
	double  startf;		/* start time change per event in sec. */
	double  durf;		/* duration change per event in sec. */
	double	freqf;		/* frequency change per event in Hz */
	double	hpp2;		/* to hold previous p2 val: case LINEAR SAME */
	double	hpp3;		/* to hold previous p3 val: case LINEAR SAME */
	double	hpp4;		/* to hold previous p4 val: case LINEAR SAME */
	double	hpp5;		/* to hold previous p5 val: case LINEAR SAME */

	/* READ INPUTS */
	while(argc > 1 && argv[1][0] == '-') {
		switch(argv[1][1]) {		/* loop to pick up the flags */
		case ('s'):
			startf = atof(&argv[1][2]); /* start time change */
			printf("start time factor: %f\n", startf);
			break;
		case ('d'):
			durf = atof(&argv[1][2]); /* duration change */
			printf("duration factor: %f\n", durf);
			break;
		case ('a'):
			amplf = atoi(&argv[1][2]); /* amplitude change */
			printf("amplitude scaling factor: %d\n", amplf);
			break;
		case ('f'):
			freqf = atof(&argv[1][2]); /* frequency change */
			printf("frequency scaling factor: %f\n", freqf);
			break;
		case ('t'):
			type = atoi(&argv[1][2]);  /* processing type */
			printf("processing type: %d\n", type);
			break;
		case ('i'):
			scoreflag = atoi(&argv[1][2]);  /* inscore type */
			printf("inscore type: %d\n", scoreflag);
			break;
		default:
			fprintf(stderr, "CS13TEXT: foxed by '%s'\n", argv[1]);
			phelp();
			exit(1);
		}
		argc--, argv++;	/* decrement counter, move to next argument */
	}

	if(argc < 2) {		/* remaining args should be in/outfile */
		phelp();	/* call phelp() for messages */
		exit(1);
	}

	initial(argc, argv);  /* check args, open in/out scorefiles */


	/* CREATE OUTPUT SCORE FILE */
	putstr(";What follows is the Csound score written by CS13TEXT\n");
	a = lget();		/* read the inscore.sc */
	b = lsepf(a);		/* separate out the f statement(s) */
	lput(b);
	lrelev(b);
	e = defev("t 0 60");	/* define a tempo statement event */
	putev(e);		/* write this event to the score */
	numev = a->nevents;	/* set event counter */
	fprintf(stderr,"diagnostic: nevents = %d\n", numev);
	half = numev/2;		/* halfway point of texture */

	f = &a->e[1];		/* set ptr to remaining 'i' events in 'a' */
	hpp2 = 0;		/* make sure previous p[2] is set to 0 */
	hpp3 = (*f)->p[3];	/* set 'hold_previous_pfield' to start */
	hpp4 = (*f)->p[4];	/* set 'hold_previous_pfield' to start */
	hpp5 = (*f)->p[5];	/* set 'hold_previous_pfield' to start */
	j = 0;			/* set decrementing counter */

	/* CALCULATE CHANGES TO SCORE DATA AND WRITE INTO OUTSCORE.SC */
	for(i = 0; i < numev; i++) {
	  if(i <= abs(numev/2)) {
				/* 1st half: expand values */
		(*f)->p[2] = stt_up((*f)->p[2],hpp2,startf, type, scoreflag, i);
		(*f)->p[3] = dur_up((*f)->p[3],hpp3, durf, type, scoreflag, i);
		(*f)->p[4] = amp_up((*f)->p[4],hpp4, amplf, type, scoreflag, i);
		(*f)->p[5] = frq_up((*f)->p[5],hpp5, freqf, type, scoreflag, i);
		hpp2	= (*f)->p[2];	/* hold previous p[2] */
		hpp3	= (*f)->p[3];	/* hold previous p[3] */
		hpp4	= (*f)->p[4];	/* hold previous p[4] */
		hpp5	= (*f)->p[5];	/* hold previous p[5] */	
		holddur = (*f)->p[3];	/* hold highest p[3] val - global */
		holdamp = (*f)->p[4];	/* hold highest p[4] val - global */
		holdfrq = (*f)->p[5];	/* hold highest p[5] val - global */
		f++;				/* move on to next event */
	  }
	  else {
				/* 2nd half: compress values */
				/* start times keep rising! */
		(*f)->p[2] = stt_up((*f)->p[2],hpp2,startf, type, scoreflag, i);
		(*f)->p[3] = dur_dwn((*f)->p[3],hpp3, durf, type, scoreflag, j);
		(*f)->p[4] = amp_dwn((*f)->p[4],hpp4,amplf, type, scoreflag, j);
		(*f)->p[5] = frq_dwn((*f)->p[5],hpp5, freqf, type, scoreflag, j);
		hpp2	= (*f)->p[2];	/* hold previous p[2] */
		hpp3	= (*f)->p[3];	/* hold previous p[3] */
		hpp4	= (*f)->p[4];	/* hold previous p[4] */
		hpp5	= (*f)->p[5];	/* hold previous p[5] */
		f++;				/* move on to next event */
		j++;				/* decrement counter */
  	  }

	}  /* end of main score processing FOR loop */

	a = lcat(b,a);		/* join altered 'a' events to 'b' f event(s) */
	lput(a);		/* write complete scorefile to outscore.sc */
	lrelev(a);		/* release memory space */
	putstr("e");		/* write end-of-score marker */
}				/* end of main() */

/* DEFINE USER-DEFINED FUNCTIONS */
double
stt_up(double p2, double hp2, double factor, int typeflag, int scoretype, \
int counter)  /* process start times */
/* double  p2;		receive current start time in p2 */
/* double  hp2;		held previous p2 start time */
/* double  factor;	user's change factor for start times */
/* int	typeflag;	type flag */	
/* int	scoretype;	scoretype flag */
/* int	counter;	loop number as in main() */
{
	if(typeflag == FIXED && scoretype == SAME) {		/* type 0 0 */
		p2 = p2 + (counter * factor);		/* sum fixed value */
	}
	if(typeflag == FIXED && scoretype == DIFFERENT) {	/* type 0 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p2 = p2 + factor;			/* add fixed value */
	}
	if(typeflag == RAND && scoretype == SAME) {		/* type 1 0 */
		p2 = p2 + (rand48*counter*factor);
	}
	if(typeflag == RAND && scoretype == DIFFERENT) {	/* type 1 1 */
		if(counter == 0) factor = 0;		   /* 0 for 1st loop */
		p2 = p2 + (rand()*factor);
	}
	if(typeflag == LIN_SERIES && scoretype == SAME) {	/* type 2 0 */
		p2 = hp2 + (counter * factor); /* work from previous p2 value */
	}
	if(typeflag == LIN_SERIES && scoretype == DIFFERENT) {	/* type 2 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p2 = p2 + (counter * factor);		/* add expanding value*/
	}
	if(typeflag == LOG_SERIES && scoretype == SAME) {	/* type 3 0 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == LOG_SERIES && scoretype == DIFFERENT) {  /* type 3 1 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == SAME) {	/* type 4 0 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == DIFFERENT) {  /* type 4 1 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p2);
}


double
dur_up(double p3, double hp3, double factor, int typeflag, int scoretype, \
int counter)	/* process amplitudes */
/* double  p3;		receive current duration in p3 */
/* double  hp3;		held previous p3 duration */
/* double  factor;	user's duration change factor */
/* int	typeflag;	type flag */
/* int	scoretype;	inscore type */
/* int	counter;	loop number */
{
	if(typeflag == FIXED && scoretype == SAME) {		/* type 0 0 */
		p3 = p3 + (counter * factor);		/* sum fixed value */
	}
	if(typeflag == FIXED && scoretype == DIFFERENT) {	/* type 0 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p3 = p3 + factor;			/* add fixed value */
	}
	if(typeflag == RAND && scoretype == SAME) {		/* type 1 0 */
		p3 = p3 + (rand()*counter*factor);
	}
	if(typeflag == RAND && scoretype == DIFFERENT) {	/* type 1 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p3 = p3 + (rand()*factor);
	}
	if(typeflag == LIN_SERIES && scoretype == SAME) {	/* type 2 0 */
		p3 = hp3 + (counter * factor); /* work from previous p3 value */
	}
	if(typeflag == LIN_SERIES && scoretype == DIFFERENT) {	/* type 2 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p3 = p3 + (counter * factor);		/* add expanding value*/
	}
	if(typeflag == LOG_SERIES && scoretype == SAME) {	/* type 3 0 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == LOG_SERIES && scoretype == DIFFERENT) {	/* type 3 1 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == SAME) {	/* type 4 0 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == DIFFERENT) {	/* type 4 1 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p3);
}

int 
amp_up(int p4, int hp4, int factor, int typeflag, int scoretype, int counter) \
	/* process amplitudes */
/* int	p4;		receive current amplitude in p4 */
/* int	hp4;		held previous p4 value */
/* int	factor;		user's amplitude change factor */
/* int	typeflag;	type flag */
/* int	scoretype;	inscore type */
/* int	counter;	loop number */
{
	if(typeflag == FIXED && scoretype == SAME) {		/* type 0 0 */
		p4 = p4 + (counter * factor);		/* sum fixed value */
	}
	if(typeflag == FIXED && scoretype == DIFFERENT) {	/* type 0 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p4 = p4 + factor;			/* add fixed value */
	}
	if(typeflag == RAND && scoretype == SAME) {		/* type 1 0 */
		p4 = p4 + (int)(rand()*counter*factor);
	}
	if(typeflag == RAND && scoretype == DIFFERENT) {	/* type 1 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p4 = p4 + (int)(rand()*factor);
	}
	if(typeflag == LIN_SERIES && scoretype == SAME) {	/* type 2 0 */
		p4 = hp4 + (counter * factor); /* work from previous p4 value */
	}
	if(typeflag == LIN_SERIES && scoretype == DIFFERENT) {  /* type 2 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p4 = p4 + (counter * factor);		/* add expanding value*/
	}
	if(typeflag == LOG_SERIES && scoretype == SAME) {	/* type 3 0 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == LOG_SERIES && scoretype == DIFFERENT) {  /* type 3 1 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == SAME) {	/* type 4 0 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == DIFFERENT) {  /* type 4 1 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p4);
}

double 
frq_upp(double p5, double hp5, double factor, int typeflag, int scoretype, \
int counter)  /* process frequencies */
/* double  p5;		receive current frequency in p5 */
/* double  hp5;		held previous p5 frequency */
/* double  factor;	user's frequency change factor in Hz */
/* int	typeflag;	type flag */	
/* int	scoretype;	inscore type */
/* int	counter;	number of loop */
{
	if(typeflag == FIXED && scoretype == SAME) {		/* type 0 0 */
		p5 = p5 + (counter * factor);		/* add n factors */
	}
	if(typeflag == FIXED && scoretype == DIFFERENT) {	/* type 0 1 */
		if(counter == 0) factor = 0;	     /* 1st loop no change! */
		p5 = p5 + factor;			/* add fixed value */
	}
	if(typeflag == RAND && scoretype == SAME) {		/* type 1 0 */
		p5 = p5 + (rand()*counter*factor);
	}
	if(typeflag == RAND && scoretype == DIFFERENT) {	/* type 1 1 */
		if(counter == 0) factor = 0;	     /* 1st loop no change! */
		p5 = p5 + (rand()*factor);
	}
	if(typeflag == LIN_SERIES && scoretype == SAME) {	/* type 2 0 */
		p5 = hp5 + (counter * factor); /* work from previous p5 value */
	}
	if(typeflag == LIN_SERIES && scoretype == DIFFERENT) {	/* type 2 1 */
		if(counter == 0) factor = 0;		/* 0 for 1st loop */
		p5 = p5 + (counter * factor);		/* add expanding value*/
	}
	if(typeflag == LOG_SERIES && scoretype == SAME) {	/* type 3 0 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == LOG_SERIES && scoretype == DIFFERENT) {	/* type 3 1 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == SAME) {	/* type 4 0 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == DIFFERENT) {	/* type 4 1 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p5);
}


double		/* this function probably never used: st's need to sum! */
stt_dwn(double p2, double hp2, double factor, int typeflag, int scoretype, \
int counter) /* process start times */
/* double  p2;		receive current start time in p2 */
/* double  hp2;		held previous p2 start time */
/* double  factor;	user's change factor for start times */
/* int	typeflag;	type flag */	
/* int	scoretype;	scoretype flag */
/* int	counter;	loop number as in main() */
{
	if(typeflag == FIXED && scoretype == SAME) {		/* type 0 0 */
		p2 = hp2 - (counter * factor);		/* sum fixed value */
	}
	if(typeflag == FIXED && scoretype == DIFFERENT) {	/* type 0 1 */
		p2 = p2 - factor;		     /* subtract fixed value */
	}
	if(typeflag == RAND && scoretype == SAME) {		/* type 1 0 */
		p2 = p2 - (rand()*counter*factor);
	}
	if(typeflag == RAND && scoretype == DIFFERENT) {	/* type 1 1 */
		p2 = p2 - (rand()*factor);
	}
	if(typeflag == LIN_SERIES && scoretype == SAME) {	/* type 2 0 */
		p2 = hp2 - (counter * factor); /* work from previous p2 value */
	}
	if(typeflag == LIN_SERIES && scoretype == DIFFERENT) {	/* type 2 1 */
		p2 = p2 - (counter * factor);	/* subtract expanding value */
	}
	if(typeflag == LOG_SERIES && scoretype == SAME) {	/* type 3 0 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == LOG_SERIES && scoretype == DIFFERENT) {	/* type 3 1 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == SAME) {	/* type 4 0 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == DIFFERENT) {	/* type 4 1 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p2);
}

double 
dur_dwn(double p3, double hp3, double factor, int typeflag, int scoretype, \
int counter)	/* process durations */
/* double  p3;		receive current duration in p3 */
/* double  hp3;		held previous p3 duration value */
/* double  factor;	user's duration change factor */
/* int	typeflag;	type flag */
/* int	scoretype;	inscore type */
/* int	counter;	loop number */
{
	if(typeflag == FIXED && scoretype == SAME) {		/* type 0 0 */
		p3 = holddur - (counter * factor);  /* highpt less n factors */
	}
	if(typeflag == FIXED && scoretype == DIFFERENT) {	/* type 0 1 */
		p3 = p3 - factor;		      /* subtract fixed value */
	}
	if(typeflag == RAND && scoretype == SAME) {		/* type 1 0 */
		p3 = p3 - (rand()*counter*factor);
	}
	if(typeflag == RAND && scoretype == DIFFERENT) {	/* type 1 1 */
		p3 = p3 - (rand()*factor);
	}
	if(typeflag == LIN_SERIES && scoretype == SAME) {	/* type 2 0 */
		p3 = hp3 - (counter * factor); /* work from previous p3 value */
	}
	if(typeflag == LIN_SERIES && scoretype == DIFFERENT) {	/* type 2 1 */
		p3 = p3 - (counter * factor);	/* subtract expanding value */
	}
	if(typeflag == LOG_SERIES && scoretype == SAME) {	/* type 3 0 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == LOG_SERIES && scoretype == DIFFERENT) {	/* type 3 1 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == SAME) {	/* type 4 0 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == DIFFERENT) {  /* type 4 1 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p3);
}

int 
amp_dwnp(int p4, int hp4, int factor, int typeflag, int scoretype, \
int counter)	/* process amplitudes */
/* int	p4;		receive current amplitude in p4 */
/* int	hp4;		held previous p4 value */
/* int	factor;		user's amplitude change factor */
/* int	typeflag;	type flag */
/* int	scoretype;	inscore type */
/* int	counter;	loop number */
{
	if(typeflag == FIXED && scoretype == SAME) {	   /* type 0 0 */
		p4 = holdamp - (counter * factor);   /* highpt less n factors */
	}
	if(typeflag == FIXED && scoretype == DIFFERENT) {  /* type 0 1 */
		p4 = p4 - factor;		     /* subtract fixed value */
	}
	if(typeflag == RAND && scoretype == SAME) {		/* type 1 0 */
		p4 = p4 - (int)(rand()*counter*factor);
	}
	if(typeflag == RAND && scoretype == DIFFERENT) {	/* type 1 1 */
		p4 = p4 - (int)(rand()*factor);
	}
	if(typeflag == LIN_SERIES && scoretype == SAME) {	/* type 2 0 */
		p4 = hp4 - (counter * factor);   /* work from previous p4 val */
	}
	if(typeflag == LIN_SERIES && scoretype == DIFFERENT) {  /* type 2 1 */
		p4 = p4 - (counter * factor);	 /* subtract expanding value */
	}
	if(typeflag == LOG_SERIES && scoretype == SAME) {	/* type 3 0 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == LOG_SERIES && scoretype == DIFFERENT) {  /* type 3 1 */
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == SAME) {	/* type 4  0 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == DIFFERENT) {  /* type 4 1 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p4);
}

double 
frq_dwn(double p5, double hp5, double factor, int typeflag, int scoretype, \
int counter)  /* process freqs */
/* double  p5;		receive current frequency in p5 */
/* double  hp5;		held previous p5 value */
/* double  factor;	user's frequency change factor in Hz */
/* int	typeflag;	type flag */	
/* int	scoretype;	inscore type */
/* int	counter;	number of loop */
{
	if(typeflag == FIXED && scoretype == SAME) {		/* type 0 0 */
		p5 = holdfrq - (counter * factor);  /* highpt less n factors */
	}
	if(typeflag == FIXED && scoretype == DIFFERENT) {	/* type 0 1 */
		p5 = p5 - factor;		      /* subtract fixed value */
	}
	if(typeflag == RAND && scoretype == SAME) {		/* type 1 0 */
		p5 = p5 - (rand()*counter*factor);
	}
	if(typeflag == RAND && scoretype == DIFFERENT) {	/* type 1 1 */
		p5 = p5 - (rand()*factor);
	}
	if(typeflag == LIN_SERIES && scoretype == SAME) {	/* type 2 0 */
		p5 = hp5 - (counter * factor);   /* work from previous p5 val */
	}
	if(typeflag == LIN_SERIES && scoretype == DIFFERENT) {  /* type 2 1 */
		p5 = p5 - (counter * factor);	  /* subtract expanding value */
	}
	if(typeflag == LOG_SERIES && scoretype == SAME) {	/* type 3 */
		fprintf(stderr, "logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == LOG_SERIES && scoretype == DIFFERENT) {
		printf("logarithmic not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == SAME) {	/* type 4 */
		printf("exponential not implemented, s_t unchanged\n");
	}
	if(typeflag == EXP_SERIES && scoretype == DIFFERENT) {
		printf("exponential not implemented, s_t unchanged\n");
	}
	return(p5);
}
