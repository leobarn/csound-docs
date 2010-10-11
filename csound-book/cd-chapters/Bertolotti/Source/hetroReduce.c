
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




/**************************************************************************
 *
 * Perform hetro-data smoothing + division into base + supplememntary data
 *
 **************************************************************************/



#include <string.h>
#include <stdlib.h>
#include "allocate.h"

                                /*  From ``sub_ioData.c''      */
extern int  readData();         /*  Read  hetrodyne data file */
extern void writeData();        /*  Write hetrodyne data file */


                                /*  From ``hetroMath.c''      */
extern void  smoothData();      /*  Smooth data with centered stencil */
extern float myWindow();        /*  My window function        */
extern float projectSin();      /*  DFT sine                  */
extern float projectCos();      /*  DFT cosine                */
extern void  interpolate();     /*  Linear interpolation      */
extern void  chebyCollPoints(); /*  Define Chebyshev Coll. Points */
extern void  liftUp();          /*  Make distribution positive */


#define   NumCOLL  50           /*  Number of points in smoothed data   */



main(argc, argv)
  int   argc;
  char *argv[];
{
      char   fileDefault[7];
      char   fileExtensionB[13];
      char   fileExtensionA[13];
      char   fileExtensionS[13];
      char   fileExtensionR[13];
      char   fileNameOutB[280];
      char   fileNameOutA[280];
      char   fileNameOutS[280];
      char   fileNameOutR[280];
      char  *fileNameIn, *fileNameOut;
      FILE  *fileIn, *fileOutB, *fileOutS, *fileOutA, *fileOutR;

short int    time_amp[MaxPARTIALS][MaxPOINTS];    /* Original data  */
short int    time_fre[MaxPARTIALS][MaxPOINTS];
short int   value_amp[MaxPARTIALS][MaxPOINTS];
short int   value_fre[MaxPARTIALS][MaxPOINTS];
short int   count_amp[MaxPARTIALS];
short int   count_fre[MaxPARTIALS];

short int    time_ampB[MaxPARTIALS][NumCOLL];    /* Base, smoothed data  */
short int    time_freB[MaxPARTIALS][NumCOLL];
short int   value_ampB[MaxPARTIALS][NumCOLL];   
short int   value_freB[MaxPARTIALS][NumCOLL];
short int   count_ampB[MaxPARTIALS];
short int   count_freB[MaxPARTIALS];

short int    time_ampS[MaxPARTIALS][MaxPOINTS];   /* Sustain          */
short int    time_freS[MaxPARTIALS][MaxPOINTS];  
short int   value_ampS[MaxPARTIALS][MaxPOINTS];  
short int   value_freS[MaxPARTIALS][MaxPOINTS];
short int   count_ampS[MaxPARTIALS];
short int   count_freS[MaxPARTIALS];

short int    time_ampA[MaxPARTIALS][MaxPOINTS];   /* Attack + Decay   */
short int    time_freA[MaxPARTIALS][MaxPOINTS];  
short int   value_ampA[MaxPARTIALS][MaxPOINTS];   
short int   value_freA[MaxPARTIALS][MaxPOINTS];  
short int   count_ampA[MaxPARTIALS];
short int   count_freA[MaxPARTIALS];
short int    wfreq_sta[MaxPARTIALS];   /* start of      window > 0  region  */
short int    wfreq_end[MaxPARTIALS];   /* end   of      window > 0  region  */
short int    wampl_sta[MaxPARTIALS];   /* start of      window > 0  region  */
short int    wampl_end[MaxPARTIALS];   /* end   of      window > 0l region  */
short int    cfreq_sta[MaxPARTIALS];   /* start of comp-window null region  */
short int    cfreq_end[MaxPARTIALS];   /* end   of comp-window null region  */
short int    campl_sta[MaxPARTIALS];   /* start of comp-window null region  */
short int    campl_end[MaxPARTIALS];   /* end   of comp-window null region  */

short int   ff[MaxPOINTS];   /* scratch space */
short int   f1[MaxPOINTS];   /* scratch space */

short int   collPoints[MaxPOINTS];
short int   nCol, numP;
short int   i, j, k, par, partials;

short int   tMin, tMax, attackTime=0, decayTime=0;
      int   timeMax, ampMax, freqMin, freqMax, freqMean;
      float dt, sum, window, window_comp, df, amp, diff;

short int   nc=0;
short int   red=1;




      /*------------------------------
       *  Set default output file name
       *------------------------------*/
      strcpy(fileDefault   , "reduce");
      strcpy(fileExtensionB, "_base.het");
      strcpy(fileExtensionA, "_sup_a.het");
      strcpy(fileExtensionS, "_sup_s.het");
      strcpy(fileExtensionR, "_sup_r.het");
      fileNameOut = fileDefault;




      /*---------------------
       *  Parse command line      
       *---------------------*/
      i = 0;
      j = 1;
      while (j<argc){
         switch(argv[j][0]){
            case('-'): if (j+1==argc) {
                          printf(" error: option %s has no value \n", argv[j]);
                          exit(1);
                       } else if (argv[j+1][0]=='-') {
                          printf(" error: option %s has no value \n", argv[j]);
                          exit(1);
                       } 
                       switch(argv[j][1]){
                         case('o'): fileNameOut = argv[j+1];
                                    j+=2;
                                    break;
                         case('s'): red         = atoi(argv[j+1]);
                                    j+=2;
                                    break;
                         case('a'): attackTime  = atoi(argv[j+1]);
                                    j+=2;
                                    break;
                         case('r'): decayTime   = atoi(argv[j+1]);
                                    j+=2;
                                    break;
                         default:   printf("Unrecognized option %s \n",argv[j]);
                                    printf("Allowed: -o outputFileName \n");
                                    printf("         -s smoothingFactor \n");
                                    printf("         -a attack Time  (ms)\n");
                                    printf("         -r release Time (ms)\n");
                                    exit(1);
                       }
                       break;

              default: if(i==0) fileNameIn = argv[j];
                       if(i>0) {
                         printf("Unrecognized command %s \n", argv[j]);
                         exit(1);
                       }
                       i++;
                       j++;
         }
      }
      if (i<1) {
        printf("\n error: Input file must be specified \n\n");
        exit(1);
      }



      if ((fileIn = fopen(fileNameIn,"rb"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameIn);
        exit(0);
      }



      /* ---------------------------------------------
       *   Build the output file names and open files
       * ---------------------------------------------*/
       strcpy(fileNameOutB , fileNameOut);
       strcat(fileNameOutB , fileExtensionB);
       strcpy(fileNameOutA , fileNameOut);
       strcat(fileNameOutA , fileExtensionA);
       strcpy(fileNameOutS , fileNameOut);
       strcat(fileNameOutS , fileExtensionS);
       strcpy(fileNameOutR , fileNameOut);
       strcat(fileNameOutR , fileExtensionR);


      if ((fileOutB = fopen(fileNameOutB,"w"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameOutB);
        exit(0);
      }
      if ((fileOutS = fopen(fileNameOutS,"w"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameOutS);
        exit(0);
      }
      if ((fileOutA = fopen(fileNameOutA,"w"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameOutA);
        exit(0);
      }
      if ((fileOutR = fopen(fileNameOutR,"w"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameOutR);
        exit(0);
      }



     /*-----------------------
      *   Read hetrodyne data
      *-----------------------*/

      partials = readData(fileIn, time_amp,  time_fre, 
                                  value_amp, value_fre, 
                                  count_amp, count_fre,
                                  &timeMax , &ampMax,   
                                  &freqMin , &freqMax);



     /*-------------------------------------------------------------------
      *  Eliminate time duplications that erroneously occur in hetro files
      *-------------------------------------------------------------------*/

      for(par=0; par<partials; par++){  
         if (time_fre[par][count_fre[par]-2]==
             time_fre[par][count_fre[par]-1]) count_fre[par]--;
         if (time_amp[par][count_amp[par]-2]==
             time_amp[par][count_amp[par]-1]) count_amp[par]--;
      }




     /*--------------------------------------------------
      *  Set the integration interval used for smoothing
      *--------------------------------------------------*/

      if (red<0) red = 0;




     /*
      *  ===============================================================
      *  Loop through every partial, SMOOTH FREQUENCY, DEFINE DIFFERENCE
      *  ===============================================================
      */

      for(par=0; par<partials; par++){   

         numP = count_fre[par];
         tMin = time_fre[par][0];
         tMax = time_fre[par][numP-1];

         count_freA[par] = numP;
         count_freS[par] = numP;
         smoothData(numP, time_fre[par], value_fre[par], red, ff);

         for (i=0;i<numP;i++){
             value_freB[par][i] =  ff[i];
             value_freA[par][i] =  value_fre[par][i];
             value_freS[par][i] =  value_fre[par][i];
              time_freA[par][i] =   time_fre[par][i];
              time_freS[par][i] =   time_fre[par][i];

             /* Detect window region */
             window = myWindow(time_fre[par][i], tMax, attackTime, decayTime);
             if (i<numP/2 && window<0.0001) wfreq_sta[par] = i;
             if (i>numP/2 && window>0.0001) wfreq_end[par] = i;
             if (i<numP/2 && window<0.9999) cfreq_sta[par] = i;
             if (i>numP/2 && window>0.9999) cfreq_end[par] = i;


              if(par==-1)
               printf("i=%d,  tt: %d   orig=%d  smooth=%d \n",
                       i, time_fre[par][i], value_fre[par][i], ff[i]);
         }



        /*-----------------------------------------------------------
         *  Conserve memory:  Transfer smooth data to NumCOLL points
         *-----------------------------------------------------------*/

        chebyCollPoints(tMin, tMax, NumCOLL, time_freB[par]);
        interpolate(numP   , time_fre[par] , ff , 
                    NumCOLL, time_freB[par], value_freB[par]);
        count_freB[par] = NumCOLL;

      }



     /*
      *  ===============================================================
      *  Loop through every partial, SMOOTH AMPLITUDE, DEFINE DIFFERENCE
      *  ===============================================================
      */

      for(par=0; par<partials; par++){  

         numP = count_amp[par];
         tMin = time_amp[par][0];
         tMax = time_amp[par][numP-1];

         count_ampA[par] = numP;
         count_ampS[par] = numP;
         smoothData(numP, time_amp[par], value_amp[par], red, ff);

         for (i=0;i<numP;i++){
             window = myWindow(time_amp[par][i], tMax, attackTime, decayTime);
             window_comp = 1 - window;

             value_ampB[par][i] = ff[i];
             value_ampA[par][i] = (value_amp[par][i] - ff[i])*window_comp;
             value_ampS[par][i] = (value_amp[par][i] - ff[i])*window;
              time_ampA[par][i] =   time_amp[par][i];
              time_ampS[par][i] =   time_amp[par][i];

             /* Detect window region */
             if (i<numP/2 && window<0.0001) wampl_sta[par] = i;
             if (i>numP/2 && window>0.0001) wampl_end[par] = i;
             if (i<numP/2 && window<0.9999) campl_sta[par] = i;
             if (i>numP/2 && window>0.9999) campl_end[par] = i;

              if(par<0)
               printf("i=%d,  tt: %d  %d  %d   window=%f \n",
                          i,    tMin, time_amp[par][i], tMax, window);
         }



        /*-----------------------------------------------------------
         *  Conserve memory:  Transfer smooth data to NumCOLL points
         *-----------------------------------------------------------*/

        chebyCollPoints(tMin, tMax, NumCOLL, time_ampB[par]);
        interpolate(numP   , time_amp[par] , ff, 
                    NumCOLL, time_ampB[par], value_ampB[par]);
        count_ampB[par] = NumCOLL;



        /*-----------------------------------------------------
         *   Make the attack  amplitude data positive definite
         *   Make the release amplitude data positive definite
         *   Make the sustain amplitude data positive definite
         *-----------------------------------------------------*/

         liftUp(            0 ,  campl_sta[par], value_ampA[par], f1);
         liftUp(campl_end[par], count_ampA[par], value_ampA[par], f1);
         liftUp(wampl_sta[par],  wampl_end[par], value_ampS[par], f1);


      }
 


     /*----------------------
      *  Output Data: Part I
      *----------------------*/

      /*----------------------------------
       *  Compress and write sustain data
       *----------------------------------*/

      for(par=0; par<partials; par++){  
        j = wfreq_sta[par]-1;
        k = wfreq_end[par]+1;
        for (i=j; i<k; i++){
            time_freS[par][i-j+1] =  time_freS[par][i] ;
           value_freS[par][i-j+1] = value_freS[par][i] ;
        }
         time_freS[par][k-j+1] =  time_freS[par][count_freS[par]-1] ;
        value_freS[par][k-j+1] = value_freS[par][k-j] ;
        value_freS[par][0]     = value_freS[par][1] ;
        count_freS[par] = k-j+2;

        j = wampl_sta[par]-1;
        k = wampl_end[par]+1;
        for (i=j; i<k; i++){
            time_ampS[par][i-j+1] =  time_ampS[par][i] ;
           value_ampS[par][i-j+1] = value_ampS[par][i] ;
        }
         time_ampS[par][k-j+1] =  time_ampS[par][count_ampS[par]-1] ;
        value_ampS[par][k-j+1] = value_ampS[par][k-j] ;
        count_ampS[par] = k-j+2;
      }

      writeData(fileOutS, time_ampS,  time_freS, 
                          value_ampS, value_freS, 
                          count_ampS, count_freS, partials);




      /*-------------------------
       *  Split Attack and Decay
       *-------------------------*/
      for(par=0; par<partials; par++){  

         time_freA[par][cfreq_sta[par]+1] =  time_fre[par][count_fre[par]-1]; 
        value_freA[par][cfreq_sta[par]+1] = value_freA[par][cfreq_sta[par]];
        count_freA[par] = cfreq_sta[par]+2;
  
         time_ampA[par][campl_sta[par]+1] = time_amp[par][count_amp[par]-1];
        value_ampA[par][campl_sta[par]  ] = 0;
        value_ampA[par][campl_sta[par]+1] = 0;
        count_ampA[par] = campl_sta[par]+2;
  
         time_fre[par][0] = 0;
        value_fre[par][0] = value_fre[par][cfreq_end[par]];
        j = count_fre[par] - cfreq_end[par];
        for (i=0; i<j; i++){
            time_fre[par][i+1] =  time_fre[par][i + cfreq_end[par]] ;
           value_fre[par][i+1] = value_fre[par][i + cfreq_end[par]] ;
        }
        count_fre[par] = j+1;

         time_amp[par][0] = 0;
        value_amp[par][0] = 0;
        j = count_amp[par] - campl_end[par];
        for (i=0; i<j; i++){
            time_amp[par][i+1] =  time_ampA[par][i + campl_end[par]] ;
           value_amp[par][i+1] = value_ampA[par][i + campl_end[par]] ;
        }
        count_amp[par] = j+1;

      }
 

      writeData(fileOutA, time_ampA,  time_freA, 
                          value_ampA, value_freA, 
                          count_ampA, count_freA, partials);


      writeData(fileOutR, time_amp,  time_fre, 
                          value_amp, value_fre, 
                          count_amp, count_fre, partials);



     /*-----------------------
      *  Output Data: Part II
      *-----------------------*/

      for(par=0; par<partials; par++){  
        for (i=0; i<NumCOLL; i++){
          time_amp[par][i] =  time_ampB[par][i]; 
          time_fre[par][i] =  time_freB[par][i]; 
         value_amp[par][i] = value_ampB[par][i]; 
         value_fre[par][i] = value_freB[par][i]; 
        }
      }

      writeData(fileOutB,  time_amp ,  time_fre, 
                          value_amp , value_fre, 
                          count_ampB, count_freB, partials);



}



