/* choix.c */
/* handle choices, adjustments, data entry */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/* #include <sys/param.h> */
#include "defg.h"	/* macros, typedefs, prototypes */

int task(void)
{
	char s[80];
	
	do {
		printf("\n\n");
		printf("; (A)djust/view working parameters\n\n");
		printf("; (V)iew datafield [enter an \"e\" to interrupt view]\n");
		printf("; (I)nitialize datafield\n");
		printf("; (L)oad datafield\n");
		printf("; (W)rite datafield\n\n");
		printf("; (E)nter/edit minspec values\n");
		printf("; (F)ind hd and hr values: minspec-->specplex\n");
		printf("; (M)ake fullplex from specplex\n");
		printf("; (S)cale and quantize fullplex values\n");
		printf("; (C)reate scorefile\n");
		printf("; (Q)uit\n\n");
		printf("; choose one: ");
		gets(s);
	} while(!strchr("avinlwefmscq", tolower(*s)));
	return tolower(*s);
}

int wh(char *chdx, char *chab)
{
	char s[80];
	int res;
	
	do {
		printf("\n; %s? ", chdx);
		gets(s);
		strcat(s, "\n");
		if(*s=='\n') break;
	} while(!strchr(chab, tolower(*s)));
	res=strcspn(chab, s)+1;
	return (abs(res)>abs(strlen(chab)) ? 1 : res);
}

void enteredit_specxy(void)
{
	int i,u=0;
	float x,y;
		
	if(PA_SPECPOI==MAX_POI) {
		printf("\n; Spec datafield full!\n; (Want to initialize it?)\n");
		return;
	}
	if(PA_SPECPOI>0) {
		printf("\n; Current entries number 0 thru %i.\n; Start at (0-%i)? ", PA_SPECPOI-1, PA_SPECPOI);
		do scanf("%i", &u);
		while (u < 0 || u > PA_SPECPOI);
	}

	printf("\n\n; Enter \"-1, -1\" to quit entry;\n; enter \"-2, -2\" to leave values unchanged and go to next entry;\n; enter \"-3, -3\" to go back one entry.\n\n");
	printf(";\t   x\t   y\n");
	printf(";\t-------\t-------\n");
	for(i=u; i<MAX_POI; i++) {
		if(specxy[i].x>0)
			printf("%d:\t[%.0f]\t[%.0f]\t(present values)\n",
			  i, specxy[i].x, specxy[i].y);
		do {
			printf("%d:\t", i);
			scanf("%f\t%f", &x, &y);
		} while(x>ABS_XMAX || y>ABS_YMAX);

		if(x==-1) {
			if(specxy[i].x!=-1)printf("\n; Okay--value of entry %i unchanged.", i);
			else printf("\n; Okay--%i points in minspec so far (last entry: %i).", PA_SPECPOI=i, i-1);
			break;
		}
		else if(x==-2) {
			if(specxy[i].x==-1) {
				printf("\n; There's no value there to begin with.  Leaving entry.");
				break;
			}
		}
		else if(x==-3) i-=2;
		else {
			specxy[i].x=x;
			specxy[i].y=y;
			PA_SPECPOI+=(i==PA_SPECPOI);
		}
	}
	copy_specxy();
	GOT_MINSPEC=YES;
}

void adj_params(void)
{
	char cho[80];
	int nu=0;
	
	printf("\n; Enter parameter number to adjust, enter \"i\" to\n");
	printf("; initialize parameters to defaults (won't affect values\n");
	printf("; dependent upon table in memory), or press return to exit.\n");
	printf("; (nonbracketed values are fixed)\n");
	for(;;) {
		show_params(stdout);
		do {
			printf("; choice (\"i\", 1-%i, return to exit)? ", PA_NUMPARAMS-1);
			gets(cho);
			nu=atoi(cho);
			strcat(cho, "\n");
		} while ((nu>PA_NUMPARAMS || nu==0) && (*cho!='\n' && *cho!='i'));
		if(*cho=='\n') break;
		if(*cho=='i') {
			init(PARS, TIND);
			printf("\n; Okay--initializing table-independent parameters to defaults...\n");
		}
		else {
			if(MOD==YES) {
				printf("\n; new value for\n; %s: ", dpa[nu].dex);
				gets(cho);
				if(KIND==FNAME) strcpy(dpa[nu].fname, cho);
				if(KIND==INT) dpa[nu].i=atoi(cho);
				if(KIND==FLOAT) dpa[nu].f=(float)atof(cho);
				printf("\n; Okay.\n");
			}
			else printf("\n; Sorry--that value is fixed.\n");
		}
	}
}

int check_pause(int t)
{
	char key[10];
	
	if(PA_TERMLINES>0 && fmod(t+1,PA_TERMLINES-3)==0) {
		gets(key);
		return (int)strchr(key, 'e');
	}
	else return 0;
}

char *getack(void)
{
	char ackstr[80];
	printf("; [ok]:");
	return(gets(ackstr));
}

char *get_filename(int what, int which)
{
	char fn[MAX_FNLEN], *filename;
	
	printf("\n; filename: %s[%s%s] ",
		PA_PATH, dpa[what+1].fname, (which==TXT && what<FSCO) ? ".txt" : "");
	do gets(fn);
	while(strlen(fn)>MAX_FNLEN);
	if(strlen(fn)==0) strcpy(fn, dpa[what+1].fname);
	else strcpy(dpa[what+1].fname, fn);

	filename=fn;
	if(which==TXT && what<FSCO)
		if(!strstr(filename, ".txt")) strcat(filename, ".txt");
	return filename;
}





