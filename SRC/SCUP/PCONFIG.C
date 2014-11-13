/* --------------------------------- */
/* Program PCONFIG.C                 */
/* Written DEC-88                    */
/* Update  25-12-88                  */
/* Update  16-5-89                   */
/* Function : read & write prt config*/
/* Update Mon  08-07-1989  01:42:26  */
/* --------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <dir.h>

#include "inc.h"
#include "global.ext"

FILE   *cfp;

int savepconfig(void)
{
   char s1[MAXPATH]; /* modified by Suttipong Kanakakorn */
   strcpy(s1,cuprintpath);
   if ((cfp = fopen(strcat(s1,"cuprint.cfg"),"w")) == NULL){fclose(cfp);return(-1);}
   if (fprintf ( cfp,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",
      graphicprint ,prtcodestd   ,cpi        ,lineperpage ,leftmargin ,
      rightmargin  ,locpagetitle ,locheading ,locfooting  ,printer    ,
      printer24pin ,maxcol       ,maxdot     ,maxbuffer   ,linespace  ,
      nlqmode      ,pagebreak    ,pagebegin  ,pageend     ,pagenumberoffset,
      copytoprint  ,stdcode      ,smallpaper)
      == EOF) {fclose(cfp);return(-2);}

   if (fprintf(cfp,"%s\n%s\n%s\n%s\n",pagetitle,pageformat,heading,footing)
      == EOF) {fclose(cfp);return(-2);}

   else {fclose(cfp);return(0);}

}

int readpconfig(void)
{
  int i0=0,i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0,i9=0;
  int j0=0,j1=0,j2=0,j3=0,j4=0,j5=0,j6=0,j7=0,j8=0,j9=0;
  int k0=0,k1=0,k2=0;
  char w1[200],w2[200],w3[200],w4[200];
  char s1[55];
  strcpy(s1,cuprintpath);
  if ((cfp = fopen(strcat(s1,"cuprint.cfg"),"r")) == NULL) {fclose(cfp);return(-1);}
  else
  if (fscanf  ( cfp,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",
                &i0,&i1,&i2,&i3,&i4,&i5,&i6,&i7,&i8,&i9,
                &j0,&j1,&j2,&j3,&j4,&j5,&j6,&j7,&j8,&j9,
                &k0,&k1,&k2) == EOF) {fclose(cfp);return(2);}
  else
  if (fgets(w1,200,cfp) == NULL) {fclose(cfp);return('3');}
  if (fgets(w2,200,cfp) == NULL) {fclose(cfp);return('4');}
  if (fgets(w3,200,cfp) == NULL) {fclose(cfp);return('5');}
  if (fgets(w4,200,cfp) == NULL) {fclose(cfp);return('6');}
/*
  printf("linespace=%d\nlineperpage=%d\npage read in=%s\n",j4,i3,w1);
  printf("heading=%s\nfooting=%s\n",w3,w4);
  getch();
*/
  fclose(cfp);
  /* ******************* */
  /* verify valid values */
  /* ******************* */
  graphicprint=    (i0==0 || i0==1)  ? i0 : graphicprint;
  prtcodestd=      (i1==0 || i1==1)  ? i1 : prtcodestd;
  cpi=             (i2>0 &&  i2<21)  ? i2 : 10;
 lineperpage  =    (i3>0 )           ? i3 : lineperpage;

 leftmargin   =    (i4<136&& i4>1 )  ? i4 : leftmargin;
 rightmargin  =    (i5>i4 && i5>1 )  ? i5 : rightmargin;

 locpagetitle =    (i6<6  && i6>=0)  ? i6 : locpagetitle;
 locheading   =    (i7<6  && i7>=0)  ? i7 : locheading;
 locfooting   =    (i8<6  && i8>=0)  ? i8 : locfooting;
 printer      =    (i9<7  && i9>=0)  ? i9 : FX;
 printer24pin =    (j0==0 || j0==1)  ? j0 : printer24pin;
 maxcol       =    (j1>0  && j1<137) ? j1 : 80;
 maxdot       =    (j2>0)            ? j2 : 1920;
 maxbuffer    =    (j3>0)            ? j3 : 1920;
 linespace    =    (j4==4 || j4==15) ? j4 :  15;
 nlqmode      =    (j5==0 || j5==1)  ? j5 : nlqmode;
 pagebreak    =    (j6>0)            ? j6 : pagebreak;
 pagebegin    =    (j7>0)            ? j7 : 1;
 pageend      =    (j8>=j7)          ? j8 : 9999;
 pagenumberoffset =(j9>0)            ? j9 : pagenumberoffset;
 copytoprint  =    (k0>0)            ? k0 : copytoprint;
 stdcode      =    (k1==0 || k1==1)  ? k1 : stdcode;
 smallpaper   =    (k2==0 || k2==1)  ? k2 : YES;
 if (strlen(w1)>0 && strlen(w1)<200){
    strcpy(pagetitle,w1);
    pagetitle[strlen(pagetitle)-1]='\0';
 }
 if (strlen(w2)>0 && strlen(w2)<200){
    strcpy(pageformat,w2);
    pageformat[strlen(pageformat)-1]='\0';
 }
 if (strlen(w3)>0 && strlen(w3)<200){
    strcpy(heading,w3);
    heading[strlen(heading)-1]='\0';
 }
 if (strlen(w4)>0 && strlen(w4)<200){
    strcpy(footing,w4);
    footing[strlen(footing)-1]='\0';
 }
 setprinter(smallpaper,printer24pin);
 return(0);
}
