/* par.c */
/* initializations */


#include <string.h>
#include "defg.h"	/* macros, typedefs, prototypes */

void init(int what, int which)
{
	int nu=0;
	
	if(what==PARS) {
	
		DEX("(number of parameters)");
			MOKI=INT+NO;
			NEXT;
		DEX("working path");
			MOKI=FNAME+YES;
			ONLY_ON_ALL NAME_IS(DEF_PATH);
			NEXT;
		DEX("working parameters filename");
			MOKI=FNAME+YES;
			ONLY_ON_ALL NAME_IS(DEF_PARSFILE);
			NEXT;
		DEX("minspec filename");
			MOKI=FNAME+YES;
			ONLY_ON_ALL NAME_IS(DEF_MINSPECFILE);
			NEXT;
		DEX("specplex filename");
			MOKI=FNAME+YES;
			ONLY_ON_ALL NAME_IS(DEF_SPECPLEXFILE);
			NEXT;
		DEX("fullplex filename");
			MOKI=FNAME+YES;
			ONLY_ON_ALL NAME_IS(DEF_FULLPLEXFILE);
			NEXT;
		DEX("adsyn control filename");
			MOKI=FNAME+YES;
			ONLY_ON_ALL NAME_IS(DEF_ASCFFILE);
			NEXT;
		DEX("full sco filename");
			MOKI=FNAME+YES;
			ONLY_ON_ALL NAME_IS(DEF_FSCOFILE);
			NEXT;
		DEX("mr-sco filename");
			MOKI=FNAME+YES;
			if(which==ALL) NAME_IS(DEF_MRSCOFILE);
			NEXT;
		DEX("number of points in spec");
			MOKI=INT+NO;
			ONLY_ON_ALL PA_SPECPOI=0;
			NEXT;
		DEX("number of points in fullplex");
			MOKI=INT+NO;
			ONLY_ON_ALL PA_FULLPLEXPOI=0;
			NEXT;
		DEX("maximum x-value in spec/fullplex");
			MOKI=FLOAT+NO;
			ONLY_ON_ALL PA_XMAX=1000.00;
			NEXT;
		DEX("minimum x-value in spec/fullplex");
			MOKI=FLOAT+NO;
			ONLY_ON_ALL PA_XMIN=0.00;
			NEXT;
		DEX("maximum y-value in spec/fullplex");
			MOKI=FLOAT+NO;
			ONLY_ON_ALL PA_YMAX=1000.00;
			NEXT;
		DEX("minimum y-value in spec/fullplex");
			MOKI=FLOAT+NO;
			ONLY_ON_ALL PA_YMIN=0.00;
			NEXT;
		DEX("step increment thru fullplex (positive integer)");
			MOKI=INT+YES;
			PA_STEP=100;
			NEXT;
		DEX("durational unit (in beats)");
			MOKI=FLOAT+YES;
			PA_DURUNIT=1.66F;
			NEXT;
		DEX("max./min. duration multiple/fraction (positive integer)");
			MOKI=INT+YES;
			PA_DURMF=6;
			NEXT;
		DEX("maximum loudness (in dB)");
			MOKI=FLOAT+YES;
			PA_MAXDB=60;
			NEXT;
		DEX("minimum loudness (in dB)");
			MOKI=FLOAT+YES;
			PA_MINDB=40;
			NEXT;
		DEX("minimum [R] pitch (in register.decimal)");
			MOKI=FLOAT+YES;
			PA_MINPCH=5.50F;
			NEXT;
		DEX("maximum [R] pitch (in register.decimal)");
			MOKI=FLOAT+YES;
			PA_MAXPCH=11.00F;
			NEXT;
		DEX("maximum pitch wobble (in percent of pitch value: between 0 and 1)");
			MOKI=FLOAT+YES;
			PA_MAXWOB=.06F;
			NEXT;
		DEX("number of pitch-classes");
			MOKI=INT+YES;
			PA_NPC=12;
			NEXT;
		DEX("minimum distance of sound events (in meters)");
			MOKI=FLOAT+YES;
			PA_MINDIST=2;
			NEXT;
		DEX("maximum distance of sound events (in meters)");
			MOKI=FLOAT+YES;
			PA_MAXDIST=50;
			NEXT;
		DEX("average reverb time (in seconds)");
			MOKI=FLOAT+YES;
			PA_RVBTIME=0.60F;
			NEXT;
		DEX("number of lines on terminal screen");
			MOKI=INT+YES;
			PA_TERMLINES=40;
			NEXT;
		SET_NUMPARAMS;	
	}
	
	if(what==MINSPEC) {
		for(nu=0; nu<MAX_POI; nu++) specxy[nu].x = specxy[nu].y = -1;
		PA_SPECPOI=0;
	}
	
	if(what==SPECPLEX) {
		for(nu=0; nu<MAX_POI; nu++) specplex[nu].x = specplex[nu].y =
								  specplex[nu].hr = specplex[nu].hd = -1;
		PA_SPECPOI=0;
	}
	
	if(what==FULLPLEX) {
		for(nu=0; nu<MAX_POI; nu++) fullplex[nu].x = fullplex[nu].y =
								  fullplex[nu].hr = fullplex[nu].hd = -1;
		PA_FULLPLEXPOI=0;
	}

	if(what==FSCO || what==MRSCO) printf("\n; No need to bother.");
}





