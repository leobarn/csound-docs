/* main.c */

#include <stdio.h>

#include <stdlib.h>
#include "defg.h"	/* macros, typedefs, prototypes */

void main(void)
{
	int wha;

	char i1pf[NUM_I1PFIELDS+1][15]={"", I1PF};
	char i2pf[NUM_I2PFIELDS+1][15]={"", I2PF};
	char i4pf[NUM_I4PFIELDS+1][15]={"", I4PF};

	init(MINSPEC, NA);
	init(SPECPLEX, NA);
	init(FULLPLEX, NA);
	init(PARS, ALL);
	for(;;) switch(task()) {

		case 'a' : adj_params();
			break;

		case 'v' : display(wh(DATAFIELDW, DFABBW), stdout);
			break;

		case 'i' : init(wh(DATAFIELDW, DFABBW), ALL);
			break;

		case 'l' : load(wh(DATAFIELDL, DFABBL));
			break;

		case 'w' : 
			wha=wh(DATAFIELDW, DFABBW);
			if(wha<=ASCF) swrite(wha,wh(FILETYPE, FTABB));
			else swrite(wha,TXT);
			break;

		case 'e' : enteredit_specxy();
			break;

		case 'f' :
			if(GOT_MINSPEC==NO) {
				printf("\n; You must first enter or load a minspec.\n");
				printf("; Cannot create specplex.\n");
				getack();
			}
			else {			
				printf("; Calculating hd and hr values...\n");
				calc_hd_hr();
				GOT_SPECPLEX=YES;
				printf("; ...done.\n");
			}
			break;

		case 'm' :
			if(GOT_SPECPLEX==NO) {
				printf("\n; You must first calculate or load a specplex.\n");
				printf("; Cannot create fullplex.\n");
				getack();
			}
			else {
				printf("\n; Fitting %i-point spec to %i-point fullplex...\n",
					PA_SPECPOI, PA_SPECPOI*(MAX_POI/PA_SPECPOI));
				make_fullplex();
				GOT_FULLPLEX=YES;
				printf("; ...done.\n");
			}
			break;

		case 's' :
			if(GOT_FULLPLEX==NO) {
				printf("\n; You must first calculate or load a fullplex.\n");
				printf("; Cannot scale non-existent data.\n");
				getack();
			}
			else {
				printf("\n; in fullplex:\n");
				printf("; Scaling x and y values to pitch/amp ranges\n");
				printf(";   (pitch-range: %.2f to %.2f, ", PA_MINPCH, PA_MAXPCH);
				printf("amp range: %.2f to %.2f dB)\n", PA_MINDB, PA_MAXDB);
				printf(";   and quantizing pitch-values into %i pitch-classes,\n", PA_NPC);
				printf("; Scaling hd values to max. dur of %.3f, min. of %.3f\n", PA_MAXDUR, PA_MINDUR);
				printf(";   (quantizing into %.3f/%.3fs),\n", PA_DURUNIT, PA_MAXDUR);
				printf("; Scaling hr values to varying min. reach of +/-%i, max. reach of +/-%i...\n", PA_MINREACH, PA_MAXREACH);
				s_and_q();
				SCALED_FULLPLEX=YES;
				printf("; ...done\n");
			}
			break;
		case 'c' :
			if(SCALED_FULLPLEX==NO) {
				if(GOT_FULLPLEX==YES) {
					printf("\n; Warning: Fullplex in memory is not scaled.\n");
					if(wh("Generate score anyway (y/[n])", "ny")==2)
						SCALED_FULLPLEX=YES;
					else {
						printf("; Aborting score generation.\n");
						getack();
						break;
					}
				}
				else {
					printf("\n; You must first calculate or load a fullplex.\n");
					printf("; No score generated.\n");
					getack();
					break;
				}
			}
			printf("\n; Beginning Csound file generation...\n");
			make_sco();
			printf("; ...done.  Score is ready for Csound.\n");
			GOT_SCO=YES;
			break;
		case 'q' : exit(0);
	}
		
}

