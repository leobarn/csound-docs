
/* -----------------------------------------------------------------------
 
    Copyright (C) 1997  Fabio P. Bertolotti   (fabio.bertolotti@dlr.de)
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 
------------------------------------------------------------------------- */






/*----------------------------------------------------------------------------
 * 
 *  Each new graph is plotted on a fresh PostScript page
 *  On each page we print the current date, the name of the
 *  hetro file, the maximum and minimum values
 *  and the figure caption.
 * 
 *
  ----------------------------------------------------------------------------*/

#include <string.h>
#include "allocate.h"


          /* 
           *   Origin and size of PostScript plot
           * 
           */
#define MyPS_FONT      "/Times-Roman"
#define MyPS_FONTSIZE  20 


static  int MyPS_XORIG;
static  int MyPS_YORIG;                     
static  int MyPS_WIDTH;                   
static  int MyPS_HEIGHT;


static void setAxisNumbers(min, max, cmin, cmax)
       float *min;
       float *max;
       char  *cmin;
       char  *cmax;
{
        double bmin, bmax, big;
        int    i;
      

        /*
         *  Get most significant digit
         */

        bmin = 0.0000001;
        if (fabs(*min)>bmin) {
          while ( (int) (fabs(*min)/bmin) ) bmin = bmin * 10;
        }

        bmax = 0.0000001;
        if (fabs(*max)>bmax) {
          while ( i = fabs(*max)/bmax ) bmax = bmax * 10;
        }
        if (fabs(bmin) > fabs(bmax)) big = fabs(bmin);
        else big = fabs(bmax);

        /*
         *  Set max and minimm to nearest 2nd sig digit
         */  

        if (*max == 0) i = 0;
        else           i = (*max/big)*100 + 1;
        *max = i*big/100;

        if (*min == 0) i = 0;
        else           i = (*min/big)*100 - 1;
        *min = i*big/100;

        if (fabs(*max - *min) < 0.0000001) 
            *max = *min + 1;                       /* No zero divide */


        /*
         *  Write characters
         */

         sprintf(cmin,"%g",*min);
         sprintf(cmax,"%g",*max);
}



static void PS_drawAxes(psFile, cxmin, cxmax, cymin, cymax) 
      FILE *psFile;
      char *cxmin;
      char *cxmax;
      char *cymin; 
      char *cymax; 
{
	float xx, yy, dx, dy;
        float fnts, swide;
	int i;

	/*
	 * Make axes - box
	 */
	fprintf(psFile,"          \n");
	fprintf(psFile,"%%   Axes  \n");
	fprintf(psFile,"1 setlinewidth \n");
	fprintf(psFile,"newpath   \n");      
	xx = MyPS_XORIG ;
	yy = MyPS_YORIG ;
	fprintf(psFile,"%f  %f  moveto \n",xx, yy);     

	xx = MyPS_XORIG + MyPS_WIDTH;
	fprintf(psFile,"%f  %f  lineto \n",xx, yy);     

	yy = MyPS_YORIG + MyPS_HEIGHT;
	fprintf(psFile,"%f  %f  lineto \n",xx, yy);     

	xx = MyPS_XORIG ;
	fprintf(psFile,"%f  %f  lineto \n",xx, yy);     
	fprintf(psFile,"closepath stroke \n");



	/*
	 * Make tick marks:   x-axis
	 */
	fprintf(psFile,"                \n");
	fprintf(psFile,"%%   x-tickmarks \n");
	fprintf(psFile,"1 setlinewidth  \n");
	xx = MyPS_XORIG;
        dx = (1.00 * MyPS_WIDTH) / 10;
        dy = (1.00 * MyPS_HEIGHT)/ 10;
	for (i = 0; i <= 10; i++) {
                yy = MyPS_YORIG ;
                fprintf(psFile,"%f  %f  moveto \n",xx, yy);     
                yy = MyPS_YORIG + dy/6 ;
                fprintf(psFile,"%f  %f  lineto stroke \n",xx, yy);     
                yy = MyPS_YORIG + MyPS_HEIGHT - dy/6 ;
                fprintf(psFile,"%f  %f  moveto \n",xx, yy);     
                yy = MyPS_YORIG + MyPS_HEIGHT;
                fprintf(psFile,"%f  %f  lineto stroke \n",xx, yy);     
                xx += dx;
	}


	/*
	 * Make tick marks:   y-axis
	 */
	fprintf(psFile,"                \n");
	fprintf(psFile,"%%   y-tickmarks \n");
	fprintf(psFile,"1 setlinewidth  \n");
	yy = MyPS_YORIG;
	for (i = 0; i <= 10; i++) {
                xx = MyPS_XORIG ;
                fprintf(psFile,"%f  %f  moveto \n",xx, yy);     
                xx = MyPS_XORIG + dx/6 ;
                fprintf(psFile,"%f  %f  lineto stroke \n",xx, yy);     
                xx = MyPS_XORIG + MyPS_WIDTH - dx/6;
                fprintf(psFile,"%f  %f  moveto \n",xx, yy);     
                xx = MyPS_XORIG + MyPS_WIDTH;
                fprintf(psFile,"%f  %f  lineto stroke \n",xx, yy);     
                yy += dy;
	}


	/*
	 * Label the axes's max and min
	 */
        fnts = 1.0*MyPS_FONTSIZE;
	fprintf(psFile,"                \n");
	fprintf(psFile,"%s findfont %f scalefont setfont \n", 
                       MyPS_FONT, fnts);

        swide = 0.5*fnts*strlen(cxmin);
        xx    = MyPS_XORIG - swide / 2;
        yy    = MyPS_YORIG - fnts * 1.8;
        fprintf(psFile,"%f %f moveto \n",xx, yy);
        fprintf(psFile,"(%s) show \n", cxmin);

        swide = 0.5*fnts*strlen(cxmax);
        xx    = MyPS_XORIG + MyPS_WIDTH - swide / 2;
        yy    = MyPS_YORIG - fnts * 1.8;
        fprintf(psFile,"%f %f moveto \n",xx, yy);
        fprintf(psFile,"(%s) show \n", cxmax);

        swide = 0.5*fnts*strlen(cymin);
        xx    = MyPS_XORIG - fnts/2 - swide;
        yy    = MyPS_YORIG - fnts * 0.4;
        fprintf(psFile,"%f %f moveto \n",xx, yy);
        fprintf(psFile,"(%s) show \n", cymin);

        swide = 0.5*fnts*strlen(cymax);
        xx    = MyPS_XORIG - fnts/2 - swide;
        yy    = MyPS_YORIG + MyPS_HEIGHT - fnts * 0.4;
        fprintf(psFile,"%f %f moveto \n",xx, yy);
        fprintf(psFile,"(%s) show \n", cymax);

}



void makePlot2D( psFile, today, fileNameIn, partials, 
                 time_amp , time_fre, 
                 value_amp, value_fre, 
                 count_amp, count_fre )
       FILE *psFile;
       char *today; 
       char *fileNameIn;
       int   partials;
 short int   time_amp[MaxPARTIALS][MaxPOINTS];
 short int   time_fre[MaxPARTIALS][MaxPOINTS];
 short int  value_amp[MaxPARTIALS][MaxPOINTS];
 short int  value_fre[MaxPARTIALS][MaxPOINTS];
 short int  count_amp[MaxPARTIALS];
 short int  count_fre[MaxPARTIALS];
{
    float xmin, xmax, ymin, ymax, xx, yy, dx, dy, fnts;
    char  cxmin[20], cxmax[20], cymin[20], cymax[20];
    char  string[100];
    int   i, j, currentPage=0, N, par;



    /*------------------------
     *  Loop through partials
     *------------------------*/

     for(par=0; par<partials; par++){



       /*--------------------
        *  write ``newpage'' 
        *--------------------*/

        currentPage++;
        if(currentPage>1) fprintf(psFile,"showpage  \n");
        fprintf(psFile,"  \n");
        fprintf(psFile,"%%%%Page: %d %d \n", currentPage, currentPage);
        fprintf(psFile,"  \n");



        /*------------------------------------------------------------
         *  AMPLITUDE: Get labels for axis limits, then draw the axis
         *------------------------------------------------------------*/

        MyPS_XORIG  =  100;
        MyPS_YORIG  =  430;             
        MyPS_WIDTH  =  450;            
        MyPS_HEIGHT =  220;            

        N    = count_amp[par];
        if (N >= MaxPOINTS) {
           printf("  error (2dPlot) partial=%d has more points than allowed\n",
                   par);
           printf("       number of ampl. break points = %d,  maximum = %d\n",
                   N, MaxPOINTS);
           exit(1);
        }
        xmin = 0.0; 
        xmax = time_amp[par][N-1];

        ymin = 0.0;
        ymax = 0.0;
        for (j=0; j<N; j++) 
          if(ymax < 1.1*value_amp[par][j]) ymax = 1.1*value_amp[par][j];

        if (ymax - ymin < 1e-07) ymax = ymin + 0.01;
        if (xmax - xmin < 1e-07) {
           printf("  error (2dPlot) partial=%d,  time max - min = 0 \n", par);
           exit(1);
        }

        sprintf(cxmin,"%d",(int) xmin);
        sprintf(cxmax,"%d",(int) xmax);
        setAxisNumbers(&ymin, &ymax, cymin, cymax);
	PS_drawAxes(psFile, cxmin, cxmax, cymin, cymax);



        /* ------------------------
         *  write the plot caption
         * ------------------------*/

        fnts = 1.0*MyPS_FONTSIZE;
	fprintf(psFile,"                \n");
	fprintf(psFile,"%s findfont %f scalefont setfont \n", 
                       MyPS_FONT, fnts);
        fprintf(psFile,"50 740 moveto (date: %s) show \n", today);
	fprintf(psFile,"50 720 moveto (file: %s) show \n", fileNameIn);
 

        xx = MyPS_XORIG + MyPS_WIDTH/3;
        yy = MyPS_YORIG + MyPS_HEIGHT + fnts;
        fprintf(psFile,"%f  %f  moveto \n", xx, yy);
        fprintf(psFile,"(Amplitude:  Partial %d) show \n", par);



        /* ---------------------
         *  write the plot data
         * ---------------------*/

	fprintf(psFile,"               \n");
	fprintf(psFile,"%% Plot data    \n");
	fprintf(psFile,"1 setlinewidth \n");

	dx = MyPS_WIDTH /(xmax - xmin);
	dy = MyPS_HEIGHT/(ymax - ymin);

	for(i=0; i < N; i++) {
                xx = MyPS_XORIG + ( time_amp[par][i] - xmin)*dx;
                yy = MyPS_YORIG + (value_amp[par][i] - ymin)*dy;
                if (!i) fprintf(psFile,"%f  %f  moveto \n", xx, yy);
                if ( i) fprintf(psFile,"%f  %f  lineto \n", xx, yy);
	}
	fprintf(psFile,"stroke \n");





        /*------------------------------------------------------------
         *  FREQUENCY: Get labels for axis limits, then draw the axis
         *------------------------------------------------------------*/

        MyPS_XORIG  =  100;
        MyPS_YORIG  =  100;             
        MyPS_WIDTH  =  450;            
        MyPS_HEIGHT =  220;            

        N    = count_fre[par];
        if (N >= MaxPOINTS) {
           printf("  error (2dPlot) partial=%d has more points than allowed\n",
                   par);
           printf("       number of freq. break points = %d,  maximum = %d\n",
                   N, MaxPOINTS);
           exit(1);
        }
        xmin = 0.0; 
        xmax = time_fre[par][N-1];

        ymin = 1e+9;
        ymax = 0.0;
        for (j=0; j<N; j++) {
          if(ymin > 0.9*value_fre[par][j]) ymin = 0.9*value_fre[par][j];
          if(ymax < 1.1*value_fre[par][j]) ymax = 1.1*value_fre[par][j];
        }


        if (ymax - ymin < 1e-07) ymax = ymin + 0.01;
        if (xmax - xmin < 1e-07) {
           printf("  error (2dPlot) partial=%d,  time max - min = 0 \n", par);
           exit(1);
        }

        sprintf(cxmin,"%d",(int) xmin);
        sprintf(cxmax,"%d",(int) xmax);
        setAxisNumbers(&ymin, &ymax, cymin, cymax);
	PS_drawAxes(psFile, cxmin, cxmax, cymin, cymax);



        /* ------------------------
         *  write the plot caption
         * ------------------------*/

	fprintf(psFile,"\n");
        xx = MyPS_XORIG + MyPS_WIDTH/3;
        yy = MyPS_YORIG + MyPS_HEIGHT + fnts;
        fprintf(psFile,"%f  %f  moveto \n", xx, yy);
        fprintf(psFile,"(Frequency:  Partial %d) show \n", par);



        /* ---------------------
         *  write the plot data
         * ---------------------*/

	fprintf(psFile,"               \n");
	fprintf(psFile,"%% Plot data    \n");
	fprintf(psFile,"1 setlinewidth \n");

	dx = MyPS_WIDTH /(xmax - xmin);
	dy = MyPS_HEIGHT/(ymax - ymin);

	for(i=0; i < N; i++) {
                xx = MyPS_XORIG + ( time_fre[par][i] - xmin)*dx;
                yy = MyPS_YORIG + (value_fre[par][i] - ymin)*dy;
                if (!i) fprintf(psFile,"%f  %f  moveto \n", xx, yy);
                if ( i) fprintf(psFile,"%f  %f  lineto \n", xx, yy);
	}
	fprintf(psFile,"stroke \n");


     }  /*   Next Partial  */



     /* --------------------
      *  PostScript Trailer
      * --------------------*/

      fprintf(psFile,"\n");
      fprintf(psFile,"showpage \n");
      fprintf(psFile,"%%%%Trailer \n");
      fprintf(psFile,"%%%%Pages: %d \n", currentPage);
      fprintf(psFile,"%%%%EOF \n");



}


