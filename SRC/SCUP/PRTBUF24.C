/* ------------------------------------------------------------------------
   Module         :  PRTBUF24.C
   Description    :  print data in cp_lqbuf.
   Written by     :  Mr. Prasart Topitumatukhun ID. 2716943
   Date written   :  10 May 1989
   ------------------------------------------------------------------------ */
#include <string.h>
#include <stdio.h>
#include "inc.h"
#include "24pins.h"
#include "global.ext"

void ClearTempBuffer(int memclear)
{
 memclear = (memclear>maxdot*2) ? maxdot*2:memclear;
 memset(bupper1,0,memclear);
 memset(bupper2,0,memclear);
 memset(bupper3,0,memclear);
 memset(bmiddle1,0,memclear);
 memset(bmiddle2,0,memclear);
 memset(bmiddle3,0,memclear);
 memset(bbelow1,0,memclear);
 memset(bbelow2,0,memclear);
 memset(bbelow3,0,memclear);
 if(extbarprinting) {
   memset(extbar1,0,memclear);
   memset(extbar2,0,memclear);
   memset(extbar3,0,memclear);
 }
}

/* ------------------------------------------------------------------------
   Function       :  TransformBuffer()
   Description    :  Use to transfrom data structure cp_lqbuf to nine
                     one dimension array.
   Input parameter:  cindex = number of character to transform.
   Return value   :  none.
   ------------------------------------------------------------------------ */
/* ----------------------------------------------
void TransformBuffer(int cindex)
{
 int i,j,k,memclear;
 ClearTempBuffer(cindex*18);
 for (i=0,k=0;i<cindex;i++)
     for (j=0;j<18;j++,k++) {
         bupper1[k]=(cp_lqbuf.upper+i)->font[j][0];
         bupper2[k]=(cp_lqbuf.upper+i)->font[j][1];
         bupper3[k]=(cp_lqbuf.upper+i)->font[j][2];
         bmiddle1[k]=(cp_lqbuf.middle+i)->font[j][0];
         bmiddle2[k]=(cp_lqbuf.middle+i)->font[j][1];
         bmiddle3[k]=(cp_lqbuf.middle+i)->font[j][2];
         bbelow1[k]=(cp_lqbuf.lower+i)->font[j][0];
         bbelow2[k]=(cp_lqbuf.lower+i)->font[j][1];
         bbelow3[k]=(cp_lqbuf.lower+i)->font[j][2];
     }
}
----------------------------- */
void TransformBuffer(int cindex)
{
 register int i,j,k;
 int  memclear;
 FONT *p10,*p20,*p30,*p40;
 char *p11,*p21,*p31,*p41;
 for (i=0,k=0;i<cindex;i++) {
     p10=(cp_lqbuf.upper+i);
     p20=(cp_lqbuf.middle+i);
     p30=(cp_lqbuf.lower+i);
     if(extbarprinting)
       p40=(cp_lqbuf.extbar+i);
     for (j=0;j<18;j++,k++) {
	 p11=&(p10->font[j][0]);
	 p21=&(p20->font[j][0]);
	 p31=&(p30->font[j][0]);
	 bupper1[k]=*(p11);
	 bupper2[k]=*(p11+1);
	 bupper3[k]=*(p11+2);
	 bmiddle1[k]=*(p21);
	 bmiddle2[k]=*(p21+1);
	 bmiddle3[k]=*(p21+2);
	 bbelow1[k]=*(p31);
	 bbelow2[k]=*(p31+1);
	 bbelow3[k]=*(p31+2);
         if(extbarprinting) {
           p41=&(p40->font[j][0]);
           extbar1[k]=*(p41);
           extbar2[k]=*(p41+1);
           extbar3[k]=*(p41+2);
         }
     }
 }
}

int Ltrim24(char buf1[], char buf2[], char buf3[], int left, int right)
{
     register int i    ;
     i = left ;
     while ( (buf1[i] == 0) && (buf2[i]==0) && (buf3[i]==0) ) {
          i++ ;
          if (i > right)
             return(right);
     }
     return((int)(i / 18));
}


int Rtrim24(char buf1[], char buf2[], char buf3[], int left, int right)
{
     register int i      ;
     i = right-1;
     while ( (buf1[i] == 0) && (buf2[i]==0) && (buf3[i]==0) ) {
          i-- ;
          if(i < left)
          return(left) ;
     }
     return(i+1);
}

void PrintBuffer24pin(char buf1[], char buf2[], char buf3[], int maxscan)
{
  int left,right,length;
  register int i,j;
  left = Ltrim24(buf1,buf2,buf3,0,maxscan);
  if (left != maxscan) {
     right = Rtrim24(buf1,buf2,buf3,0,maxscan);
     for (i=0;i<left;i++)
         putp(' ');                 /* print in text mode */
     length=right-(left*18);
     if (length>0) {
        PrinterSetGraphicMode24pinTripleDensity(length);
        for (i=0,j=left*18;i<length;i++,j++) {
            putp(buf1[j]);
            putp(buf2[j]);
            putp(buf3[j]);
        }
     }
  }
}

void PrintThree24pin(int uindex, int mindex, int lindex)
{
    long maxprint;        /* max byte to print */
    int spleft;

    maxprint = (cpi>=10) ? ((long)(mindex)*18*1000)/(cpi*100):maxdot;
    maxprint = (maxprint>maxdot) ? maxdot:maxprint;
    ClearTempBuffer(mindex*18);
    TransformBuffer(mindex);

    charwidth(bupper1);
    charwidth(bupper2);
    charwidth(bupper3);
    PrintBuffer24pin(bupper1,bupper2,bupper3,(int)(maxprint));

    PrinterLineFeed180inch(23);
    charwidth(bmiddle1);
    charwidth(bmiddle2);
    charwidth(bmiddle3);
    PrintBuffer24pin(bmiddle1,bmiddle2,bmiddle3,(int)(maxprint));

    PrinterLineFeed180inch(23);
    charwidth(bbelow1);
    charwidth(bbelow2);
    charwidth(bbelow3);
    PrintBuffer24pin(bbelow1,bbelow2,bbelow3,(int)(maxprint));

    if(linespace>23) {
      spleft=linespace;
      charwidth(extbar1);
      charwidth(extbar2);
      charwidth(extbar3);
      while(spleft>23) {
        PrinterLineFeed180inch(23);
        PrintBuffer24pin(extbar1,extbar2,extbar3,(int)(maxprint));
        spleft-=23;
      }
      PrinterLineFeed180inch(spleft);
    } else
      PrinterLineFeed180inch(linespace);
}
