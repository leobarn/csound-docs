/* data.c */
/* make pre-scorefile datafields */

#include <math.h>
#include <stdlib.h>
#include "defg.h"	/* macros, typedefs, prototypes */

void copy_specxy(void)
{
	int i;
	
	for(i=0; i<PA_SPECPOI; i++) {
		specplex[i].x = specxy[i].x;
		specplex[i].y = specxy[i].y;
	}
}

void calc_hd_hr(void)
{
	int q, r=1;
	// float rumin;
	float *ri, *ru;
	double ppri, apri, pnri, anri, ppru, apru, pnru, anru;
	ri=malloc(PA_SPECPOI*sizeof(float));
	ru=malloc(PA_SPECPOI*sizeof(float));

	if(!ri || !ru)
		printf("\n; Malloc trouble.\n\n");

	/* hd (angle of the curve) */

	for(q=0; q<PA_SPECPOI; q++) {
		if(q<PA_SPECPOI-1) {
			r=q+1;								/* > q segment */
			ppri=specplex[r].y - specplex[q].y;
			ppru=specplex[r].x - specplex[q].x;
			r++;
			do {
				apri=ppri;
				apru=ppru;
				ppri=specplex[r].y - specplex[q].y;
				ppru=specplex[r].x - specplex[q].x;
				r++;
			} while( (r < PA_SPECPOI) &&
					 (fabs(ppri) > fabs(apri)) &&
					 (fabs(ppru) >= fabs(apru)) );
		}
		else apri=apru=0;
	
		if(q>0) {
			r=q-1;								/* < q segment */
			pnri=specplex[q].y - specplex[r].y;
			pnru=specplex[q].x - specplex[r].x;
			r--;
			if( ((SIGN(pnri)==SIGN(apri)) && (SIGN(pnru)==SIGN(apru)))
				|| apri==0 )
				do {
					anri=pnri;
					anru=pnru;
					pnri=specplex[q].y - specplex[r].y;
					pnru=specplex[q].x - specplex[r].x;
					r--;
				} while( (r > 0) &&
						 (fabs(pnri) > fabs(anri)) &&
						 (fabs(pnru) > fabs(anru)) );
			else {
				anri=0;
				anru=0;
			}
			ri[q]=(float)(anri+apri);
			ru[q]=(float)(anru+apru);
		}
		else {
			ri[q]=(float)apri;
			ru[q]=(float)apru;
		}
	}

	for(q=0; q<PA_SPECPOI; q++)
		specplex[q].hd=asin( ri[q] / hypot(ri[q],ru[q]) );

	/* hr (change in angle) */
	
	for(q=0; q<PA_SPECPOI; q++) {
		if(q<PA_SPECPOI-1) {
			r=q+1;								/* > q segment */
			ppri=specplex[r].hd - specplex[q].hd;
			ppru=hypot(specplex[r].x - specplex[r-1].x,
					   specplex[r].y - specplex[r-1].y);
			r++;
			do {
				apri=ppri;
				apru=ppru;
				ppri=specplex[r].hd - specplex[q].hd;
				ppru+=hypot(specplex[r].x - specplex[r-1].x,
							specplex[r].y - specplex[r-1].y);
				r++;
			} while( (r < PA_SPECPOI) &&
					 (fabs(ppri) > fabs(apri)) );
		}
		else apri=apru=0;

		if(q>0) {
			r=q-1;								/* < q segment */
			pnri=specplex[q].hd - specplex[r].hd;
			pnru=hypot(specplex[r+1].x - specplex[r].x,
					   specplex[r+1].y - specplex[r].y);
			r--;
			if( (SIGN(pnri) == SIGN(apri)) || apri==0.0)
				do {
					anri=pnri;
					anru=pnru;
					pnri=specplex[q].hd - specplex[r].hd;
					pnru+=hypot(specplex[r+1].x - specplex[r].x,
								specplex[r+1].y - specplex[r].y);
					r--;
				} while( (r > 0) &&
						 (fabs(pnri) > fabs(anri)) );
			else {
				anri=0;
				anru=0;
			}
			ri[q]=(float)(anri+apri);
			ru[q]=(float)(anru+apru);
		}
		else {
			ri[q]=(float)apri;
			ru[q]=(float)apru;
		}
	}

	for(q=0; q<PA_SPECPOI; q++)
		specplex[q].hr=ri[q]/ru[q];
}

void make_fullplex(void)
{
	int r, q, loc;
	float fb, te;
	int scale=MAX_POI/PA_SPECPOI;
	PA_FULLPLEXPOI=scale*PA_SPECPOI;
	
	for(r=0; r<PA_SPECPOI; r++) {
		loc = r * scale;
		fullplex[loc].x = specplex[r].x;
		fullplex[loc].y = specplex[r].y;
		fullplex[loc].hr = specplex[r].hr;
		fullplex[loc].hd = specplex[r].hd;
		for(q=1; q<scale; q++) {
			fb = (float)(scale-q)/scale;
			te = (float)q/scale;
			fullplex[q+loc].x = (specplex[r].x * fb) + (specplex[r+1].x * te);
			fullplex[q+loc].y = (specplex[r].y * fb) + (specplex[r+1].y * te);
			fullplex[q+loc].hd=(specplex[r].hd * fb) + (specplex[r+1].hd * te);
			fullplex[q+loc].hr=(specplex[r].hr * fb) + (specplex[r+1].hr * te);
		}
	}
	printf("; (%i-point scale factor)\n", scale);
}

void s_and_q(void)
{
	int q;
	float pcsz=(float)1/PA_NPC, nminhr, pminhr, nmaxhr, pmaxhr;
	
	q=0;
	while(fullplex[q].hr<=0) q++;
	pminhr=pmaxhr=(float)fullplex[0].hr;

	q=0;
	while(fullplex[q].hd>=0) q++;
	nminhr=nmaxhr=(float)fullplex[0].hr;

	for(q=0; q<PA_FULLPLEXPOI; q++) {
		if(fullplex[q].hr>0) {
			pminhr=(float)(fullplex[q].hr<pminhr ? fullplex[q].hr : pminhr);
			pmaxhr=(float)(fullplex[q].hr>pmaxhr ? fullplex[q].hr : pmaxhr);
		}
		else {
			nminhr=(float)(fullplex[q].hr>nminhr ? fullplex[q].hr : nminhr);
			nmaxhr=(float)(fullplex[q].hr<nmaxhr ? fullplex[q].hr : nmaxhr);
		}
	}

	for(q=0; q<PA_FULLPLEXPOI; q++) {
		fullplex[q].x=quant(
			linscale(fullplex[q].x, PA_XMIN, PA_XMAX, PA_MINPCH, PA_MAXPCH)
			, pcsz);
		fullplex[q].y=quant(
			linscale(fullplex[q].y, PA_YMIN, PA_YMAX, PA_MINPCH, PA_MAXPCH)
			, pcsz);
		
		if(fullplex[q].hd>=0)
			fullplex[q].hd=quant(
				linscale(fullplex[q].hd, 0, POSHALFPI,
				PA_DURUNIT, PA_MAXDUR), PA_MINDUR);
		else
			fullplex[q].hd=quant(
				linscale(fullplex[q].hd, 0, NEGHALFPI,
				PA_MINDUR, PA_DURUNIT), PA_MINDUR);
		
		if(fullplex[q].hr>0)
			fullplex[q].hr=floor(
				linscale(fullplex[q].hr,pminhr,pmaxhr,
				PA_MINREACH,PA_MAXREACH));
		else
			fullplex[q].hr=floor(
				linscale(fullplex[q].hr,nminhr,nmaxhr,
				-PA_MINREACH,-PA_MAXREACH));
	}
}


