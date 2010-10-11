/* instr_2.c */
/* instrument 2 ("P"-stage) */

#include <math.h>

#include <malloc.h>

#include <stdlib.h>
#include "defg.h"	/* macros, typedefs, prototypes */

int instr_2(int bat);

int instr_2(int bat)
{
	int pc1, pc2, pcdiff,
	  m1, m2, numnotes=lcount(rsco)-1, numpevs=abs(bat),
	  evnum, reach, dir, *tally1, *tally2,
	  p, q, r, j, nl=(bat>0);
	float dur, tdur, durf, pchdiff1, pchdiff2, pcsize=(float)1/PA_NPC,
	  notetime, pch1, pch2, freq1, freq2,
	  absmindur=0.025F, absminatk=0.005F, absmindec=0.0125F,
	  minatk, maxatk, mindec1, mindec2, maxdec1, maxdec2,
	  absminfreq=50.0F, absmaxfreq=18000.0F,
	  absminpch=(float)gof(PA_MINPCH-2,octcps(absminfreq)),
	  absmaxpch=(float)lof(PA_MAXPCH+2,octcps(absmaxfreq));
	  

	tally1 = malloc(PA_NPC * sizeof(int));

	tally2 = malloc(PA_NPC * sizeof(int));



	if (!tally1 || !tally2) {

		printf("Memory allocation failed.");

		exit(1);

	}


	for(q=0; q<PA_NPC; q++) tally1[q]=tally2[q]=0;  

	i2ev=&sco->e[1];
	i1ev=&rsco->e[1];
	
	if(nl) {
		printf("\n\n; \"P\"-stage, instr 2:\n\n\t");
		for(j=1; j<=NUM_I2PFIELDS; j++) printf("\t%-7s", i2pf[j]);
	}

	for(evnum=1, q=0; q<numnotes; q++, i1ev++) {
		do {
			reach=(int)(*i1ev)->p[I1_RCH];
			p=(q*PA_STEP)+2;
			if(reach<0 && abs(reach)>q) {
				numpevs-=2*(abs(reach)-q);
				reach=(-q);
			}
			if(reach>0 && q+reach>numnotes) {
				numpevs-=2*(abs(reach)-(numnotes-q));
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
		for(r=0; abs(r)<abs(reach); r+=dir) tdur+=(float)(*(i1ev+r))->p[II_DUR];
		durf=((*i1ev)->p[II_DUR])/(tdur);
		for(r=0; abs(r)<abs(reach); r+=dir, evnum++, i2ev++) {
			(*i2ev)=createv(NUM_I2PFIELDS);
			(*i2ev)->op='i';
			(*i2ev)->p[II_I]=2;
			(*i2ev)->p[II_STA]=(float)(*i1ev)->p[II_STA]+notetime+
				(.5F*(PA_DURUNIT/(reach+dir)))+(.5F*(PA_DURUNIT/(r+dir)));
			(*i2ev)->p[II_DUR]=dur=
				(float)gof(((*(i1ev+r))->p[II_DUR])*durf,absmindur);
			notetime+=(float)fabs((*i2ev)->p[II_DUR]);
			(*i2ev)->p[I2_WID1]=(float)(*(i1ev+r))->p[I1_WID2];
			(*i2ev)->p[I2_WID2]=(float)(*(i1ev+r))->p[I1_WID1];
			pch1=(float)((*(i1ev+r))->p[I1_PCH1])+pchdiff1;
			pch2=(float)((*(i1ev+r))->p[I1_PCH2])+pchdiff2;
			freq1=(float)cpsoct(pch1);
			freq2=(float)cpsoct(pch2);
			pc1=pc(quant(pch1,pcsize));
			pc2=pc(quant(pch2,pcsize));
			tally1[pc1]++;
			tally2[pc2]++;
			
			(*i2ev)->p[I2_FREQ1]=(float)fbetw(
				freq1*linscale(pc1,0,PA_NPC,0,1.6),
				absminfreq,absmaxfreq);
			(*i2ev)->p[I2_FREQ2]=(float)fbetw(
				freq2*linscale(pc2,0,PA_NPC,0,1.6),
				absminfreq,absmaxfreq);


			pcdiff=abs(pc1-pc2)+((pc1==pc2)*PA_NPC);
			(*i2ev)->p[I2_PDF1]=(float)1/(abs(pc1-pc2)+((pc1==pc2)*PA_NPC));
			(*i2ev)->p[I2_PDF2]=(float)1/(PA_NPC-pcdiff+((pcdiff==PA_NPC)*PA_NPC));

			minatk=(float)gof(absminatk, 0.05*dur);
			maxatk=5*minatk;
			m1=talmid(tally1);
			m2=talmid(tally2);
			(*i2ev)->p[I2_ATK1]=
				(float)linscale(pc1,(PA_NPC*(pc1>m1)),m1,minatk,maxatk);
			(*i2ev)->p[I2_ATK2]=
				(float)linscale(pc2,(PA_NPC*(pc2>m2)),m2,minatk,maxatk);

			mindec1=(float)gof(absmindec,
				0.5*(dur-((*i2ev)->p[I2_ATK1])));
			mindec2=(float)gof(absmindec,
				0.5*(dur-((*i2ev)->p[I2_ATK2])));
			maxdec1=(float)(0.99*((*i2ev)->p[II_DUR]))-((*i2ev)->p[I2_ATK1])-mindec1;
			maxdec2=(float)(0.99*((*i2ev)->p[II_DUR]))-((*i2ev)->p[I2_ATK2])-mindec2;
			(*i2ev)->p[I2_DEC1]=(float)linscale(tally2[pc2],
				tally2[talmax(tally2)],tally2[talmin(tally2)],
				mindec1,maxdec1);
			(*i2ev)->p[I2_DEC2]=(float)linscale(tally1[pc1],
				tally1[talmax(tally1)],tally1[talmin(tally1)],
				mindec2,maxdec2);

			(*i2ev)->p[I2_PDB1]=(float)linscale(tally1[pc1],
				tally1[talmin(tally1)],tally1[talmax(tally1)],
				PA_MINDB,PA_MAXDB);
			(*i2ev)->p[I2_PDB2]=(float)linscale(tally2[pc2],
				tally2[talmin(tally2)],tally2[talmax(tally2)],
				PA_MINDB,PA_MAXDB);
				
			(*i2ev)->p[I2_AZI1]=(float)linscale(pc1,0,PA_NPC,DEG30,DEG120);
			(*i2ev)->p[I2_AZI2]=(float)linscale(pc2,0,PA_NPC,DEG330,DEG240);
			
			(*i2ev)->p[I2_DIST1]=(float)linscale(tally1[pc1],
				tally1[talmin(tally1)],tally1[talmax(tally1)],
				PA_MINDIST,PA_MAXDIST);
			(*i2ev)->p[I2_DIST2]=(float)linscale(tally2[pc2],
				tally2[talmin(tally2)],tally2[talmax(tally2)],
				PA_MINDIST,PA_MAXDIST);
		
			if(nl) {
				printf("\nP %i/%i:\ti%.1f", evnum, numpevs,
					(*i2ev)->p[II_I]);
				for(j=2; j<=NUM_I2PFIELDS; j++)
					printf("\t%7.3f", (*i2ev)->p[j]); 
			}
		}
	}
		
	printf("\n; instr 2 finished\n");
	printf(";\n");

	return numpevs*(nl==1 ? 1 : -1);
}
