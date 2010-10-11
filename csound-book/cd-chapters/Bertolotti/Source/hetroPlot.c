
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




/*-----------------------------------------------------------------------*/
/*                                                                       */
/*      hetroPlot.c   -  3-D plots of hetrodyne data                     */
/*                                                                       */
/*      syntax   hetroplot -o outFileName   myDataFile.hetro             */ 
/*      (if -o option is omitted , the default output name is            */
/*                     ``hetro_plot.eps''                                */
/*       if outFileName does not end in ``.eps'', then ``.eps'' added)   */
/*                                                                       */
/*                                                                       */
/*      by:   Fabio Bertolotti                                           */
/*            (email:  fabio.bertolotti@tdlr.de)                         */
/*                                                                       */
/*            Dec 1995.                                                  */
/*                                                                       */
/*-----------------------------------------------------------------------*/


#include "allocate.h"
#include <time.h>
 

#define  SCALE  200                     /* SCALE TO POST_SCRIPT SIZE */
#define  ZSCALE 0.4                     /* 1/2 FACTOR IN Z HEIGHT    */



struct CAMER {
    float    cx,  cy,  cz ;       /*  Camara position        */
    float    ax,  ay,  az ;       /*  Camara amiming point   */
    float   n1x, n1y, n1z ;       /*                         */
    float   n2x, n2y, n2z ;       /*  Unit normals of the    */
    float   n3x, n3y, n3z ;       /*  camara plane           */
    float   zoom;                 /*  Zoom lens setting      */
} camara;



extern int  readData();
       void initCamara();
       void drawEnclosure();   
       void project();
       void linePlot3D();
       void makePlot2D();
       void project();



main(argc, argv)
  int   argc;
  char *argv[];
{
      char   fileNameIn[128], fileNameOut[128]; 
      FILE  *fileIn, *fileOut;

short int    time_amp[MaxPARTIALS][MaxPOINTS];
short int    time_fre[MaxPARTIALS][MaxPOINTS];
short int   value_amp[MaxPARTIALS][MaxPOINTS];
short int   value_fre[MaxPARTIALS][MaxPOINTS];
short int   count_amp[MaxPARTIALS];
short int   count_fre[MaxPARTIALS];

      int   itimeMax, ifreqMax, ifreqMin, iampMax;
      int   fill=0, Divide=1, Dim2=0;
      float freqMax, freqMid, freqMin, timeMax, maxPar;
      float amplMax, amplMid, amplMin;
      float x[MaxPOINTS];
      float y[MaxPOINTS];
      float z[MaxPOINTS];
     
      int   i=0, j=1, j0, j1, par, partials;

      char      *today;   
      struct tm *timePtr;
      time_t    lt;


      lt = time('\0');                     /*  Get the Date  */
      timePtr = localtime(&lt);
      today = asctime(timePtr);
      


      /*---------------------
       *  Default file names      
       *---------------------*/

      strcpy(fileNameOut, "hetro_plot.eps");




      /*---------------------
       *  Parse command line      
       *---------------------*/

      while (j<argc){
         switch(argv[j][0]){
            case('-'): switch(argv[j][1]){
                          case('f'): fill = 1;
                                     j++;
                                     break;
                          case('2'): Dim2=1;
                                     j++;
                                     break;
                          case('n'): Divide=0;
                                     j++;
                                     break;
                          case('o'): if(j==argc) {
                                      printf("error: -o option needs a file\n");
                                      exit(1);
                                     } 
                                     strcpy(fileNameOut, argv[j+1]);
                                     j+=2;
                                     break;
                          default:  printf("Unrecognized option %s ", argv[j]);
                                    printf("Allowed are -o outputFileName \n");
                                    printf("            -fill             \n");
                                    printf("            -nodivide         \n");
                                    printf("            -2d               \n");
                                    exit(1);
                       }
                       break;

              default: if(i==0) strcpy(fileNameIn, argv[j]);
                       if(i>0) {
                         printf("Unrecognized command %s \n", argv[j]);
                         exit(1);
                       }
                       i++;
                       j++;
         }
      }
      if (i<1) {
        printf("\n error: No input file specified \n\n");
        exit(1);
      }

     



      /* --------------------------
       *   Open files and read data
       * --------------------------*/

      if ((fileIn = fopen(fileNameIn,"rb"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameIn);
        exit(0);
      }

      if ((j=strlen(fileNameOut)) > 4 && strcmp(fileNameOut+j-4,".eps"))
         strcat(fileNameOut,".eps");
      if ((fileOut = fopen(fileNameOut,"w"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameOut);
        exit(0);
      }
  
      partials = readData(fileIn, time_amp,  time_fre, 
                                  value_amp, value_fre, 
                                  count_amp, count_fre,
                                  &itimeMax, &iampMax,   
                                  &ifreqMin, &ifreqMax);




      /* -------------------------
       *  Write PostScript header 
       *-------------------------*/

      fprintf(fileOut,"%%!PS-Adobe-2.0 \n");
      fprintf(fileOut,"%%%%Title: file = %s \n", fileNameIn);
      fprintf(fileOut,"%%%%Creator: displayhetro \n");
      fprintf(fileOut,"%%%%BoundingBox: 050 050 560 760 \n");
      fprintf(fileOut,"%%%%Orientation: Portrait \n");
      fprintf(fileOut,"%%%%Pages: (atend) \n");
      fprintf(fileOut,"%%%%PageOrder: Ascend \n");
      fprintf(fileOut,"%%%%EndComments \n");



      /* -----------------------------------------------------------------
       *   If 2d plots are wanted,  
       * -----------------------------------------------------------------*/
      
       if (Dim2) {
          makePlot2D(fileOut,  today, fileNameIn, partials, 
                                      time_amp,  time_fre, 
                                      value_amp, value_fre, 
                                      count_amp, count_fre);
          fclose(fileIn);
          fclose(fileOut);
          exit(0);
       }




      /* -----------------------------------------------------------------
       *   Find fundamental frequency.  Skip over attack portion of sound
       * -----------------------------------------------------------------*/

      j0 = 0;
      j1 = 0;
      freqMid = 0;
      itimeMax = time_fre[0][count_fre[0]-1];
      while(j0 < count_fre[0] && time_fre[0][j0] < itimeMax* 0.35) j0++;
      while(j1 < count_fre[0] && time_fre[0][j1] < itimeMax* 0.85) j1++;
      for (j=j0; j<j1; j++)
         freqMid += value_fre[0][j];
      if (j1-j0) freqMid = (float) freqMid/ (float) (j1-j0);




      /* ---------------------------
       *   Get maximums and minimums
       * ----------------------------*/

      timeMax = itimeMax;
      amplMax = iampMax;
      amplMin = 0; 
      if(Divide) {
        freqMax = ifreqMax;
        freqMin = ifreqMin;
      } else {
        freqMax = ifreqMax*(partials-1);
        freqMin = ifreqMin;
        freqMid = (freqMax + freqMin)/2;
      }
      maxPar  = partials - 1;
      initCamara();




      /* ----------------------
       *  Write 3-D Plot header 
       *-----------------------*/

      fprintf(fileOut," \n");
      fprintf(fileOut,"%%%%Page: 1 1 \n\n");
      fprintf(fileOut,"1.0 setlinewidth  \n");
      fprintf(fileOut," \n");
      fprintf(fileOut,"/Times-Roman findfont 16 scalefont setfont \n");
      fprintf(fileOut," 60 740 moveto \n");
      fprintf(fileOut,"(%s) show \n", fileNameIn);
      fprintf(fileOut," 60 718 moveto \n ");
      fprintf(fileOut,"(%s) show \n", today);
      fprintf(fileOut," \n");
      fprintf(fileOut,"gsave \n");
      fprintf(fileOut," /Times-Roman findfont 18 scalefont setfont \n");
      fprintf(fileOut," 5 rotate \n");
      fprintf(fileOut," 250 605 moveto \n");
      fprintf(fileOut,"(Amplitude) show \n");
      if (Divide) {
         fprintf(fileOut," 175 286 moveto \n");
         fprintf(fileOut,"(Frequency / (partial+1)) show \n");
      } else {
         fprintf(fileOut," 222 286 moveto \n");
         fprintf(fileOut,"(Frequency) show \n");
      }
      fprintf(fileOut,"grestore \n");
      fprintf(fileOut," \n");





      /* ---------------------------------------------------
       *   Top graph:  Amplitudes vs Time vs Partial number
       * ---------------------------------------------------*/

      fprintf(fileOut," \n ");
      fprintf(fileOut,"305 500 translate \n");
      drawEnclosure(fileOut, timeMax, partials-1, amplMin, -1.0, amplMax);
      for(par=0; par<partials; par++){
        for(i=0; i<count_amp[par]; i++){
           x[i] = par;
           y[i] = time_amp[par][i];
           z[i] = value_amp[par][i];
        }
        linePlot3D(fileOut, count_amp[par], x, y, z, 
                                        0.0, maxPar,  
                                        0.0, timeMax, 
                                        0.0, amplMax, fill);
      }




      /* ----------------------------------------------------------
       *   Bottom graph:  Frequencies/f0 vs Time vs Partial number
       * ----------------------------------------------------------*/

      fprintf(fileOut," \n ");
      fprintf(fileOut,"000 -320 translate \n");
      drawEnclosure(fileOut, timeMax, partials-1, freqMin, freqMid, freqMax);
      for(par=0; par<partials; par++){
        for(i=0; i<count_fre[par]; i++){
           x[i] = par;
           y[i] = time_fre[par][i];
           if (Divide){
             z[i] = value_fre[par][i]/(par+1);
           } else {
             z[i] = value_fre[par][i];
           }
        }
        linePlot3D(fileOut, count_fre[par], x, y, z, 
                                        0.0, maxPar,  
                                        0.0, timeMax, 
                                        freqMin, freqMax, 0);
      }




     /* --------------------
      *  PostScript Trailer
      * --------------------*/

      fprintf(fileOut,"\n");
      fprintf(fileOut,"showpage \n");
      fprintf(fileOut,"%%%%Trailer \n");
      fprintf(fileOut,"%%%%Pages: 1 \n");
      fprintf(fileOut,"%%%%EOF \n");

      fclose(fileIn);
      fclose(fileOut);
}



void initCamara()
{
      double  dis;


/*  CAMARA POSITION  */
      camara.cx =  3.5;
      camara.cy = -1.6;
      camara.cz =  1.0;
    

/*  CAMARA AIMIMG POINT */
      camara.ax = 0.5;
      camara.ay = 0.5;
      camara.az = 0.0;


/*  CAMARA ZOOM        */
      camara.zoom =  5;


/*  DETERMINE UNIT VECTOR: FOTO-PLANE NORMAL  */
      camara.n3x = camara.ax - camara.cx;
      camara.n3y = camara.ay - camara.cy;
      camara.n3z = camara.az - camara.cz;
      dis = sqrt( (double) camara.n3x * camara.n3x + 
                  (double) camara.n3y * camara.n3y + 
                  (double) camara.n3z * camara.n3z );
      camara.n3x = camara.n3x / dis;
      camara.n3y = camara.n3y / dis;
      camara.n3z = camara.n3z / dis;


/*  DETERMINE UNIT VECTOR: FOTO-PLANE X-AXIS  =  n3 x k  */
      camara.n1x =  camara.n3y;
      camara.n1y = -camara.n3x;
      camara.n1z =           0;
      dis = sqrt( (double) camara.n1x * camara.n1x + 
                  (double) camara.n1y * camara.n1y + 
                  (double) camara.n1z * camara.n1z );
      camara.n1x = camara.n1x / dis;
      camara.n1y = camara.n1y / dis;
      camara.n1z = camara.n1z / dis;


/*  DETERMINE UNIT VECTOR: FOTO-PLANE Y-AXIS  */
      camara.n2x =  camara.n1y * camara.n3z - camara.n1z * camara.n3y;
      camara.n2y =  camara.n1z * camara.n3x - camara.n1x * camara.n3z;
      camara.n2z =  camara.n1x * camara.n3y - camara.n1y * camara.n3x;
}





void linePlot3D(fileOut, numPoints, x, y, z, 
                                 xMin, xMax, 
                                 yMin, yMax, 
                                 zMin, zMax, fill)
  FILE  *fileOut;
  int   numPoints, fill;
  float x[MaxPOINTS];
  float y[MaxPOINTS];
  float z[MaxPOINTS];
  float xMin, xMax;
  float yMin, yMax;
  float zMin, zMax;
{

      int      n;
      float    px,  py,  pz, vx, vy;
      double   dx, dy, dz;

      dx = xMax - xMin;
      dy = yMax - yMin;
      dz = zMax - zMin;


      fprintf(fileOut, "\n newpath \n");
      px = 0;
      py = 0;
      pz = 0;
      if(abs(dx)>0) 
        px = (x[0] - xMin)/dx;
      if(abs(dy)>0) 
        py = (y[0] - yMin)/dy;
      if(abs(dz)>0) 
        pz = (z[0] - zMin)/dz * ZSCALE;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);


      for (n=1; n < numPoints; n++){
        px = 0;
        py = 0;
        pz = 0;
        if(abs(dx)>0) 
          px = (x[n] - xMin)/dx;
        if(abs(dy)>0) 
          py = (y[n] - yMin)/dy;
        if(abs(dz)>0) 
          pz = (z[n] - zMin)/dz * ZSCALE;
        project (px, py, pz, &vx, &vy);
        fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);
      }

      if (fill==1) {
         fprintf(fileOut,"closepath gsave 1.0 setgray fill \n");
         fprintf(fileOut,"grestore \n");
      } 
      fprintf(fileOut,"stroke \n");

}





void project (px, py, pz, vx, vy)
   float  px,  py,  pz;
   float *vx, *vy;
{

      float    rx,  ry,  rz, vz;


/*  REALTIVE VECTOR  */
      rx = px - camara.cx;
      ry = py - camara.cy;
      rz = pz - camara.cz;


/*  PROJECT  */
      *vx = rx*camara.n1x + ry*camara.n1y + rz*camara.n1z;
      *vy = rx*camara.n2x + ry*camara.n2y + rz*camara.n2z;
       vz = rx*camara.n3x + ry*camara.n3y + rz*camara.n3z;


/*  SCALE  */
      *vx = camara.zoom * *vx / vz;
      *vy = camara.zoom * *vy / vz;

}





void  drawEnclosure(fileOut, timeMax, partials, fmin, fmid, fmax)
    FILE  *fileOut;
    float timeMax;
    int   partials; 
    float fmin, fmid, fmax;
{
      char  label[20];
      float px, py, pz, vx, vy;


     /*
      *   Labels
      */

      fprintf(fileOut," \n ");
      fprintf(fileOut,"/Times-Roman findfont 14 scalefont setfont \n");

      px =  0.5;
      py = -0.2;
      pz = -0.02;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut," \n");
      fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
      fprintf(fileOut,"(Partials) show \n");

      px =     0;
      py = -0.08;
      pz = -0.02;
      if( fmin >= 0) px = 0.05;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut," \n");
      fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
      fprintf(fileOut,"(0) show \n");

      px =     1;
      py = -0.08;
      pz = -0.02;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut," \n");
      fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
      fprintf(fileOut,"(%d) show \n", partials);

      px =  1.08;
      py =   0.5;
      pz = -0.02;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut," \n");
      fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
      fprintf(fileOut,"(Sec) show \n");

      px =  1.06;
      py =     0;
      pz = -0.02;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut," \n");
      fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
      fprintf(fileOut,"(0) show \n");

      px =  1.06;
      py =     1;
      pz = -0.02;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut," \n");
      fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
      fprintf(fileOut,"(%6.3f) show \n", timeMax/1000);


      if(fmid>0 && (fmax - fmin)){
        px = 0;
        pz = ZSCALE * ((float) fmid - fmin) / (fmax - fmin) ;

        py = -0.03;
        project (px, py, pz, &vx, &vy);
        fprintf(fileOut," \n");
        fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);

        py =  0.00;
        project (px, py, pz, &vx, &vy);
        fprintf(fileOut,"%g  %g  lineto stroke \n", SCALE*vx, SCALE*vy);

        sprintf(label, "%g", (int) (fmid*10)/10.0);
        py  = -0.03*strlen(label) -0.01;
        project (px, py, pz, &vx, &vy);
        fprintf(fileOut," \n");
        fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
        fprintf(fileOut,"(%s) show \n", label);
      }


      if(fmin>0){
        sprintf(label, "%g", (int) (fmin*10)/10.0);
        px =  0;
        py = -0.03*strlen(label) -0.01;
        pz =  0.025;
        project (px, py, pz, &vx, &vy);
        fprintf(fileOut," \n");
        fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
        fprintf(fileOut,"(%s) show \n", label);
      }


      if(fmax>0){
        sprintf(label, "%g", (int) (fmax*10)/10.0);
        px =  0;
        py = -0.03*strlen(label) -0.01;
        pz =  0.98 * ZSCALE;
        project (px, py, pz, &vx, &vy);
        fprintf(fileOut," \n");
        fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);
        fprintf(fileOut,"(%s) show \n", label);
      }



      fprintf(fileOut," \n");
      fprintf(fileOut,"newpath \n");
      px = 0;
      py = 0;
      pz = 0;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  moveto \n", SCALE*vx, SCALE*vy);

      px = 1;
      py = 0;
      pz = 0;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 1;
      py = 1;
      pz = 0;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 0;
      py = 1;
      pz = 0;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 0;
      py = 0;
      pz = 0;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 0;
      py = 0;
      pz = 1 * ZSCALE;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 0;
      py = 1;
      pz = 1 * ZSCALE;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 0;
      py = 1;
      pz = 0;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 0;
      py = 1;
      pz = 1 * ZSCALE;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 1;
      py = 1;
      pz = 1 * ZSCALE;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);

      px = 1;
      py = 1;
      pz = 0;
      project (px, py, pz, &vx, &vy);
      fprintf(fileOut,"%g  %g  lineto \n", SCALE*vx, SCALE*vy);
      fprintf(fileOut,"stroke \n");
      fprintf(fileOut,"       \n");
}


