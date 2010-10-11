/* instr_4.c */
/* instrument 4 ("P"-stage) */

#include <math.h>

#include <malloc.h>

#include <stdlib.h>
#include "defg.h"	/* macros, typedefs */

int instr_4(int bat);

int instr_4(int bat)
{
	int pc1, pc2, *tally1, *tally2,
	  numnotes=lcount(rsco)-1, numpevs=abs(bat),
	  beg=lcount(sco)+1, evnum, reach, dir,
	  p, q, r, j, nl=(bat>0);
	float tdur, durf, pchdiff1, pchdiff2, pcsize=(float)1/PA_NPC,
	  notetime,
	  absmindur=0.025F, absminatk=0.007F, absmindec=0.014F,
	  minatk, maxatk, mindec1, mindec2, maxdec1, maxdec2,
	  absminpch=(float)gof(PA_MINPCH-2,octcps(ABS_MINFREQ)),
	  absmaxpch=(float)lof(PA_MAXPCH+2,octcps(ABS_MAXFREQ));
	  

	tally1 = malloc(PA_NPC * sizeof(int));

	tally2 = malloc(PA_NPC * sizeof(int));



	if (!tally1 || !tally2) {

		printf("Memory allocation failed.");

		exit(1);

	}


	for(q=0; q<PA_NPC; q++) tally1[q]=tally2[q]=0;  

	i4ev=&sco->e[beg];
	i1ev=&rsco->e[1];
	
	if(nl) {
		printf("\n\n; \"P\"-stage, instr 4:\n\n\t");
		for(j=1; j<=NUM_I4PFIELDS; j++) printf("\t%-7s", i4pf[j]);
	}
		
	for(evnum=beg, q=0; q<numnotes; q++, i1ev++) {
		do {
			reach=(int)(*i1ev)->p[I1_RCH];
			p=(q*PA_STEP)+2;
			if(reach<0 && abs(reach)>q) {
				numpevs-=(abs(reach)-q);
				reach=(-q);
			}
			if(reach>0 && q+reach>numnotes) {
				numpevs-=(abs(reach)-(numnotes-q));
				reach=numnotes-q;
			}
			if(reach==0) q++, i1ev++;
		} while(reach==0);
		
		pchdiff1=
			(float)((*i1ev)->p[I1_PCH1])-((*(i1ev+reach))->p[I1_PCH1]);
		pchdiff2=
			(float)((*i1ev)->p[I1_PCH2])-((*(i1ev+reach))->p[I1_PCH2]);
		dir=SIGN(reach);
		notetime=tdur=0;
		for(r=0; abs(r)<abs(reach); r+=dir) tdur+=(*(i1ev+r))->p[II_DUR];
		durf=(float)((*i1ev)->p[II_DUR])/(tdur);
		for(r=0; abs(r)<abs(reach); r+=dir, evnum++, i4ev++) {
			(*i4ev)=createv(NUM_I4PFIELDS);
			(*i4ev)->op='i';
			(*i4ev)->p[II_I]=4;
			(*i4ev)->p[II_STA]=(float)(*i1ev)->p[II_STA]+notetime+
				(.5F*(PA_DURUNIT/(reach+dir)))+(.5F*(PA_DURUNIT/(r+dir)));
			(*i4ev)->p[II_DUR]=(float)gof(((*(i1ev+r))->p[II_DUR])*durf,absmindur);
			(*i4ev)->p[I4_WOB1]=(float)(*(i1ev+r))->p[I1_WOB2];
			(*i4ev)->p[I4_WOB2]=(float)(*(i1ev+r))->p[I1_WOB1];
			(*i4ev)->p[I4_WID1]=(float)(*(i1ev+r))->p[I1_WID2];
			(*i4ev)->p[I4_WID2]=(float)(*(i1ev+r))->p[I1_WID1];
			(*i4ev)->p[I4_PCH1]=(float)quant(fbetw(
				((*(i1ev+r))->p[I1_PCH1])+pchdiff2,
				absminpch,absmaxpch),pcsize);
			(*i4ev)->p[I4_PCH2]=(float)quant(fbetw(
				((*(i1ev+r))->p[I1_PCH2])+pchdiff1,
				absminpch,absmaxpch),pcsize);
			(*i4ev)->p[I4_PC1]=(float)pc1=(float)pc((*i4ev)->p[I4_PCH2]);
			(*i4ev)->p[I4_PC2]=(float)pc2=(float)pc((*i4ev)->p[I4_PCH1]);
			notetime+=(float)(20/(pc1+pc2+2))*((*i4ev)->p[II_DUR]);
			tally1[pc1]++;
			tally2[pc2]++;

			(*i4ev)->p[I4_PDF1]=(float)(*(i1ev+r))->p[I1_PDF1];
			(*i4ev)->p[I4_PDF2]=(float)(*(i1ev+r))->p[I1_PDF2];

			minatk=(float)gof(absminatk, 0.1*((*i4ev)->p[II_DUR]));
			maxatk=5*minatk;
			(*i4ev)->p[I4_ATK1]=(float)linscale(tally1[pc1],
				tally1[talmax(tally1)],tally1[talmin(tally1)],
				absminatk,maxatk);
			(*i4ev)->p[I4_ATK2]=(float)linscale(tally2[pc2],
				tally2[talmax(tally2)],tally2[talmin(tally2)],
				absminatk,maxatk);

			mindec1=(float)gof(absmindec,
				0.3*(((*i4ev)->p[II_DUR])-((*i4ev)->p[I4_ATK1])));
			mindec2=(float)gof(absmindec,
				0.3*(((*i4ev)->p[II_DUR])-((*i4ev)->p[I4_ATK2])));
			maxdec1=(float)(0.99*((*i4ev)->p[II_DUR]))-((*i4ev)->p[I4_ATK1])-mindec1;
			maxdec2=(float)(0.99*((*i4ev)->p[II_DUR]))-((*i4ev)->p[I4_ATK2])-mindec2;
			(*i4ev)->p[I4_DEC1]=(float)linscale(tally2[pc2],
				tally2[talmax(tally2)],tally2[talmin(tally2)],
				mindec1,maxdec1);
			(*i4ev)->p[I4_DEC2]=(float)linscale(tally1[pc1],
				tally1[talmax(tally1)],tally1[talmin(tally1)],
				mindec2,maxdec2);

			(*i4ev)->p[I4_PDB1]=(float)linscale(tally1[pc1],
				tally1[talmin(tally1)],tally1[talmax(tally1)],
				PA_MINDB,PA_MAXDB);
			(*i4ev)->p[I4_PDB2]=(float)linscale(tally2[pc2],
				tally2[talmin(tally2)],tally2[talmax(tally2)],
				PA_MINDB,PA_MAXDB);
				
			(*i4ev)->p[I4_AZI1]=(float)((fullplex[p].hr>0)*3.1416F)-
				(float)linscale(tally1[pc1],tally1[talmin(tally1)],
				tally1[talmax(tally1)],DEG300,DEG210);
			(*i4ev)->p[I4_AZI2]=(float)((fullplex[p].hr>0)*3.1416F)-
				(float)linscale(tally2[pc2],tally2[talmax(tally2)],
				tally2[talmin(tally2)],DEG60,DEG150);
				
			(*i4ev)->p[I4_DIST1]=(float)linscale(pc1,0,PA_NPC,PA_MINDIST,PA_MAXDIST);
			(*i4ev)->p[I4_DIST2]=(float)linscale(pc2,0,PA_NPC,PA_MINDIST,PA_MAXDIST);

			
			if(nl) {
				printf("\nP %i/%i:\ti%.1f", evnum, numpevs, (*i4ev)->p[II_I]);
				for(j=2; j<=NUM_I4PFIELDS; j++)
					printf("\t%7.3f", (*i4ev)->p[j]); 
			}
		}
	}
	printf("\n; instr 4 finished\n; -- end of \"P\"-stage --\n");
	printf(";\n");

	return nl;
}
