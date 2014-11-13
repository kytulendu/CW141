/* Modified: Suttipong Kanakakorn Mon  08-14-1989  10:56:08 */
/* Tue  08-29-1989  13:22:53  add page length */

#include <stdio.h>
#include <dir.h>
#include "inc.h"

/*  ******************************* */
/*   3-level  buffer  declaration   */
/*  ******************************* */
char *font[2]; /* Suttipong Kanakakorn Tue  08-15-1989  01:23:14 */
char *fonts;   /* Suttipong Kanakakorn Tue  08-15-1989  01:23:19 */
char *bupper1;
char *bupper2;
char *bupper3;
char *bmiddle1;
char *bmiddle2;
char *bmiddle3;
char *bbelow1;
char *bbelow2;
char *bbelow3;
char *extbar1;
char *extbar2;
char *extbar3;

int  table[96] = {
/* ------------------------------------------------------------------------------
                    converse code from so-mo-oo to Kaset
   0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
   ------------------------------------------------------------------------------ */
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdb,0xdc,0xdd,
  0xde,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0x00,0xfc,0x00,0xfc,
  0x00,0x00,0x00,0x00,0xfd,0x00,0xfd,0x00,0x00,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,
  0x00,0x00,0x00,0xd2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x7f,0x7f,0x7f,0xfd,0xfd,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
} ;
char tablev[48] =  {
       2,1,2,1,1,1,1,1,  3,3,3,3,3,3,3,2,
       2,2,1,1,1,2,2,1,  1,1,1,1,1,1,1,2,
       2,2,2,2,2,2,2,2,  2,2,2,2,1,3,1,2
} ;
/*
int  gtable[16]  = {
       0x03,0x02,0x01,0x03,0x03,
       0x01,0x00,0x03,0x01,0x01,
       0x01,0x02,0x02,0x02,0x02,
       0x00
} ;
*/
/*
char mask241[8][3] ={
       0xc0,0x00,0x00,
       0x18,0x00,0x00,
       0x03,0x00,0x00,
       0x00,0x60,0x00,
       0x00,0x0c,0x00,
       0x00,0x01,0x80,
       0x00,0x00,0x30,
       0x00,0x00,0x06
} ;

char mask242[8][3] ={
       0x60,0x00,0x00,
       0x0c,0x00,0x00,
       0x01,0x80,0x00,
       0x00,0x30,0x00,
       0x00,0x06,0x00,
       0x00,0x00,0xc0,
       0x00,0x00,0x18,
       0x00,0x00,0x03
} ;
*/

FILE   *fp,*fopen() ;

int  fileready = NO;
char oldfilename[MAXPATH];
char filename[MAXPATH];
char cup_dir[MAXDRIVE+MAXDIR];
/**************************
   Menu data area .
***************************/
char mcol[3];
char smcol[3];
char mlenght=24;
/**************************
   Dotcommand global area .
 **************************/
   FILE   *mfp;                  /* define mailmerge file ptr  */
   char   *fieldname[20];    /* define array of fieldnames */
   char   *fieldcontent[20]; /* define array of strings    */
   int    fieldcount;            /* define number of fields    */
   int    mailmergeflag=NO;      /* each field content ready   */
   int    mergefileexist=NO;     /* .df successful             */
   int    fieldnameexist=NO;     /* .rv successful             */
/**************************/
/*** Printer Data Area ****/
int graphicprint=YES;
int prtcodestd = NO;              /* YES standard,NO ku */
int cpi=10;                      /* number of characters per inch */
int lineperpage = 33;
int userlineperpage = 33;
int pagelength =  11;            /* page length in inches */
int leftmargin = 1;
int rightmargin = 80;
char pagetitle[200] = "หน้าที่ ";
char pageformat[200]= "หน้าที่ %d";
char heading[200];
char footing[200];
/* --------------------------------------
    Location of pagetitle,heading,footing
   0 -> no print       1 -> left  justify
   2 -> center justify 3 -> right justify
   4 -> right<-> odd pgno  left  <-> even
   5 -> right<-> even      left  <->  odd
   -------------------------------------- */
int locpagetitle = 0;
int locheading   = 0;
int locfooting   = 0;
int printer      = LX;
int printer24pin = NO;
int printer24pininit = NO;
int printer9pininit = NO;
int maxcol       = 80;
int maxdot       = 1920;
int maxbuffer    = 1920;
int linespace    = 24;
int nlqmode = YES;
int pagebreak = NO;
int pagebegin = 1;
int pageend   = 999;
int pagenumberoffset = 1;
int copytoprint = 1;
int stdcode = YES;
int curline = 0;
int curpage = 0;
int newpage = NO;
/*********************************/
int thaimode = NO;
int quitprog = NO;
int rowmax[3] = {7,7,7};
int prtrowmax = 2;
int titlerowmax= 2;
int moderowmax= 1;
int smallpaper=YES;

/* add by Suttipong Kanakakorn */
char *progname;

/* By CUCC */
int extbarprinting;
