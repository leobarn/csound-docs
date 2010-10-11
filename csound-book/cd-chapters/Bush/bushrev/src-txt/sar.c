/* sar.c */
/* store and retrieve datafields */

#include <stdio.h>
#include <string.h>
/* #include <sys/param.h> */
#include "defg.h"	/* macros, typedefs */

void load(int what)
{
	FILE *fp;
	int i, np=PA_NUMPARAMS, spp=PA_SPECPOI, tpp=PA_FULLPLEXPOI;
	char filename[MAX_PATHLEN+MAX_FNLEN], *fn;
	
	if(what>=ASCF) {
		printf("\n; This program doesn't load that type of datafield.\n");
		getack();
		return;
	}
	strcpy(filename, PA_PATH);
	strcat(filename, get_filename(what, BIN));
	fn=filename;
		
	if((fp=tryopen(fp, fn, "rb"))==NULL) return;

	init(what, 0);
	init(PARS, ALL);

	if(what==MINSPEC)
		if(wh("Does file have alternate number of parameters in header (y/[n])", "ny")==2) {
			printf("; How many? ");
			scanf("%i", &np);
		}

	for(i=0; i<np; i++) {
		if(fread(&dpa[i], sizeof(pars), 1, fp)!=1) {
			if(feof(fp)) break;
			printf("\n; error reading parameters for %s\n", filename);
			getack();
			return;
		}
	}
	
	PA_NUMPARAMS=dpa[0].i;
	PA_SPECPOI=spp;
	PA_FULLPLEXPOI=tpp;
						
	if(what==MINSPEC) {
		if((PA_SPECPOI=np=readlist(fp, fn, specxy, 0, MAX_POI))==0) return;
		GOT_MINSPEC=YES;
		copy_specxy();
	}

	if(what==SPECPLEX) {
		if((PA_SPECPOI=np=readplex(fp, fn, specplex, 0, MAX_POI))==0) return;
		GOT_SPECPLEX=YES;
	}
			
	if(what==FULLPLEX) {
		if((PA_FULLPLEXPOI=np=readplex(fp, fn, fullplex, 0, MAX_POI))==0) return;
		GOT_FULLPLEX=YES;
	}

	fclose(fp);
	printf("\n; Successfully loaded %s.\n", fn);
	if(PA_FULLPLEXPOI>0) getmaxmin(fullplex, PA_FULLPLEXPOI);
	else getmaxmin(specplex, PA_SPECPOI);
	if(what!=PARS)
		printf("; (%i points: xmax=%.2f, xmin=%.2f, ymax=%.2f, ymin=%.2f)\n",
		np, PA_XMAX, PA_XMIN, PA_YMAX, PA_YMIN);
}

void swrite(int what, int which)
{
	FILE *fp;
	int i;

	// int np;
	char filename[MAX_PATHLEN+MAX_FNLEN], *fn;
	
	if(what>MRSCO) return;
	strcpy(filename, PA_PATH);
	strcat(filename, get_filename(what, which));
	fn=filename;
	
	printf("\n; Writing datafield...\n");
	
	if(what==ASCF) return;
	
	if(which==TXT) {
		if((fp=tryopen(fp, fn, "w"))==NULL) return;
		display(PARS, fp);
		if(what!=PARS) display(what, fp);
	}

	else {
		
		if((fp=tryopen(fp, fn, "wb"))==NULL) return;
		for(i=0; i<PA_NUMPARAMS; i++)
			if(fwrite(&dpa[i], sizeof(pars), 1, fp)!=1) {
				printf("\n; error writing %s\n", filename);
				getack();
				return;
			}

		if(what==MINSPEC)
			if(writelist(fp, fn, specxy, 0, PA_SPECPOI)==0) {
				printf("\n; error in loading file");
				getack();
				return;
			}
		if(what==SPECPLEX)
			if(writeplex(fp, fn, specplex, 0, PA_SPECPOI)==0){
				printf("\n; error in loading file");
				getack();
				return;
			}
		if(what==FULLPLEX)
			if(writeplex(fp, fn, fullplex, 0, PA_FULLPLEXPOI)==0){
				printf("\n; error in loading file");
				getack();
				return;
			}
	}

	printf("...done.\n");
	fclose(fp);
}

FILE *tryopen(FILE *fp, char *filename, char *mode)
{
	if((fp=fopen(filename, mode))==NULL) {
		printf("; cannot open %s\n", filename);
		getack();
	}
	return fp;
}

int readlist(FILE *fp, char *name, clist *fld, int beg, int end)
{
	int np;
	for(np=beg; np<end; np++)
		if(fread(&fld[np], sizeof(clist), 1, fp)!=1) {
			if(feof(fp)) break;
			printf("\n; error reading %s\n", name);
			getack();
			return 0;
		}
	return np;
}

int readplex(FILE *fp, char *name, cplex *fld, int beg, int end)
{
	int np;
	for(np=beg; np<end; np++)
		if(fread(&fld[np], sizeof(cplex), 1, fp)!=1) {
			if(feof(fp)) break;
			printf("\n; error reading %s\n", name);
			getack();
			return 0;
		}
	return np;
}

int writelist(FILE *fp, char *name, clist *fld, int beg, int end)
{
	int i;
	for(i=beg; i<end; i++)
		if(fwrite(&fld[i], sizeof(clist), 1, fp)!=1) {
			printf("\n; error writing %s\n", name);
			getack();
			return 0;
		}
	return 1;
}

int writeplex(FILE *fp, char *name, cplex *fld, int beg, int end)
{
	int i;
	for(i=beg; i<end; i++)
		if(fwrite(&fld[i], sizeof(cplex), 1, fp)!=1) {
			printf("\n; error writing %s\n", name);
			getack();
			return 0;
		}
	return 1;
}




