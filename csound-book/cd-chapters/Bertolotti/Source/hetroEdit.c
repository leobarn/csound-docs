/* -----------------------------------------------------------------------
 
    Copyright (C) 1997  Fabio P. Bertolotti    (fabio.bertolotti@dlr.de)
 
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

extern int  readData();
extern void writeData();
extern int  readAsciiData();
extern void writeAsciiData();


main(argc, argv)
  int   argc;
  char *argv[];
{
      char   c[3];
      char   fileDefaultBin[18];
      char   fileDefaultAsc[18];
      char  *fileNameIn, *fileNameOut; 
      FILE  *fileIn, *fileOut;

short int    time_amp[MaxPARTIALS][MaxPOINTS];
short int    time_fre[MaxPARTIALS][MaxPOINTS];
short int   value_amp[MaxPARTIALS][MaxPOINTS];
short int   value_fre[MaxPARTIALS][MaxPOINTS];
short int   count_amp[MaxPARTIALS];
short int   count_fre[MaxPARTIALS];

      int   reverse=0;
      int   timeMax, ampMax, freqMin, freqMax;
      float dt, sum;

      int   tt[MaxPOINTS],  ff[MaxPOINTS];
      int   nc;
     
      int   i, j, par, partials;
      int   red = 1;


      strcpy(fileDefaultBin, "hetro_edit.het");
      strcpy(fileDefaultAsc, "hetro_edit.asc");


      if(!(--argc)) {
        printf(" No input file specified \n");
        exit(0);
      }


      if (*argv[1]!='-'){          /* if filenameIn is first in sequence */
        fileNameIn  = argv[1];
        argc--;
        if(argc){
           if (!strcmp(argv[2],"-r")) {
              reverse=1;
           } else {
              printf(" Unrecognized option  (not -r) \n");
              exit(0);
           }
        }
      }



      if (*argv[1]=='-'){          /* if filenameIn is second in sequence */
        if (!strcmp(argv[1],"-r")) {
           reverse=1;
        } else {
           printf(" Unrecognized option  (not -r) \n");
        }
        argc--;
        if(argc){
           fileNameIn  = argv[2];
        } else {
           printf(" Input filename not given \n");
           exit(0);
        }
      }



      if (!reverse) {
         fileNameOut = fileDefaultAsc;
         if ((fileIn = fopen(fileNameIn,"rb"))==NULL){
           printf(" Cannot open file:  %s \n",fileNameIn);
           exit(0);
         }
         if ((fileOut = fopen(fileNameOut,"w"))==NULL){
           printf(" Cannot open file:  %s \n",fileNameOut);
           exit(0);
         }
      } else {
         fileNameOut = fileDefaultBin;
         if ((fileIn = fopen(fileNameIn,"r"))==NULL){
           printf(" Cannot open file:  %s \n",fileNameIn);
           exit(0);
         }
         if ((fileOut = fopen(fileNameOut,"wb"))==NULL){
           printf(" Cannot open file:  %s \n",fileNameOut);
           exit(0);
         }
      }



      if(!reverse) {

        partials = readData(fileIn, time_amp,  time_fre, 
                                    value_amp, value_fre, 
                                    count_amp, count_fre,
                                    &timeMax , &ampMax,   
                                    &freqMin , &freqMax);


        writeAsciiData(fileOut, time_amp,  time_fre, 
                                value_amp, value_fre, 
                                count_amp, count_fre, partials);

      } else {

        partials = readAsciiData(fileIn, time_amp,  time_fre, 
                                         value_amp, value_fre, 
                                         count_amp, count_fre,
                                         &timeMax , &ampMax,   
                                         &freqMin , &freqMax);


        writeData(fileOut, time_amp,  time_fre, 
                           value_amp, value_fre, 
                           count_amp, count_fre, partials);
      } 


}


