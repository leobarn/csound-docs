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






#include "allocate.h"


extern int   readData();         /*  from file ioData.c  */
extern void  writeData();        /*  from file ioData.c  */

       void  mix();
short  int   interpolate();
       void  timeStretch();
       void  readWeights(); 
       int   whereIs(); 
       char* getToken(); 



int main(argc, argv)
    int   argc;
    char *argv[];
{
      char   fileDefault[18];
      char   fileDefaultInfo[17];
      char   fileDefaultScale[3];
      char  *fileNameIn0, *fileNameIn1;
      char  *fileNameOut, *fileNameInfo;
      char  *fileNameScale; 
      FILE  *fileIn0, *fileIn1, *fileOut, *fileInfo, *fileScale;

short int    time_amp0[MaxPARTIALS][MaxPOINTS];
short int    time_fre0[MaxPARTIALS][MaxPOINTS];
short int   value_amp0[MaxPARTIALS][MaxPOINTS];
short int   value_fre0[MaxPARTIALS][MaxPOINTS];
short int   count_amp0[MaxPARTIALS];
short int   count_fre0[MaxPARTIALS];

short int    time_amp1[MaxPARTIALS][MaxPOINTS];
short int    time_fre1[MaxPARTIALS][MaxPOINTS];
short int   value_amp1[MaxPARTIALS][MaxPOINTS];
short int   value_fre1[MaxPARTIALS][MaxPOINTS];
short int   count_amp1[MaxPARTIALS];
short int   count_fre1[MaxPARTIALS];

short int    time_ampM[MaxPARTIALS][MaxPOINTS];
short int    time_freM[MaxPARTIALS][MaxPOINTS];
short int   value_ampM[MaxPARTIALS][MaxPOINTS];
short int   value_freM[MaxPARTIALS][MaxPOINTS];
short int   count_ampM[MaxPARTIALS];
short int   count_freM[MaxPARTIALS];

      float   freq_scale0[MaxPARTIALS], deflt_fre0=0.5;
      float   freq_scale1[MaxPARTIALS], deflt_fre1=0.5;
      float   ampl_scale0[MaxPARTIALS], deflt_amp0=0.5;
      float   ampl_scale1[MaxPARTIALS], deflt_amp1=0.5;

      int   timeMax0, ampMax0, freqMin0, freqMax0;
      int   timeMax1, ampMax1, freqMin1, freqMax1;
      float dt, sum, timeFct, frac0, frac1;
      float baseFreq0, baseFreq1, baseFreq2=0; 
      int   i=0, j=1, nc, pM, p0, p1, attackOffset;
short int   par, partials, partials0, partials1;
short int   partialsMin, partialsMax;



      /*---------------------------
       *  Default output filenames
       *---------------------------*/

      strcpy(fileDefault, "data_mixed.het");
      strcpy(fileDefaultInfo, "data_mixed.info");
      strcpy(fileDefaultScale, " ");

      fileNameOut   = fileDefault;
      fileNameInfo  = fileDefaultInfo;
      fileNameScale = fileDefaultScale;


      /*---------------------
       *  Parse command line      
       *---------------------*/

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
                          case('o'): fileNameOut   = argv[j+1];
                                     j+=2;
                                     break;
                          case('w'): fileNameScale = argv[j+1];
                                     j+=2;
                                     break;
                          case('a'): attackOffset  = atoi(argv[j+1]);
                                     j+=2;
                                     break;
                          default:   printf("Unrecognized option %s ",
                                     argv[j]," (is not -o -w or -a) \n");
                                     exit(1);
                       }
                       break;

              default: if(i==0) fileNameIn0 = argv[j];
                       if(i==1) fileNameIn1 = argv[j];
                       if(i>1) {
                         printf("Unrecognized command %s \n", argv[j]);
                         exit(1);
                       }
                       i++;
                       j++;
         }
      }
      if (i<2) {
        printf("\n error: Two input input files must be specified \n\n");
        exit(1);
      }


      /*---------------------------
       *   Read data of first file 
       *---------------------------*/

      if ((fileIn0 = fopen(fileNameIn0,"rb"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameIn0);
        exit(1);
      } else {
        printf("\n reading file %s \n", fileNameIn0);
        partials0 = readData(fileIn0, time_amp0,  time_fre0, 
                                     value_amp0, value_fre0, 
                                     count_amp0, count_fre0,
                                      &timeMax0, &ampMax0,   
                                      &freqMin0, &freqMax0);
        fclose(fileIn0);
      }



      /*----------------------------
       *   Read data of second file
       *----------------------------*/

      if ((fileIn1 = fopen(fileNameIn1,"rb"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameIn1);
        exit(1);
      } else {
        printf("\n reading file %s \n", fileNameIn1);
        partials1 = readData(fileIn1,  time_amp1,  time_fre1, 
                                      value_amp1, value_fre1, 
                                      count_amp1, count_fre1,
                                       &timeMax1, &ampMax1,   
                                       &freqMin1, &freqMax1);
        fclose(fileIn1);
      }



      if ((fileOut = fopen(fileNameOut,"w"))==NULL){
        printf(" Cannot open file:  %s \n",fileNameOut);
        exit(1);
      } else {
        printf("\n output binary data to file %s \n", fileNameOut);
      }



      /*--------------------------------------------------------------------
       *   Number of common partials               
       *--------------------------------------------------------------------*/
      partialsMin = partials0;
      partialsMax = partials0;
      if (partials1 < partials0) partialsMin = partials1;
      if (partials1 > partials0) partialsMax = partials1;




      /*--------------------------------------------------------------------
       *   Read scaling factors
       *--------------------------------------------------------------------*/

      if (strcmp(fileNameScale," ")) {
         if ((fileScale = fopen(fileNameScale,"r"))==NULL){
           printf(" Cannot open file:  %s \n",fileNameScale);
           exit(1);
         } 
         readWeights(fileScale, freq_scale0, freq_scale1,
                                ampl_scale0, ampl_scale1,
                                &deflt_fre0, &deflt_fre1,
                                &deflt_amp0, &deflt_amp1);
      } else {
         for (par=0; par<MaxPARTIALS; par++){
            freq_scale0[par] = deflt_fre0;
            freq_scale1[par] = deflt_fre1;
            ampl_scale0[par] = deflt_amp0;
            ampl_scale1[par] = deflt_amp1;
         }
      }
      
      

      /*--------------------------------------------------------------------
       *   Write info to output file   
       *--------------------------------------------------------------------*/

       if ((fileInfo = fopen(fileNameInfo,"w"))==NULL){
         printf(" Cannot open file:  %s \n",fileNameInfo);
         exit(1);
       } else {
         printf(" output infformation to file %s \n\n", fileNameInfo);
       }

       fprintf(fileInfo, " Input hetrodata 0 = %s \n", fileNameIn0);
       fprintf(fileInfo, " Input hetrodata 1 = %s \n", fileNameIn1);
       fprintf(fileInfo, " Mixed hetrodata   = %s \n\n", fileNameOut);
       fprintf(fileInfo, " Mixing weights:  \n\n");
       fprintf(fileInfo, "   Partial     ----- Amplitude -----    " );
       fprintf(fileInfo,               " ----- Frequency ----- \n");
       fprintf(fileInfo, "               Weight_0     Weight_1    " );
       fprintf(fileInfo,               " Weight_0     Weight_1 \n\n");

       for (par=0; par<partialsMin; par++){
           fprintf(fileInfo,"    %3d       %9.5f    %9.5f    ",
                     par, 
                     ampl_scale0[par],
                     ampl_scale1[par]);
           fprintf(fileInfo,              "%9.5f    %9.5f     \n",
                     freq_scale0[par],
                     freq_scale1[par]);
       }
       fprintf(fileInfo," default:     %9.5f    %9.5f    ",
                     deflt_amp0, deflt_amp1);
       fprintf(fileInfo,              "%9.5f    %9.5f     \n",
                     deflt_fre0, deflt_fre1);

       fclose (fileInfo);



      /*--------------------------------------------------------------------
       *   Stretch the time axis so that both data-sets have equal support
       *   -> Keep 0 to ``attackOffset'' msec unstretched
       *--------------------------------------------------------------------*/

      for(par=0; par<partialsMin; par++){  

         timeMax0 = time_fre0[par][count_fre0[par]-1];
         timeMax1 = time_fre1[par][count_fre1[par]-1];

         if (attackOffset >= timeMax0) {
             printf("Duration of freq, partial %d  of file %s, is",
                    " shorter or equal to attackOffset=%d \n", 
                     par, fileNameIn0, attackOffset);
             exit(1);
         }
         if (attackOffset >= timeMax1) {
             printf("Duration of freq, partial %d  of file %s, is",
                    " shorter or equal to attackOffset=%d \n", 
                     par, fileNameIn1, attackOffset);
             exit(1);
         }


         if (timeMax0 > timeMax1) {
           timeStretch(attackOffset, timeMax0, count_fre1[par], time_fre1[par]);
         } else { 
           timeStretch(attackOffset, timeMax1, count_fre0[par], time_fre0[par]);
         }


         timeMax0 = time_amp0[par][count_amp0[par]-1];
         timeMax1 = time_amp1[par][count_amp1[par]-1];

         if (attackOffset >= timeMax0) {
             printf("Duration of ampl, partial %d  of file %s, is",
                    " shorter or equal to attackOffset=%d \n", 
                     par, fileNameIn0, attackOffset);
             exit(1);
         }
         if (attackOffset >= timeMax1) {
             printf("Duration of ampl, partial %d  of file %s, is",
                    " shorter or equal to attackOffset=%d \n", 
                     par, fileNameIn1, attackOffset);
             exit(1);
         }


         if (timeMax0 > timeMax1) {
           timeStretch(attackOffset, timeMax0, count_amp1[par], time_amp1[par]);
         } else { 
           timeStretch(attackOffset, timeMax1, count_amp0[par], time_amp0[par]);
         }


      }





      /*--------------------------------------------------------------------
       *   Find union of time break points for freq and ampl
       *--------------------------------------------------------------------*/

      for(par=0; par<partialsMin; par++){  
         pM = 0;
         p0 = 0;
         p1 = 0;
         while (1){
           if (time_fre0[par][p0] == time_fre1[par][p1]) {
              time_freM[par][pM++] = time_fre0[par][p0++]; p1++;
           } else if (time_fre0[par][p0] < time_fre1[par][p1]) {
              time_freM[par][pM++] = time_fre0[par][p0++];
           } else if (time_fre1[par][p1] < time_fre0[par][p0]) {
              time_freM[par][pM++] = time_fre1[par][p1++];
           }
           if(p0 == count_fre0[par]){
              for(j=p1; j<count_fre1[par]; j++)
                 time_freM[par][pM++] = time_fre1[par][j];
              break;
           }
           if(p1 == count_fre1[par]){
              for(j=p0; j<count_fre0[par]; j++)
                 time_freM[par][pM++] = time_fre0[par][j];
              break;
           }
         }
         if (pM == MaxPOINTS){
             printf("  error:  two many break points.",
                    "  MaxPOINTS=%d\n", MaxPOINTS);
             exit(1);
         }
         count_freM[par] = pM;

         pM = 0;
         p0 = 0;
         p1 = 0;
         while (1){
           if (time_amp0[par][p0] == time_amp1[par][p1]) {
              time_ampM[par][pM++] = time_amp0[par][p0++]; 
              p1++;
           } else if (time_amp0[par][p0] < time_amp1[par][p1]) {
              time_ampM[par][pM++] = time_amp0[par][p0++];
           } else if (time_amp1[par][p1] < time_amp0[par][p0]) {
              time_ampM[par][pM++] = time_amp1[par][p1++];
           }
           if(p0 == count_amp0[par]){
              for(j=p1; j<count_amp1[par]; j++)
                 time_ampM[par][pM++] = time_amp1[par][j];
              break;
           }
           if(p1 == count_amp1[par]){
              for(j=p0; j<count_amp0[par]; j++)
                 time_ampM[par][pM++] = time_amp0[par][j];
              break;
           }
         }
         if (pM == MaxPOINTS){
             printf("  error:  two many break points.",
                    "  MaxPOINTS=%d\n", MaxPOINTS);
             exit(1);
         }
         count_ampM[par] = pM;
      }

       


      /*--------------------------------------------------------------------
       *   Mix the data         
       *--------------------------------------------------------------------*/

      for(par=0; par<partialsMin; par++){  
         mix(time_amp0[par], value_amp0[par], count_amp0[par], ampl_scale0[par],
             time_amp1[par], value_amp1[par], count_amp1[par], ampl_scale1[par],
             time_ampM[par], value_ampM[par], count_ampM[par]);

         mix(time_fre0[par], value_fre0[par], count_fre0[par], freq_scale0[par],
             time_fre1[par], value_fre1[par], count_fre1[par], freq_scale1[par],
             time_freM[par], value_freM[par], count_freM[par]);
      } 

      if (partials0 > partials1){
         for(par=partialsMin+1; par<partialsMax; par++){  
            printf("WARNING: partial %d in %s  has no match",
                     par, fileNameIn0);
            printf(" (partial neglected)\n");
         }
      }
      if (partials1 > partials0){
         for(par=partialsMin+1; par<partialsMax; par++){  
            printf("WARNING:  partial %d in %s  has no match",
                     par, fileNameIn1);
            printf(" (partial neglected)\n");
         }
     }


      writeData(fileOut, time_ampM,  time_freM, 
                         value_ampM, value_freM, 
                         count_ampM, count_freM, partialsMin);


}




void mix(time0, value0, count0, frac0,
         time1, value1, count1, frac1,
         timeM, valueM, countM)
     short int    time0[MaxPOINTS],  value0[MaxPOINTS];
     short int    time1[MaxPOINTS],  value1[MaxPOINTS];
     short int    timeM[MaxPOINTS],  valueM[MaxPOINTS];
     short int    count0, count1, countM;
           float  frac0, frac1;
{
    short int inter0, inter1, time, j;

    for (j=0; j<countM; j++){
      time      = timeM[j];
      inter0    = interpolate(time, count0, time0, value0);
      inter1    = interpolate(time, count1, time1, value1);
      valueM[j] = frac0*inter0 + frac1*inter1;
    } 
}



short int interpolate(atTime, count, time, value)
      short int atTime, count;
      short int time[MaxPOINTS];
      short int value[MaxPOINTS];
{
      short int   atValue, j;
            float delta;


     j = count-1;
     while((atTime-time[j])<=0 && j) j--;
  
/*
   printf(" interpol:  j %d,  (%d, %d, %d) \n", j, time[j], atTime, time[j+1]);
*/
  

     if (j==0 && atTime < time[0]){
        printf("  error:  value %d is not in data range (%d, %d) \n",
                  time[0], time[count-1]);
        exit(1);
     }

     if (j==count-1) {
        printf("  error:  value %d is not in data range (%d, %d) \n",
                  time[0], time[count-1]);
        exit(1);
     }

     if (time[j+1] - time[j])
     delta   = (atTime - time[j])/( (float) time[j+1] - time[j] );
     atValue = value[j] + (value[j+1]-value[j]) * delta;

/*
     printf(" interpol:  j %d,  (%d, %d, %d) \n", j, value[j], atValue, value[j+1]);
*/

     return(atValue);
}






void timeStretch (attackOffset, maxTime, count, time)
           int   attackOffset; 
           int   maxTime; 
     short int   count;
     short int   time[MaxPOINTS];
{
   float  k;
   int    t,i;


   /*------------------------------------
    *   Get expansion/contraction factor
    *-----------------------------------*/
    k    = ((float) maxTime       - attackOffset)/
           ((float) time[count-1] - attackOffset);


   /* --------------------------------------------------------
    *   Time contraction or expansion factor must be positive
    * --------------------------------------------------------*/

   if (k < 0){
      printf(" Error:  illigal time-compressions factor (k<0)  = %g \n", k);
      exit(1);
   }


   /* ------------------------------------
    *   Linear Time Expansion/Contraction
    * ------------------------------------*/

    for (i=0; i<count; i++){           
      if( (t = time[i] - attackOffset) > 0 )
         time[i] = attackOffset + t*k;
    }
    time[count-1] = maxTime;
   
    

}




void  readWeights(fileIn, freq_scale0, freq_scale1,
                          ampl_scale0, ampl_scale1,
                          dflt_fre0,  dflt_fre1,
                          dflt_amp0,  dflt_amp1)
      FILE  *fileIn;
      float  freq_scale0[MaxPARTIALS], *dflt_fre0;
      float  freq_scale1[MaxPARTIALS], *dflt_fre1;
      float  ampl_scale0[MaxPARTIALS], *dflt_amp0;
      float  ampl_scale1[MaxPARTIALS], *dflt_amp1;
{
      char  ampl_mask[15];
      char  freq_mask[15];
      char  line[201];
      char  c;
      int   j, j0, par;
      float scale0, scale1;

      float default_fre0=0.5, default_fre1=0.5;
      float default_amp0=0.5, default_amp1=0.5;

      strcpy(ampl_mask,"amplitude:");     
      strcpy(freq_mask,"frequency:");    


     /*-------------------- 
      *  Initialize Scales  
      *--------------------*/
      for (par=0; par<MaxPARTIALS; par++){
          freq_scale0[par] = -1;
          freq_scale1[par] = -1;
          ampl_scale0[par] = -1;
          ampl_scale1[par] = -1;
      }
    



     /*------------------
      *  Read input file 
      *------------------*/

     while (1){
        j = 0;
        while ((c = fgetc(fileIn)) != EOF && c!='\n' && j<200){ 
                line[j++] = c;
        }
        if (c == EOF) break;
        line[j] = '\0';


        /* ------------------------
         *  Case  ``amplitude:'' 
         * -----------------------*/
    
        if ((j=whereIs(line, 0, ampl_mask))>=0) {
           if ((j0=whereIs(line, j, "default"))>0){ 
              default_amp0 = atof(getToken(line, &j0));
              default_amp1 = atof(getToken(line, &j0));
           } else {
              par    = atoi(getToken(line, &j));
              scale0 = atof(getToken(line, &j));
              scale1 = atof(getToken(line, &j));
              if (par>=MaxPARTIALS) {
                  printf(" ** error **  amplitude: partial=%d is ",
                         " larger than MaxPARTIALS=%d \n", par, MaxPARTIALS);
                  exit(1);
              }
              if (scale0 < 0) {
                  printf(" ** error **  amplitude: partial=%d is ",
                         "  scale0=%f is less than zero \n", par, scale0);
                  exit(1);
              }
              if (scale1 < 0) {
                  printf(" ** error **  amplitude: partial=%d is ",
                         "  scale1=%f is less than zero \n", par, scale1);
                  exit(1);
              }
              ampl_scale0[par] = scale0;
              ampl_scale1[par] = scale1;
           }

        }



        /* ------------------------
         *  Case  ``frequency:'' 
         * -----------------------*/
    
        if ((j=whereIs(line, 0, freq_mask))>=0) {
           if ((j0=whereIs(line, j, "default"))>0){ 
              default_fre0 = atof(getToken(line, &j0));
              default_fre1 = atof(getToken(line, &j0));
           } else {
              par    = atoi(getToken(line, &j));
              scale0 = atof(getToken(line, &j));
              scale1 = atof(getToken(line, &j));
              if (par>=MaxPARTIALS) {
                  printf(" ** error **  frequency: partial=%d is ",
                         " larger than MaxPARTIALS=%d \n", par, MaxPARTIALS);
                  exit(1);
              }
              if (scale0 < 0) {
                  printf(" ** error **  frequency: partial=%d is ",
                         "  scale0=%f is less than zero \n", par, scale0);
                  exit(1);
              }
              if (scale1 < 0) {
                  printf(" ** error **  frequency: partial=%d is ",
                         "  scale1=%f is less than zero \n", par, scale1);
                  exit(1);
              }
              freq_scale0[par] = scale0;
              freq_scale1[par] = scale1;
           }

        }

     }


     /*----------------------------------
      *  Set undefined values to default
      *----------------------------------*/

      for (par=0; par<MaxPARTIALS; par++){
          if(freq_scale0[par] < 0) freq_scale0[par] = default_fre0;
          if(freq_scale1[par] < 0) freq_scale1[par] = default_fre1;
          if(ampl_scale0[par] < 0) ampl_scale0[par] = default_amp0;
          if(ampl_scale1[par] < 0) ampl_scale1[par] = default_amp1;
      }

      *dflt_fre0 = default_fre0;
      *dflt_fre1 = default_fre1;
      *dflt_amp0 = default_amp0;
      *dflt_amp1 = default_amp1;
    
} 



char* getToken (string, start)
      char *string;                           /*  Input string             */
      int  *start;                            /*  starting from this point */
{
     char value[201];
     char *c, *d;

     c  = value;
     d  = string;
     d += *start;

     while((*d == ' ' || *d == ',') && *d ) d++;    /* skip ahead to entry */
     while( *d != ' ' && *d != ','  && *d ) *c++ = *d++;

     *c = '\0';
      c = value;
     *start = (int) d - (int) string;
     return(c);
}



     
int  whereIs(string, start, substring)
     char *string;                           /*  Input string             */
     char *substring;                        /*  look for this substring  */
     int   start;                            /*  starting from this point */
{
     int j, j0, j1, k, k1, test;
     
     j1 = strlen(string);
     k1 = strlen(substring);

     if (j1 < k1) return(-1);

     j=start;
     while (j<(j1-k1)) {
        test = 0;
        for (k=0; k<k1; k++) 
           test += abs(tolower(string[j+k]) - tolower(substring[k]));
        if (!test) 
           return(j + strlen(substring));
        j++;
     }

     return(-1);
}


