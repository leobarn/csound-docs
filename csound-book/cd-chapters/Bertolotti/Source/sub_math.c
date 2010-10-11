
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






#include <stdio.h>

#include <math.h>
#include <stdlib.h>
#define   PI 3.141592653589793




     /*------------------
      *    String Append
      *------------------*/

void  strapp(string, append)
   char *string;
   char *append;
{
   int lengthAppend=0, j=0;
   int lengthString=0, k=strlen(string);

   if ((lengthAppend=strlen(append))==0) return;
   while(j <= lengthAppend) string[k++] = append[j++];
   return;
}






      /*--------------------------------------------------------------
       *   Smooth data with a centered stencil of width = wide (msec)
       *--------------------------------------------------------------*/


void smoothData(N, time, value, wide, vs)
short int  N;       /* Number of data points in ``time'' and ``value'' */
short int  wide;    /* Smooth factor:  half-width of integration  domain */
short int *time;    /* Original time-values           */
short int *value;   /* Original function-values       */
short int *vs;      /* Array holding smoothed values  */
{
     int    i, j, low, high, N0;
     int    j_low=0, j_high=0;
     double dt, sum, norm, weight0, weight1, R;
short int*  local_time;
short int*  local_value;

     

     /*------------------
      * Exceptional cases
      *------------------*/

     if (!wide){
        for(i=0; i<N; i++) vs[i] = 0;
        return;
     }
     if (N==0) return;
     if (N==1) {
        vs[0] = value[0];
        return;
     }
     if (N==2) {
        vs[0] = value[0];
        vs[1] = value[1];
        return;
     }
     if (N==3) {
        vs[0] =  value[0];
        vs[1] = (value[2]+value[0])/2;
        vs[2] =  value[2];
        return;
     }
     

     /* -------------------------------------------------------
      *  Determine how many break points constitute wide (msec)        
      * -------------------------------------------------------*/
      while (j_low  < N &&                  time[j_low] < wide) j_low++;
      while (j_high < N && time[N-1] - time[N-1-j_high] < wide) j_high++;
      N0 = N + j_low + j_high;

/*
      printf("  j_low = %d,  j_high = %d   N0=%d \n", j_low, j_high, N0);
*/


     
     /* ---------------------------------------------------------------
      *  Extend data anti-symmmetric for amplitude, symmetric for freq.
      * ---------------------------------------------------------------*/

       local_time  = (short int*) malloc( N0 * sizeof(short int) );
       local_value = (short int*) malloc( N0 * sizeof(short int) );

       for (j=0; j<N; j++) {
         local_time[j+j_low]  = time[j];
         local_value[j+j_low] = value[j];
       }

       if (value[0]==0) {             /*  amplitude data  */
          for (j=0; j<j_low; j++) {
             local_time[j]    = -time[j_low-j];
             local_value[j]   = -value[j_low-j];
          }
          for (j=0; j<j_high; j++) {
             local_time[N+j_low+j]  = 2*time[N-1] - time[N-2-j];
             local_value[N+j_low+j] = -value[N-2-j];
          }
       } else {                       /*  frequency data  */
          for (j=0; j<j_low; j++) {
             local_time[j]    = -time[j_low-j];
             local_value[j]   =  value[j_low-j];
          }
          for (j=0; j<j_high; j++) {
             local_time[N+j_low+j]  = 2*time[N-1] - time[N-2-j];
             local_value[N+j_low+j] =  value[N-2-j];
          }
       }


/*
      for (j=0; j<j_low; j++) printf(" %3d  %6d - %6d    %6d - %6d  \n", 
          j, local_time[j], local_value[j],  0,  0);

      for (j=j_low; j<N+j_low; j++) printf(" %3d  %6d - %6d    %6d - %6d  \n", 
          j, local_time[j], local_value[j],
             time[j-j_low], value[j-j_low]);

      for (j=N+j_low; j<N0; j++) printf(" %3d  %6d - %6d    %6d - %6d  \n", 
          j, local_time[j], local_value[j],  0,  0);
*/



     /*----------------------------------
      *  Smooth data through integration
      *----------------------------------*/

     for(i=j_low; i<N+j_low; i++){

        low  = 0;
        high = 0;
        while (low  < i    && local_time[i] -  local_time[i-low] < wide) low++;
        while (high < N0-1-i && local_time[i+high]-local_time[i] < wide) high++;

        sum  = 0;
        norm = 0;
        R    = (local_time[i+high] - local_time[i-low])/2;

        for (j=i-low+1; j<=i+high; j++) {
           dt      = local_time[j] - local_time[i-low];
           weight1 = 1 - cos( 3.1415926535 * dt / R );
           dt      = local_time[j-1] - local_time[i-low];
           weight0 = 1 - cos( 3.1415926535 * dt / R );
           sum = sum + (local_value[j]*weight1 + 
                        local_value[j-1]*weight0) * 
                       (local_time[j] - local_time[j-1]);
           norm = norm + (weight1 + weight0) * 
                       (local_time[j] - local_time[j-1]);

/*
        printf(" window: (%3d)  %d  w1=%lf  w0=%lf   sum = %lf \n", 
           i,  j, weight1, weight0, sum); 
*/


        }
        vs[i - j_low] = (int) (sum/norm);

  
/*
        printf("l=%4d i=%4d h=%4d  vs=%d  dt=%d  %d %d \n", 
                low, i, high, vs[i], time[high] - time[low],
                                   time[low] , time[high]);
*/
  

     }


     return;
}








     /*
      *   Projection Intergal_{0}^{T}  f[x] sin[ mode x ]  dx
      */



float projectSin(N, time, value, mode)
      int   N   ;
short int  *time;
short int *value;
      int   mode;
{
  double t0, t1;  
  double s0, s1;  
  double fct;
  double tMin;
  double tMax;
  double sum=0;
  int    i;

  tMin = time[0];
  tMax = time[N-1];
  fct  = 2*PI/(tMax - tMin);

  t1 = -PI;
  s1 =   0;
  for (i=1; i<N; i++){
     t0 = t1;
     t1 = fct*(time[i] - tMin) - PI; 
     s0 = s1;
     s1 = sin(mode*t1);
     sum = sum + (s1*value[i] + s0*value[i-1])*(t1 - t0);
  }

  return( (float) (sum/(2*PI)) );

}






     /*
      *   Projection Intergal_{0}^{T}  f[x] cos[ mode x ]  dx
      */


float projectCos(N, time, value, mode)
      int   N   ;
short int  *time;
short int *value;
      int   mode;
{
  double t0, t1;  
  double c0, c1;  
  double fct;
  double tMin;
  double tMax;
  double sum=0;
  int    i;

  tMin = time[0];
  tMax = time[N-1];
  fct  = 2*PI/(tMax - tMin);

  t1 = -PI;
  c1 =   1;
  for (i=1; i<N; i++){
     t0 = t1;
     t1 = fct*(time[i] - tMin) - PI; 
     c0 = c1;
     c1 = cos(mode*t1);
     sum = sum + (c1*value[i] + c0*value[i-1])*(t1 - t0);
  }

  return( (float) (sum/(2*PI)) );

}






float myWindow (t, tMax, off0, off1)
short int t;
short int tMax;
short int off0;
short int off1;
{
      float   w;
      double  rise, fall, dt, s, ss;
      double  a0 =   (double) 315 / 128;
      double  a3 = - (double) 420 / 128;
      double  a5 =   (double) 378 / 128;
      double  a7 = - (double) 180 / 128;
      double  a9 =   (double)  35 / 128;


   /*----------------
    *  Attack offset
    *----------------*/
   if (t < off0) return((float) 0);


   /*---------------
    *  Decay offset
    *---------------*/
   if (t > (tMax - off1)) return((float) 0);


   /*----------------------------------------------------
    *   ``5th-derivative discontinuous'' window function
    *----------------------------------------------------*/
   rise =     (double) (tMax - off0 - off1) / (double) 3;
   fall = 2 * (double) (tMax - off0 - off1) / (double) 3;
   dt   = t - off0;
   if (dt < rise) {
     s = -1 + 2*dt/rise; 
     ss=  s*s;
     w =  1 + s*(a0 + ss*( a3 + ss*( a5 + ss*( a7 + a9*ss ))));
   } else if (dt > fall) {
     s = -1 + 2*(dt - fall)/rise; 
     ss=  s*s;
     w =  1 - s*(a0 + ss*( a3 + ss*( a5 + ss*( a7 + a9*ss ))));
   } else {
     w = 2;
   }
   return (w/2);

}



void  chebyCollPoints(tMin, tMax, nCol, collPoints)
short int tMin;
short int tMax;
short int nCol;
short int *collPoints;
{
   int     n;
   float   dt;
   double  s;

   for (n=0; n<nCol; n++){
      s = cos((double) n * PI / (nCol-1));
      collPoints[n] = tMin*(1 + s)/2 + tMax*(1 - s)/2;
   }
  
}




void  interpolate(np0, x0, f0,
                  np1, x1, f1)
short int np0;
short int *x0;
short int *f0;
short int np1;
short int *x1;
short int *f1;
{
   int     n0=0, n1;
   double  s;

   for (n1=0; n1<np1; n1++){

      if (x1[n1] > x0[np0-1]) {
        printf("  ** error ** (interpolate) x1[%d]=%d > x0[%d]=%d \n",
                  n1, x1[n1], np0-1, x0[np0-1]);
        return;
      }
      while(x0[n0+1] < x1[n1]) n0++;
      s = (x1[n1] - x0[n0])/((double) x0[n0+1] - (double) x0[n0]);
      f1[n1] = f0[n0] + s*(f0[n0+1] - f0[n0]);
   }
   return;
}



void posNegSplit(n0, time0, value0, n1, time1, value1, MaxPOINTS)
 short int *n0;
 short int *time0;
 short int *value0;
 short int *n1;
 short int *time1;
 short int *value1;
       int MaxPOINTS;
{
    int np=2, nn=2, j;
    int t0;


    if (*n0 > MaxPOINTS-4) return;
 

    /*
     *  Split               
     */
     for(j=0; j<*n0; j++){
       time1[j] = time0[j];
       if (value0[j] >= 0) value1[j] = 0;
       if (value0[j] <  0){
         value1[j] = value0[j];
         value0[j] = 0;
       }
     }
     time1[(*n0)+1] = 2*time1[(*n0)] - time1[(*n0)-1];
     time0[(*n0)+1] = 2*time0[(*n0)] - time0[(*n0)-1];
     value0[(*n0)++] = 0;
     value1[(*n0)  ] = 0;
     *n1 = *n0;



    /*
     *  Get zero crossings
     */
     for(j=1; j<*n0; j++){
        if (value0[j] > 0 && value1[j+1] < 0) {       /* Crossing + to -  */
          t0 =  time0[j] + value0[j]*( time0[j+1] - time0[j] ) / 
                     ((float) value0[j] - (float) value1[j+1]);
          time0[j+1] = t0;
          time1[j  ] = t0;
        }

        if (value1[j] < 0 && value0[j+1] > 0) {       /* Crossing - to +  */
          t0 =  time1[j] + value1[j]*( time1[j+1] - time1[j] ) / 
                     ((float) value1[j] - (float) value0[j+1]);
          time1[j+1] = t0;
          time0[j  ] = t0;
        }

     }


    /*
     *   Remove consecutive zeros
     */

     while ( np < (*n0) ){
       while(!(value0[np-2] == 0 && value0[np-1] == 0 && value0[np]==0) 
               && np < (*n0)) np++;
       if (np == (*n0)-1 ) { (*n0)--;  break;}
       for (j=np-1; j<(*n0)-2; j++) {
           time0[j]  = time0[j+1];
           value0[j] = value0[j+1];
       }
       (*n0)--;

     }


     while ( nn < (*n1) ){
       while(!(value1[nn-2] == 0 && value1[nn-1] == 0 && value1[nn]==0) 
               && nn < (*n1)) nn++;
       if (nn == (*n1)-1 ) { (*n1)--; break;}
       for (j=nn-1; j<(*n1)-2; j++) {
           time1[j]  = time1[j+1];
           value1[j] = value1[j+1];
       }
       (*n1)--;
     }


    /*
     *   Flip sign of the negative data
     */
       for (j=0; j<(*n1); j++) 
           value1[j] = (-1)*value1[j];

}



void  liftUp(n0, n1, f0, f1)
short int  n0, n1;
short int *f0;
short int *f1;
{
        double slope;
        int    numStart, numEnd;
        int    j0, j1;
  short int    value1, value0, i, iter, off;


/*
    FILE *fp;
    fp=fopen("junk.out","w");
*/



  /*
   *   Detect non-zero function support
   */
   numStart = n0;
   numEnd   = n1;
   while(f0[numStart]==0 && numStart < n1) numStart++;
   while(f0[numEnd  ]==0 && numEnd   > numStart) numEnd--;
   if(numStart>n0) numStart--;
   if(numEnd  <n1) numEnd++;
   



  /*
   *   Iterate four times
   */
   for (iter=0; iter<5; iter++){
     for (i=n0; i<n1; i++) f1[i] = 0;


    /*
     *   Look for local minima in negative range in selected region
     */

     j0     = numStart;
     value0 = 0;
     for(j1=numStart+1; j1<numEnd; j1++) {
        value1 = f0[j1];
        if (f0[j1-1] > value1 && value1 < 0 && f0[j1+1] > value1){
           slope = (value1 - value0)/((double) j1 - (double) j0);
           for (i=j0; i<j1; i++){
              f1[i] = value0 + slope * (i - j0);
           }
           value0 = value1;
           j0     = j1;
        }
     }
     if (j0>numStart && j0<numEnd) {
     slope = (0 - value0)/((double) (numEnd-1) - (double) j0);
       for (i=j0; i<numEnd; i++)
          f1[i] = value0 + slope * (i - j0);
     }



/*
   fprintf(fp, "  n0      =%d,  n1    =%d \n", n0, n1);
   fprintf(fp, "  numStart=%d,  numEnd=%d \n", numStart, numEnd);
   fprintf(fp,"i=%d   f0=%d   f1=%d  \n",numStart,f0[numStart],f1[numStart]);
   for (i=numStart+1; i<numEnd-1; i++){
      off = - (f1[i-1] + f1[i] + f1[i+1])/((float) 3) + 0.5;
      fprintf(fp,"i=%d      f0=%d    f1=%d    lift=%d \n",i,f0[i],f1[i],off);
   }
   fprintf(fp,"i=%d   f0=%d   f1=%d  \n",numEnd-1,f0[numEnd-1],f1[numEnd-1]);
   fprintf(fp, " --- end \n \n");
*/




     /*
      *   Shift upwards the original data.
      *   Low-pass filter shift data to remove slope discontinuity.
      */

     for (i=numStart+1; i<numEnd-1; i++){
        off = - (f1[i-1] + f1[i] + f1[i+1])/((float) 3) + 0.5;
        f0[i] += off;
     }


   }



   /*
    *   Remove remaining small negative values
    */

     for (i=0; i<numEnd; i++) if (f0[i]<0) f0[i] = 0;



}






struct Fourier {
      float amp;               /* Sin and Cos coefficients  */
      int   num;               /* Mode number               */
     };


int modeCompare(x0, x1)
    struct Fourier *x0;
    struct Fourier *x1;
{
    int  value=0;
    if(x0->amp < x1->amp) value =  1;
    if(x0->amp > x1->amp) value = -1;
    return(value);
}


