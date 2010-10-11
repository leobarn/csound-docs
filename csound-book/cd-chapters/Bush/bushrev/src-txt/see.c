/* see.c */
/* display datafields */

#include <stdio.h>
#include <string.h>
#include "defg.h"	/* general macros, typedefs, prototypes */

void display(int what, FILE *fp)
{
	switch(what) {
		case PARS: show_params(fp);
			break;
		case MINSPEC: show_minspec(fp);
			break;
		case SPECPLEX: show_specplex(fp);
			break;
		case FULLPLEX: show_fullplex(fp);
			break;
		case ASCF: return;
			break;
		case FSCO: show_sco(FSCO, fp);
			break;
		case MRSCO: show_sco(MRSCO, fp);
			break;
	}
}	

void show_params(FILE *fp)
{
	int nu=0;
	
	fprintf(fp, "\n");
	for (nu=1; nu<PA_NUMPARAMS; nu++) {
		if(fp==stdout)
			if(check_pause(nu)) break;
		fprintf(fp, "; %i> %s: ", nu, dpa[nu].dex);
		if(MOD==YES) fprintf(fp, "[");
		if(KIND==INT) fprintf(fp, "%i", dpa[nu].i);
		if(KIND==FLOAT) fprintf(fp, "%.2f", dpa[nu].f);
		if(KIND==FNAME) fprintf(fp, "\"%s%s\"", PA_PATH, 
			nu>1 ? dpa[nu].fname : "");
		if(MOD==YES) fprintf(fp, "]");
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
}

void show_minspec(FILE *fp)
{
	int t, u;

	if(PA_SPECPOI==0) {
		printf("\n; Datafield empty.\n");
		return;
	}

	if(PA_SPECPOI > 0) {
		if(fp==stdout) {
			printf("\n; Current entries number 0 thru %i.\n; Start at (0-%i)? ", PA_SPECPOI-1, PA_SPECPOI-1);
			do scanf("%i", &u);
			while (u < 0 || u > PA_SPECPOI-1);
		}
	}

	fprintf(fp, "\n;\t   x\t   y");
	fprintf(fp, "\n;\t-------\t-------\n");
	for(t=u; t<PA_SPECPOI; t++) {
		if(fp==stdout)
			if(check_pause(t-u)) break;
		fprintf(fp, "\n; %i:\t%-6.0f\t%-6.0f",
		t, specxy[t].x, specxy[t].y);
	}

}

void show_specplex(FILE *fp)
{
	int t, u;

	if(PA_SPECPOI==0) {
		printf("\n; Datafield empty.\n");
		return;
	}

	if(PA_SPECPOI > 0) {
		if(fp==stdout) {
			printf("\n; Current entries number 0 thru %i.\n; Start at (0-%i)? ", PA_SPECPOI-1, PA_SPECPOI-1);
			do scanf("%i", &u);
			while (u < 0 || u > PA_SPECPOI-1);
		}
	}
	fprintf(fp, "\n;\t   x\t   y\t   hd\t    hr");
	fprintf(fp,	"\n;\t-------\t-------\t-------\t----------\n");

	for(t=u; t<PA_SPECPOI; t++) {
		if(fp==stdout)
			if(check_pause(t-u)) break;
		fprintf(fp,	"\n; %i:\t%-7.2f\t%-7.2f\t%-7.3f\t%-10.6g",
				t, specplex[t].x, specplex[t].y,
				specplex[t].hd, specplex[t].hr);
	}
}

void show_fullplex(FILE *fp)
{
	int t, u;

	if(PA_FULLPLEXPOI==0) {
		printf("\n; Datafield empty.\n");
		return;
	}

	if(PA_FULLPLEXPOI > 0) {
		if(fp==stdout) {
			printf("\n; Current entries number 0 thru %i.\n; Start at (0-%i)? ", PA_FULLPLEXPOI-1, PA_FULLPLEXPOI-1);
			do scanf("%i", &u);
			while (u < 0 || u > PA_SPECPOI-1);
		}
	}

	fprintf(fp, "\n;\t   x\t   y\t   hd\t    hr");
	fprintf(fp,	"\n;\t-------\t-------\t-------\t----------\n");

	for(t=0; t<PA_FULLPLEXPOI; t+=PA_STEP) {
		if(fp==stdout)
			if(check_pause(t/PA_STEP)) break;
		fprintf(fp,	"\n; %i:\t%-7.2f\t%-7.2f\t%-7.3f\t%-10.6g",
				t, fullplex[t].x, fullplex[t].y, fullplex[t].hd, fullplex[t].hr);
	}
}

void show_sco(int what, FILE *fp)
{
	int ne, j, t, np;
	float inum, rvbdur;
	char op;
	EVENT **snote;

	ne=lcount(f);
	snote=&f->e[1];

/* list general f-statements */

	fprintf(fp, "\n; general f-statements\n; ====================");

	while(ne--) {
		np=(*snote)->pcnt;
		fprintf(fp, "\nf ");
		for(j=1; j<=4;  j++) fprintf(fp, "%.0f ", (*snote)->p[j]);
		for(j=5; j<=np; j++) fprintf(fp, "%f ", (*snote)->p[j]);
		snote++;
	}

/* list i-statements */

	fprintf(fp, "\n\n; i-statements\n; ============\n");
			
	ne=lcount(sco);
	snote=&sco->e[ne];

	fprintf(fp, "\n;\t");
	for(t=2; t<=MOST_PFIELDS; t++) fprintf(fp, "p%i\t",t);

	rvbdur = (*snote)->p[II_STA] + (*snote)->p[II_DUR] + (PA_RVBTIME * 3);
	fprintf(fp, "\ni91\t0\t%-12.6f\t%.3f", rvbdur, PA_RVBTIME);
	fprintf(fp, "\ni92\t0\t%-12.6f\t%.3f", rvbdur, PA_RVBTIME);
	fprintf(fp, "\ni94\t0\t%-12.6f\t%.3f", rvbdur, PA_RVBTIME);
	
	snote=&sco->e[1];

	while(ne--) {
		op=(*snote)->op;
		inum=(*snote)->p[II_I];
		np=(*snote)->pcnt;
		if(fp==stdout) check_pause(t);
		fprintf(fp, "\ni %.1f\t", inum);
		if(what==FSCO)
			for(j=2; j<=np; j++)
				fprintf(fp, "%-f\t", (*snote)->p[j]);
		if(what==MRSCO)
			for(j=2; j<=np; j++)
				fprintf(fp, "%-7.3f\t", (*snote)->p[j]);
	snote++;
	}
	
	fprintf(fp,"\ne\n");
}





