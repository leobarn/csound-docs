/* sco.c */
/* make scorefile */

#include <math.h>

#include <stdlib.h>
#include "defg.h"	/* macros, typedefs, prototypes */

void make_sco(void)
{
	int numevs, nl;

	// int q
	// float adjtime;
	EVENT **sev, **fev;

	nl=instr_4(
		instr_2(
			instr_1(
				wh("Display events as they are computed (y/[n])","ny")-1
			)
		)
	);
	sco=lcat(sco, rsco);
	lsort(sco);
	printf("\n; Score sorted in order of time of initiation.");

	printf("\n; Entering f-statements..");
	printf(".");
	f=lcreat(5);
	fev=&f->e[1];
	(*fev++)=defev("f 1 0 16385 10 1");
	(*fev)=defev("f 2 0 2049 7 0 2049 1");
	(*fev++)->p[5]=(float)ABS_MINAMPF;
	(*fev)=defev("f 3 0 2049 5 0 2049 1");
	(*fev++)->p[5]=(float)ABS_MINAMPF;
	(*fev++)=defev("f 4 0 16384 10 1");
	(*fev++)=defev("f 5 0 16384 10 1 .8 .5 .5 .15");

	numevs=lcount(sco);
	sev=&sco->e[numevs];
	printf("; (approx. duration: %.2f)\n", (*sev)->p[II_STA]+(*sev)->p[II_DUR]);
}

int talmid(int *tally)
{
	int d, mid=(int)(PA_NPC/2)-1;

	float *stally, st;

	double u;



	stally = malloc(PA_NPC * sizeof(int));



	if (!stally) {

		printf("Memory allocation failed.");

		exit(1);

	}


	for(d=0; d<PA_NPC; d++) stally[d]=tally[d]+((float)d/10);
	qsort(stally, PA_NPC, sizeof(float), comp);
	st=(float)(10*modf((double)stally[mid], &u));
	return (int)st;
}

int talmin(int *tally)
{
	int d, mpc=0;
	
	for(d=0; d<PA_NPC; d++) mpc=(tally[d]<tally[mpc] ? d : mpc);
	return mpc;
}


int talmax(int *tally)
{
	int d, mpc=0;
	
	for(d=0; d<PA_NPC; d++) mpc=(tally[d]>tally[mpc] ? d : mpc);
	return mpc;
}

int comp(const void *i, const void *j)
{
	return *(int *)i - *(int *)j;
}
	

