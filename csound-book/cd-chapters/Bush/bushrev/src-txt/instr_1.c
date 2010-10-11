/* instr_1.c */
/* instrument 1 ("R"-stage) */

#include <math.h>

#include <malloc.h>

#include <stdlib.h>
#include "defg.h"	/* macros, typedefs, prototypes */

int instr_1(int nl);

int instr_1(int nl)
{
	int q, r, j, numnotes=((PA_FULLPLEXPOI-1)/PA_STEP)-1,

		*tally1, *tally2, pcdiff, numpevs=0;
	float previnit=0.0F, prevdur=0.0F, pcsize=(float)1/PA_NPC,
	  pch1, pch2, freq1, freq2, maxdb=(float)(PA_MINDB + PA_MAXDB)/2,
	  absminatk=0.07F, absmindec=0.014F,
	  minatk, maxatk, mindec1, mindec2, maxdec1, maxdec2;
	int pc1, pc2;



	tally1 = malloc(PA_NPC * sizeof(int));

	tally2 = malloc(PA_NPC * sizeof(int));



	if (!tally1 || !tally2) {

		printf("Memory allocation failed.");

		exit(1);

	}

	for(q=0; q<PA_NPC; q++) tally1[q]=tally2[q]=0;
	rsco=lcreat(numnotes);
	i1ev=&rsco->e[1];

	printf("\n\n; NOTE-BY-NOTE READOUT:\n\n");
	if(nl) {
		printf("; \"R\"-stage, instr 1:\n\n\t");
		for(j=1; j<=NUM_I1PFIELDS; j++) printf("\t%-7s", i1pf[j]);
	}
	else printf("; (...turned off...)\n");
		
	for(q=0; q<numnotes; q++, i1ev++) {
		r=(q*PA_STEP)+2;
		(*i1ev)=createv(NUM_I1PFIELDS);
		(*i1ev)->op='i';
		(*i1ev)->p[II_I]=1;
		(*i1ev)->p[II_STA]=(float)previnit+prevdur;
		if((*i1ev)->p[II_STA]<0) (*i1ev)->p[II_STA]=0;
		(*i1ev)->p[II_DUR]=(float)fabs(fullplex[r].hd);
		previnit=(*i1ev)->p[II_STA];
		prevdur=(float)fabs((*i1ev)->p[II_DUR]);
		pch1=(float)fullplex[r].x;
		pch2=(float)fullplex[r].y;
		freq1=(float)cpsoct(pch1);
		freq2=(float)cpsoct(pch2);
		(*i1ev)->p[I1_PC1]=(float)pc1=(float)pc(pch1);
		(*i1ev)->p[I1_PC2]=(float)pc2=(float)pc(pch2);
		tally1[pc1]++;
		tally2[pc2]++;
		pcdiff=(int)fabs(pc1-pc2)+((pc1==pc2)*PA_NPC);
		(*i1ev)->p[I1_WOB1]=(float)linscale(pc1,0,PA_NPC,PA_MINWOB,PA_MAXWOB);
		(*i1ev)->p[I1_WOB2]=(float)linscale(pc2,0,PA_NPC,PA_MINWOB,PA_MAXWOB);
		(*i1ev)->p[I1_PCH1]=(float)pch1;
		(*i1ev)->p[I1_PCH2]=(float)pch2;
		(*i1ev)->p[I1_PDF1]=(float)1/pcdiff;
		(*i1ev)->p[I1_PDF2]=
			(float)1/(PA_NPC-pcdiff+((pcdiff==PA_NPC)*PA_NPC));
		(*i1ev)->p[I1_WID1]=
			(float)(freq1-((pc1+1)*25)>25 ? (pc1+1)*50 : 50);
		(*i1ev)->p[I1_WID2]=
			(float)(freq2-((pc2+1)*25)>25 ? (pc2+1)*50 : 50);

		minatk=(float)gof(absminatk, 0.05*((*i1ev)->p[II_DUR]));
		maxatk=7*minatk;
		(*i1ev)->p[I1_ATK1]=(float)linscale(tally1[pc1],
			tally1[talmax(tally1)],tally1[talmin(tally1)],minatk,maxatk);
		(*i1ev)->p[I1_ATK2]=(float)linscale(tally2[pc2],
			tally2[talmax(tally2)],tally2[talmin(tally2)],minatk,maxatk);

		mindec1=(float)gof(absmindec,
				0.2*(((*i1ev)->p[II_DUR])-((*i1ev)->p[I1_ATK1])));
		mindec2=(float)gof(absmindec,
				0.2*(((*i1ev)->p[II_DUR])-((*i1ev)->p[I1_ATK2])));
		maxdec1=(float)(0.99*((*i1ev)->p[II_DUR]))-((*i1ev)->p[I1_ATK1])-mindec1;
		maxdec2=(float)(0.99*((*i1ev)->p[II_DUR]))-((*i1ev)->p[I1_ATK2])-mindec2;
		(*i1ev)->p[I1_DEC1]=(float)linscale(tally2[pc2],
			tally2[talmax(tally2)],tally2[talmin(tally2)],mindec1,maxdec1);
		(*i1ev)->p[I1_DEC2]=(float)linscale(tally1[pc1],
			tally1[talmax(tally1)],tally1[talmin(tally1)],mindec2,maxdec2);

		(*i1ev)->p[I1_PDB1]=(float)linscale(tally1[pc1],
			tally1[talmin(tally1)], tally1[talmax(tally1)],
			PA_MINDB, maxdb);
		(*i1ev)->p[I1_PDB2]=(float)linscale(tally2[pc2],
			tally2[talmin(tally2)], tally2[talmax(tally2)],
			PA_MINDB, maxdb);
		
		(*i1ev)->p[I1_ILOC1]=(float)linscale(tally1[pc1],
			tally1[talmin(tally1)], tally1[talmax(tally1)], 0, 0.5);
		(*i1ev)->p[I1_ELOC1]=(float)linscale(tally2[pc2],
			tally2[talmin(tally2)], tally2[talmax(tally2)], 0.75, 0.25);
		(*i1ev)->p[I1_ILOC2]=(float)linscale(tally2[pc2],
			tally2[talmin(tally1)], tally2[talmax(tally1)], 0.75, 0.25);
		(*i1ev)->p[I1_ELOC2]=(float)linscale(tally1[pc1],
			tally1[talmin(tally1)], tally1[talmax(tally1)], 0.5, 1);
		
		(*i1ev)->p[I1_RCH]=(float)fullplex[r].hr;
		numpevs+=abs((int)fullplex[r].hr);
		
		if(nl) {
			printf("\nR %i/%i:\ti1", q+1, numnotes);
			for(j=2; j<=NUM_I1PFIELDS; j++) printf("\t%7.3f", (*i1ev)->p[j]);
		}
	}
	printf("\n; instr 1 finished\n; -- end of \"R\"-stage --\n");
	numpevs*=2;
	sco=lcreat(numpevs);
	return numpevs*(nl==1 ? 1 : -1);
}
