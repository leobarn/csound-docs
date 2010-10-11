/* cal.c */
/* value converters, special functions */

#include <math.h>
#include "defg.h"	/* macros, typedefs, prototypes */

void getmaxmin(cplex *fplex, int np)
{
	int i;
	
	PA_XMIN=PA_XMAX=(float)fplex[0].x;
	PA_YMIN=PA_YMAX=(float)fplex[0].y;
	for(i=0; i<np; i++) {
		PA_XMIN=(float)(fplex[i].x<PA_XMIN ? fplex[i].x : PA_XMIN);
		PA_XMAX=(float)(fplex[i].x>PA_XMAX ? fplex[i].x : PA_XMAX);
		PA_YMIN=(float)(fplex[i].y<PA_YMIN ? fplex[i].y : PA_YMIN);
		PA_YMAX=(float)(fplex[i].y>PA_YMAX ? fplex[i].y : PA_YMAX);
	}
}

double ampdb(double p)
{
	return exp(p * 0.11512925);
}

double dbamp(double p)
{
	return log(fabs(p) / 0.11512925);
}

double cpsoct(double p)
{
	return pow(2.,p) * 1.021975;
}

double octcps(double p)
{
	return log(p / 1.021975) / 0.693147;
}

int pc(double p)
{
	double reg, interm;
	interm=modf(p, &reg)*PA_NPC;
	return (int)interm;
}

int susum(int n)
{
	int ss, r;
	for (r = 1; r <= n; r++)
		ss = ss + r;
	return(ss);
}

double quant(double value, double quantum)
{
	return floor(value/quantum)*quantum;
}

double gof(double value, double comp)
{
	return (value>comp ? value : comp);
}

double lof(double value, double comp)
{
	return (value<comp ? value : comp);
}

double betw(double value, double min, double max)
{
	return (value>=min ? (value<=max ? value : max) : min);
}

double fbetw(double value, double min, double max)
{
	if(value==0.0) return betw(value, min, max);
	while(value<min) value*=2.0;
	while(value>max) value*=0.5;
	return betw(value, min, max);
}

double avg(double val1, double val2)
{
	return (val1 + val2)/2;
}

double linscale(double value, double imin, double imax, double smin, double smax)
{
	double irange=imax-imin, srange=smax-smin;
	
	if(irange==0.0) return smin;
	return (((value-imin)/irange)*srange)+smin;
}

double logscale(double value, double imin, double imax, double smin, double smax)
{
	int range=1;

	while(value<=10) {
		range*=10;
		value*=range;
	}
	imin*=range;
	imax*=range;
	return linscale(log(value)/log(imax), 0, imax, smin, smax);
}





		
