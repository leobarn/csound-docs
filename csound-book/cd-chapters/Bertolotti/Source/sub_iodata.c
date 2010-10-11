
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



 
int readData(fp, time_amp , time_fre , 
                 value_amp, value_fre, 
                 count_amp, count_fre,
                 timeMax  , ampMax,   
                 freqMin  , freqMax)
      FILE   *fp;
short int     time_amp[MaxPARTIALS][MaxPOINTS];
short int     time_fre[MaxPARTIALS][MaxPOINTS];
short int    value_amp[MaxPARTIALS][MaxPOINTS];
short int    value_fre[MaxPARTIALS][MaxPOINTS];
short int    count_amp[MaxPARTIALS];
short int    count_fre[MaxPARTIALS];
      int   *timeMax, *freqMin, *freqMax, *ampMax;
{

      int   word=0, j;
      int   partial_amp=-1;
      int   partial_fre=-1;

      int   time, value; 
      int   amplitude=0;
      int   frequency=0;
      
short int   num, END=32767;



      /*
       *   Initialize  
       */
      *timeMax = 0;
      *ampMax  = -10000000;
      *freqMax = -10000000;
      *freqMin =  10000000;



      /*
       *   Read to end of file
       */


      printf(" Partials found: ");
      while (!feof(fp)){   

          word++;
          if(fread(&num, 2, 1, fp)==0)
             break;

          if (num==-1) {
             frequency = 0;
             amplitude = 1;
             if((partial_amp++)==MaxPARTIALS){
                printf("Out of memory: MaxPARTIALs = %d \n", MaxPARTIALS);
                exit(0);
             }
             count_amp[partial_amp] = 0;
             printf(" %d ", partial_amp);

          } else if (num==-2) {
             frequency = 1;
             amplitude = 0;
             if((partial_fre++)==MaxPARTIALS){
                printf("Out of memory: MaxPARTIALs = %d \n", MaxPARTIALS);
                exit(0);
             }
             count_fre[partial_fre] = 0;

          } else if (num!=END){
             time = num;
             word++;
             if(fread(&num, 2, 1, fp)==0){
               printf(" error reading file: word number = %d \n",word);
               exit(0);
             }
             value = num;
             if(amplitude) {
                j = count_amp[partial_amp]++;
                if(j==MaxPOINTS){
                  printf("Out of memory: MaxPOINTS = %d \n", MaxPOINTS);
                  exit(0);
                }
                time_amp[partial_amp][j] = time;
                value_amp[partial_amp][j] = value;
                if (*timeMax < (float) abs(time))  *timeMax= abs(time);
                if (*ampMax  < (float) abs(value)) *ampMax = abs(value);
             } 
             if(frequency) {
                j = count_fre[partial_fre]++;
                if(j==MaxPOINTS){
                  printf("Out of memory: MaxPOINTS = %d \n", MaxPOINTS);
                  exit(0);
                }
                time_fre[partial_fre][j] = time;
                value_fre[partial_fre][j] = value;
                j = partial_fre + 1;
                if (*timeMax < (float) abs(time) )   *timeMax= abs(time);
                if (*freqMax < (float) abs(value/j)) *freqMax= abs(value/j);
                if (*freqMin > (float) abs(value/j)) *freqMin= abs(value/j);
             }
          }
      }  
      printf(" \n");



     /*-------------------------------------------------------------------
      *  Check consistency
      *-------------------------------------------------------------------*/

      if (partial_amp != partial_fre){
         printf(" ** ERROR ** (ioData)  ");
         printf("partial-amplitude (=%d)  !=  partial_frequency (=%d) \n",
                 partial_amp, partial_fre);
         exit(1);
      } else {
         partial_amp++;
         partial_fre++;
      }


     /*-------------------------------------------------------------------
      *  Eliminate time duplications that erroneously occur in hetro files
      *-------------------------------------------------------------------*/

      for(j=0; j<partial_fre; j++){
         if (time_fre[j][count_fre[j]-2]==
             time_fre[j][count_fre[j]-1]) count_fre[j]--;
         if (time_amp[j][count_amp[j]-2]==
             time_amp[j][count_amp[j]-1]) count_amp[j]--;
      }


      return(partial_fre);
}




void writeData(fp, time_amp , time_fre, 
                   value_amp, value_fre, 
                   count_amp, count_fre, partials)
      FILE   *fp;
short int     time_amp[MaxPARTIALS][MaxPOINTS];
short int     time_fre[MaxPARTIALS][MaxPOINTS];
short int    value_amp[MaxPARTIALS][MaxPOINTS];
short int    value_fre[MaxPARTIALS][MaxPOINTS];
short int    count_amp[MaxPARTIALS];
short int    count_fre[MaxPARTIALS];
      int    partials;
{

short int   num, time, value, END=32767;
      int   n, j;
   
      for (n=0; n<partials; n++){
          num = -1;
          if(fwrite(&num, 2, 1, fp)==0){
               printf(" error writing to file:  partial %d \n",n);
               exit(0);
          }
          for (j=0; j<count_amp[n]; j++){
               time  = time_amp[n][j];
               value = value_amp[n][j];
               fwrite(&time, 2, 1, fp);
               fwrite(&value, 2, 1, fp);
          }
          fwrite(&END, 2, 1, fp);
          num = -2;
          if(fwrite(&num, 2, 1, fp)==0){
               printf(" error writing to file:  partial %d \n",n);
               exit(0);
          }
          for (j=0; j<count_fre[n]; j++){
               time  = time_fre[n][j];
               value = value_fre[n][j];
               fwrite(&time, 2, 1, fp);
               fwrite(&value, 2, 1, fp);
          }
          fwrite(&END, 2, 1, fp);
      }
}









int readAsciiData(fp, time_amp,  time_fre, 
                      value_amp, value_fre, 
                      count_amp, count_fre,
                      timeMax  , ampMax,   
                      freqMin  , freqMax)
      FILE   *fp;
short int     time_amp[MaxPARTIALS][MaxPOINTS];
short int     time_fre[MaxPARTIALS][MaxPOINTS];
short int    value_amp[MaxPARTIALS][MaxPOINTS];
short int    value_fre[MaxPARTIALS][MaxPOINTS];
short int    count_amp[MaxPARTIALS];
short int    count_fre[MaxPARTIALS];
      int   *timeMax, *freqMin, *freqMax, *ampMax;
{

      int   word=0, j;
      int   partial_amp=-1;
      int   partial_fre=-1;

      int   time, value; 
      int   amplitude=0;
      int   frequency=0;
      
      int   num;
short int   END=32767;



      /*
       *   Initialize  
       */
      *timeMax = 0;
      *ampMax  = -10000000;
      *freqMax = -10000000;
      *freqMin =  10000000;



      /*
       *   Read to end of file
       */


      printf(" Partials found: ");
      while (!feof(fp)){   

          word++;
          fscanf(fp, "%d", &num);

          if (num==-1) {
             frequency = 0;
             amplitude = 1;
             if((partial_amp++)==MaxPARTIALS){
                printf("Out of memory: MaxPARTIALs = %d \n", MaxPARTIALS);
                exit(0);
             }
             count_amp[partial_amp] = 0;
             printf(" %d ", partial_amp);

          } else if (num==-2) {
             frequency = 1;
             amplitude = 0;
             if((partial_fre++)==MaxPARTIALS){
                printf("Out of memory: MaxPARTIALs = %d \n", MaxPARTIALS);
                exit(0);
             }
             count_fre[partial_fre] = 0;

          } else if (num!=END){
             time = num;
             word++;
             fscanf(fp,"%d",&num);
             value = num;
             if(amplitude) {
                j = count_amp[partial_amp]++;
                if(j==MaxPOINTS){
                  printf("Out of memory: MaxPOINTS = %d \n", MaxPOINTS);
                  exit(0);
                }
                time_amp[partial_amp][j] = time;
                value_amp[partial_amp][j] = value;
                if (*timeMax < (float) abs(time))  *timeMax= abs(time);
                if (*ampMax  < (float) abs(value)) *ampMax = abs(value);
             } 
             if(frequency) {
                j = count_fre[partial_fre]++;
                if(j==MaxPOINTS){
                  printf("Out of memory: MaxPOINTS = %d \n", MaxPOINTS);
                  exit(0);
                }
                time_fre[partial_fre][j] = time;
                value_fre[partial_fre][j] = value;
                j = partial_fre + 1;
                if (*timeMax < (float) abs(time) )   *timeMax= abs(time);
                if (*freqMax < (float) abs(value/j)) *freqMax= abs(value/j);
                if (*freqMin > (float) abs(value/j)) *freqMin= abs(value/j);
             }
          }
      }  
      printf(" \n");



     /*-------------------------------------------------------------------
      *  Check consistency
      *-------------------------------------------------------------------*/

      if (partial_amp != partial_fre){
         printf(" ** ERROR ** (ioData)  ");
         printf("partial-amplitude (=%d)  !=  partial_frequency (=%d) \n",
                 partial_amp, partial_fre);
         exit(1);
      } else {
         partial_amp++;
         partial_fre++;
      }


      return(partial_fre);
}






void writeAsciiData(fp, time_amp,  time_fre, 
                        value_amp, value_fre, 
                        count_amp, count_fre, partials)
      FILE   *fp;
short int     time_amp[MaxPARTIALS][MaxPOINTS];
short int     time_fre[MaxPARTIALS][MaxPOINTS];
short int    value_amp[MaxPARTIALS][MaxPOINTS];
short int    value_fre[MaxPARTIALS][MaxPOINTS];
short int    count_amp[MaxPARTIALS];
short int    count_fre[MaxPARTIALS];
      int    partials;
{

short int   num, time, value, END=32767;
      int   n, j;
   
      for (n=0; n<partials; n++){
          num = -1;
          fprintf(fp, "%d \n", num);
          for (j=0; j<count_amp[n]; j++){
               time  = time_amp[n][j];
               value = value_amp[n][j];
               fprintf(fp, " %d  %d  \n", time, value);
          }
          fprintf(fp, "%d \n", END);

          num = -2;
          fprintf(fp, "%d \n", num);
          for (j=0; j<count_fre[n]; j++){
               time  = time_fre[n][j];
               value = value_fre[n][j];
               fprintf(fp, " %d  %d  \n", time, value);
          }
          fprintf(fp, "%d \n", END);
      }
}

