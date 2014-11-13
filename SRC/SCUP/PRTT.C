/* ---------------------------------------------------- */
/* Program     : PRTT.C                                 */
/* Writen      : Wittaya Wonganawat                     */
/* Called from : PMENU.C                                */
/* Function    : Operate character's occurence on paper */
/*               in textmode.                           */
/* Create date : nov.31                                 */
/* Last update : 11/12/31                               */
/* Last update : 10/4/32                                */
/* Updated by  : Suttipong Kanakakorn                   */
/*               Tue  08-08-1989  23:24:19              */
/*               Add all function prototype             */
/*               Tue  08-29-1989  11:57:57              */
/*             make it able to print 33 lines in 1 page */
/*               Sat  10-28-1989  10:39:26              */
/*             compress the table, and function         */
/* ---------------------------------------------------- */

/*  constant   area   */
#define  ASCII_NO   256
#define  BUFFERSIZE   500
 /* ------------------- */

/*   heading           */
#include <mem.h>
#include <dos.h>
#include "inc.h"
#include "cpthai.h"

/* function prototype */
void loadline(unsigned char line[]);
void tclearbuffer(void);
void endstring(int *current);
int tadj_attr(unsigned int *attr, int *ch);
void putattr(unsigned int *attr, int *current);
void printattr(unsigned char attr);
void tpretobuffer(unsigned int *attr, int *ch, int *current);
void tblockgraphic(int *ch, int *current);
void normal(int *ch, int *current);
void superscript(int *ch, int *current);
void subscript(int *ch, int *current);
int tislevel(int *line);
void tprint_three(void);
void putpstring(unsigned char s[]);

/* -------------------
   global area.
   ------------------------- */
extern int  cpi;             /* from pmenu.c */
int  underlineactive = 0;    /* 0 -> normal,1 -> subscript or superscript */
char tnortop[500];
char tnorupp[500];
char tnormid[500];
char tnorbel[500];
char tsuptop[500];
char tsupupp[500];
char tsupmid[500];
char tsupbel[500];
char tsubtop[500];
char tsubupp[500];
char tsubmid[500];
char tsubbel[500];
char textbar[500];
/* Suttipong Kanakakorn Sat  10-28-1989  10:39:12
char ttable[96] = {
/*  converse code from so-mo-oo to Kaset
 0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f   */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdb,0xdc,0xdd,
0xde,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0x00,0xfc,0x00,0xfc,
0x00,0x00,0x00,0x00,0xfd,0x00,0xfd,0x00,0x00,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,
0x00,0x00,0x00,0xd2,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x7f,0x7f,0x7f,0xfd,0xfd,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
*/
/* -------------------------------------
   so mo or level table.
   0 -> top    levle.
   1 -> upper
   2 -> middle or normal
   3 -> below
   4 -> noprint or invalid
   ------------------------------------- */
/* We don't need it, Suttipong Kanakakorn Sat  10-28-1989  10:56:48
char ttablev[48] =  {
       2,1,2,2,1,1,1,1,  3,3,3,4,4,4,4,2,
       2,2,2,2,2,2,2,1,  0,0,0,0,0,1,2,2,
       2,2,2,2,2,2,2,2,  2,2,2,2,4,4,4,4,
} ;
*/
/* ------------------------------------
   graphic table block lookup table.
   provide for grph code 0x8f to 0x9e .
   to extend '|' in the upper and lower.
   00 -> non extending             I <-- upper
   01 -> lowet  -ii-           --- + <-- character itself.
   02 -> upper  -ii-               I <-- lower
   03 ->  both lower and upper.
   ----------------------------------- */
unsigned int  tgtable[16]  = {    /* offset == 0x8f */
    0x03,
    0x02,0x01,0x03,0x03,0x01,
    0x00,0x03,0x01,0x01,0x01,
    0x02,0x02,0x02,0x02,0x00
} ;/* ---------------------------------------------------------------- */

/* add by Suttipong Kanakakorn Tue  09-05-1989  01:03:36 */
unsigned char prt_linedraw_tab[] = { /* offset == 0x8f */
    0x8a, 0x88, 0x89, 0x87, 0x86, 0x94, 0x85,
    0x84, 0x97, 0x80, 0x81, 0x82, 0x83, 0x9c,
    0x9d, 0x9e
};

/* -------------------------------------------- */
/* Print string 'line' to printer               */
/* this module is called from external routine. */
/* -------------------------------------------- */
void PrinterLoadLineText(unsigned char line[])
{
    tclearbuffer();
    loadline(line);
    tprint_three();
}

void loadline(unsigned char line[])
{
    auto unsigned int  attr=0           ;
    auto int  current=0                 ;   /* current dot position */
    int  ch                             ;
        current   = current ^ current   ;
        attr      = attr ^ attr         ;
        while (*line != '\0' ) {
               ch = *line;
               if (ch < 32) {          /* codes under 32 are control or attribute */
                  attr = tadj_attr(&attr,&ch) ;
                  putattr(&attr,&current);
               }
               else {
                  tpretobuffer(&attr,&ch,&current);
               }
               line++   ;
         }
         endstring(&current);
}

void tclearbuffer(void)
{
    int i;
    for (i = 0;i<500;) {
        tnortop[i] = ' ';
        tnorupp[i] = ' ';
        tnormid[i] = ' ';
        tnorbel[i] = ' ';
        tsuptop[i] = ' ';
        tsupupp[i] = ' ';
        tsupmid[i] = ' ';
        tsupbel[i] = ' ';
        tsubtop[i] = ' ';
        tsubupp[i] = ' ';
        tsubmid[i] = ' ';
        tsubbel[i] = ' ';
        textbar[i] = ' ';
        i++;
    }
}

void endstring(int *current)
{
    int i;
    i  = (*current)++;
        tnortop[i] = '\0';
        tnorupp[i] = '\0';
        tnormid[i] = '\0';
        tnorbel[i] = '\0';
        tsuptop[i] = '\0';
        tsupupp[i] = '\0';
        tsupmid[i] = '\0';
        tsupbel[i] = '\0';
        tsubtop[i] = '\0';
        tsubupp[i] = '\0';
        tsubmid[i] = '\0';
        tsubbel[i] = '\0';
        textbar[i] = '\0';
/*      (*current)++;*/
}
/* ------------------------------------   */
/* This module operate on all attribute   */
/* Some attributes are tokkle to itself   */
/* Some, effect to the others             */
/* Example ,superscript attr, sets itself */
/* and also resets subscript attribute .  */
/* ------------------------------------   */
int tadj_attr(unsigned int *attr, int *ch)
{
   int  s,ns,na,nb,xx,yy ;
     switch (*ch){
          case ITALIC :
                   return(*attr ^ 0x20);
          case UNDERLINE :
                   s  = 1        ;
                   ns = s ^ 0x01 ;
                   nb = (~(*attr) & 0x40) >> 6 ;
                   na = (~(*attr) & 0x10) >> 4 ;
                   yy = (s & nb) <<6 ;
                   xx = (ns & na ) << 4 ;
                   return((*attr & 0xaf ) | xx | yy) ;
          case DOUBLEUNDERLINE :
                   s  = 0        ;
                   ns = s ^ 0x01 ;
                   nb = (~(*attr) & 0x40) >> 6 ;
                   na = (~(*attr) & 0x10) >> 4 ;
                   yy = (s & nb) <<6 ;
                   xx = (ns & na ) << 4 ;
                   return((*attr & 0xaf ) | xx | yy) ;
          case SUBSCRIPT :
                   s  = 0;
                   ns = s ^ 0x01 ;
                   nb = (~(*attr) & 0x04) >> 2 ;
                   na = (~(*attr) & 0x08) >> 3 ;
                   yy = (s & nb) <<2 ;
                   xx = (ns & na ) << 3 ;
                   return((*attr & 0xf3 ) | xx | yy) ;
          case SUPER :
                   s  = 1        ;
                   ns = s ^ 0x01 ;
                   nb = (~(*attr) & 0x04) >> 2 ;
                   na = (~(*attr) & 0x08) >> 3 ;
                   yy = (s & nb) <<2 ;
                   xx = (ns & na ) << 3 ;
                   return((*attr & 0xf3 ) | xx | yy) ;
          case ENLARGE :
                   return(*attr ^ 0x02);
          case BOLD :
                   return(*attr ^ 0x01);
          default  :
                   return(*attr);
     }
}
/* --------------------------------------  */
/* Put ESC code according to attribute     */
/* in 12 text buffers.                     */
/* --------------------------------------  */
void putattr(unsigned int *attr, int *current)
{
    int i;
    unsigned char a;
    i= *current;
    a= *attr|0x80;
/*  mask on unused bit(7) to be able to insert in string */
/*  because of normal attribute apply to 00 or NULL      */

/*  We cannot print Underline for subscript attribute */
/*  So mask Underline and Double off in Normal level  */
        if((*attr & 0x08) != 0){                      /* Yes, Subscript     */
          tnortop[i] = ESC;  tnortop[i+1] = (a&0xaf); /* mask off under&dou */
          tnorupp[i] = ESC;  tnorupp[i+1] = (a&0xaf);
          tnormid[i] = ESC;  tnormid[i+1] = (a&0xaf);
          tnorbel[i] = ESC;  tnorbel[i+1] = (a&0xaf);
          tsubtop[i] = ESC;  tsubtop[i+1] = a;
          tsubupp[i] = ESC;  tsubupp[i+1] = a;
          tsubmid[i] = ESC;  tsubmid[i+1] = a;
          tsubbel[i] = ESC;  tsubbel[i+1] = a;
        }else{
          tnortop[i] = ESC;  tnortop[i+1] = a;
          tnorupp[i] = ESC;  tnorupp[i+1] = a;
          tnormid[i] = ESC;  tnormid[i+1] = a;
          tnorbel[i] = ESC;  tnorbel[i+1] = a;
          tsubtop[i] = ESC;  tsubtop[i+1] = (a&0xaf);
          tsubupp[i] = ESC;  tsubupp[i+1] = (a&0xaf);
          tsubmid[i] = ESC;  tsubmid[i+1] = (a&0xaf);
          tsubbel[i] = ESC;  tsubbel[i+1] = (a&0xaf);
        }
        tsuptop[i] = ESC;  tsuptop[i+1] = a;
        tsupupp[i] = ESC;  tsupupp[i+1] = a;
        tsupmid[i] = ESC;  tsupmid[i+1] = a;
        tsupbel[i] = ESC;  tsupbel[i+1] = a;
        textbar[i] = ESC;  textbar[i+1] = a;
        (*current)++;(*current)++;
}
/* --------------------------------------  */
/* Print ESC code according to attribute   */
/* --------------------------------------  */
void printattr(unsigned char attr)
{
     unsigned char a;
     unsigned char pattr,spattr;
     unsigned char doubleunderline,underline,italic,enlarge,bold;
     a = attr & 0x7f; /* mask of auxiliary bit. */
     underline = (a & 0x40) >> 6;
     doubleunderline = (a & 0x10) >> 4; /* both under and double under */
     italic    = (a & 0x20) >> 5;
     enlarge   = (a & 0x02) >> 1;
     bold      = (a & 0x01);
     pattr     = 0x00 | (underline << 7)| (doubleunderline << 7)
                      |(italic << 6)|(enlarge << 5)|(bold << 3);
     if(cpi>17)  pattr |= 0x05;   /* 20 cpi <=> condense-elite only in draft */
     else
     if(cpi>12)  pattr |= 0x04;   /* 17 cpi <=> condense only in draft */
     else
     if(cpi==12) pattr |= 0x01;   /* 12 cpi <=> elite    */
     spattr    = pattr & 0x7f; /* mask off under line bit in sub-superscript */

     putp(ESC); putp('!');
     if(underlineactive == 1)putp(pattr);
     else putp(spattr);   /* no underline */
}
/* --------------------------------------  */
/* Checking type of character appearance   */
/* e.g. block,boldface enlarge,normal ect. */
/* --------------------------------------  */
void tpretobuffer(unsigned int *attr, int *ch, int *current)
{
  int cont ;
  if(((unsigned)*ch > 0x8e) && ((unsigned) *ch < 0x9e)){
     tblockgraphic(ch,current)  ;
  }
  else
    {
     cont  = *attr & 0x0c ;
        switch (cont) {
             case 0x00 :
                       normal(ch,current) ;
                       break;
             case 0x04 :
                       superscript(ch,current)  ;
                       break;
             case 0x08 :
                       subscript(ch,current) ;
                       break;
              default  :
                       normal(ch,current) ;
                       break;
       }
  }
}
/* --------------------------------------------- */
/* Invention of graphic blocks to build tables   */
/* this include both single and double lineblock */
/* such as ˜ ‘ ™ “ – ’ š  › •                  */
/* modify by Suttipong Kanakakorn Tue  09-05-1989  01:00:11 */
/* allow grob to print on printer */
/* --------------------------------------------- */
/* old code
void tblockgraphic(int *ch, int *current)
{
       unsigned char g,u,m,b;
       int pos, a;
       a = *ch;
       g = tgtable[a - 0x8f] ;
       u = ((g&0x02)==0) ? 0x20:0x96;  /* write ' ' or '|' */

       m = a;
       b = ((g&0x01)==0) ? 0x20:0x96;
       pos = *current ;
       tnorupp[pos]    = u;
       tnormid[pos]    = m;
       tnorbel[pos]    = b;
       (*current)++;
}
*/
/* new code */
void tblockgraphic(int *ch, int *current)
{
       register int pos;
       register unsigned g;

       pos = *current ;
       g = tgtable[*ch - 0x8f] ;
       tnorupp[pos]    = ((g & 0x02 ) == 0) ? 0x20:0x96;  /* write ' ' or '|' */
       /* tnormid[pos]    = prt_linedraw_tab[*ch - 0x8f]; */
       tnormid[pos]    = *ch;
       tnorbel[pos]    = ((g & 0x01)==0) ? 0x20:0x96;
       textbar[pos]    = ((g & 0x01)==0) ? 0x20:0x96;
       (*current)++;
}

/* Modify: Suttipong Kanakakorn Sat  10-28-1989  10:55:32 */
void normal(int *ch, int *current)
{
       int level;
       int pos;
       unsigned char a = *ch ;
       level = tislevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
            case CP_MIDDLE :
                   pos = *current;
                   tnormid[pos] = a;
                   (*current)++;
                   break;
            case CP_UPPEST :
                   pos = --(*current);
                   if(tnorupp[pos]==' ')
                      tnorupp[pos]=a;
                   else
                      tnortop[pos]=a;
                   (*current)++;
                   break;
            case CP_UPPER :
                   pos = --(*current);
                   tnorupp[pos] = a;
                   (*current)++;
                   break;
            case CP_LOWER :
                   pos = --(*current);
                   tnorbel[pos] = a;
                   (*current)++;
                   break;
       }
}

/* Modify: Suttipong Kanakakorn Sat  10-28-1989  10:55:48 */
void superscript(int *ch, int *current)
{
       int level;
       int pos;
       unsigned char a = *ch ;
       level = tislevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
            case CP_MIDDLE :
                   pos = *current;
                   tsupmid[pos] = a;
                   (*current)++;
                   break;
            case CP_UPPEST :
                   pos = --(*current);
                   if(tsupupp[pos]==' ')
                      tsupupp[pos]=a;
                   else
                      tsuptop[pos]=a;
                   (*current)++;
                   break;
            case CP_UPPER :
                   pos = --(*current);
                   tsupupp[pos] = a;
                   (*current)++;
                   break;
            case CP_LOWER :
                   pos = --(*current);
                   tsupbel[pos] = a;
                   (*current)++;
                   break;
       }
}

void subscript(int *ch, int *current)
{
       int level;
       int pos;
       unsigned char a = *ch ;
       level = tislevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
            case CP_MIDDLE :
                   pos = *current;
                   tsubmid[pos] = a;
                   (*current)++;
                   break;
            case CP_UPPEST :
                   pos = --(*current);
                   if(tsubupp[pos]==' ')
                      tsubupp[pos]=a;
                   else
                      tsubtop[pos]=a;
                   (*current)++;
                   break;
            case CP_UPPER :
                   pos = --(*current);
                   tsubupp[pos] = a;
                   (*current)++;
                   break;
            case CP_LOWER :
                   pos = --(*current);
                   tsubbel[pos] = a;
                   (*current)++;
                   break;
       }
}
/* ---------------------------------- */
/* check if print code is what level, */
/* upper level then return 1          */
/* middle level "     "    2          */
/* below  level "     "    3          */
/* ---------------------------------- */
/* Modify: Suttipong Kanakakorn Sat  10-28-1989  10:54:49 */
int tislevel(int *line)
{
    if( *line < 0x80)
        return(CP_MIDDLE);
    else
        return(cp_thaitable[*line - 0x80]) ;
}

/* modify by Suttipong Kanakakorn Tue  08-29-1989  12:00:45 */
void tprint_three(void)
{
    extern int linespace ;
    extern int printer24pin;
    int spleft;

    underlineactive = 0;

    if (printer24pin == YES) {
        /* 53/180 inch per line */
        putpstring(tsuptop);   PrinterLineFeed180inch(6);
        putpstring(tsupupp);   PrinterLineFeed180inch(0);
        putpstring(tnortop);   PrinterLineFeed180inch(9);
        putpstring(tnorupp);   PrinterLineFeed180inch(0);
        putpstring(tsubtop);   PrinterLineFeed180inch(9);
        putpstring(tsupmid);   PrinterLineFeed180inch(0);
        putpstring(tsubupp);   PrinterLineFeed180inch(9);
        underlineactive = 1;
        putpstring(tnormid);   PrinterLineFeed180inch(9);
        underlineactive = 0;
        putpstring(tsupbel);   PrinterLineFeed180inch(00);
        underlineactive = 1;
        putpstring(tsubmid);  PrinterLineFeed180inch(9);
        underlineactive = 0;
        putpstring(tnorbel);   PrinterLineFeed180inch(9);
        putpstring(tsubbel);   PrinterLineFeed180inch(00);

        if(linespace>14) {
          PrinterLineFeed180inch(14);
          spleft=linespace-14;
          putpstring(textbar);
          while(spleft>24) {
            PrinterLineFeed180inch(24);
            putpstring(textbar);
            spleft-=24;
          }
          PrinterLineFeed180inch(spleft);
        } else {
          putpstring(textbar);
          PrinterLineFeed180inch(linespace);
        }
    } else {
        putpstring(tsuptop);   PrinterLineFeed216inch(7);
        putpstring(tsupupp);   PrinterLineFeed216inch(0);
        putpstring(tnortop);   PrinterLineFeed216inch(10);
        putpstring(tnorupp);   PrinterLineFeed216inch(0);
        putpstring(tsubtop);   PrinterLineFeed216inch(10);
        putpstring(tsupmid);   PrinterLineFeed216inch(0);
        putpstring(tsubupp);   PrinterLineFeed216inch(10);
        underlineactive = 1;
        putpstring(tnormid);   PrinterLineFeed216inch(10);
        underlineactive = 0;
        putpstring(tsupbel);   PrinterLineFeed216inch(00);
        underlineactive = 1;
        putpstring(tsubmid);  PrinterLineFeed216inch(10);
        underlineactive = 0;
        putpstring(tnorbel);   PrinterLineFeed216inch(10);
        putpstring(tsubbel);   PrinterLineFeed216inch(00);
        if(linespace>14) {
          PrinterLineFeed216inch(14);
          spleft=linespace-14;
          putpstring(textbar);
          while(spleft>24) {
            PrinterLineFeed216inch(24);
            putpstring(textbar);
            spleft-=24;
          }
          PrinterLineFeed216inch(spleft);
        } else {
          putpstring(textbar);
          PrinterLineFeed216inch(linespace);
        }
    }
/*
    putpstring(tsuptop);   PrinterLineFeed180inch(6);
    putpstring(tsupupp);   PrinterLineFeed180inch(0);
    putpstring(tnortop);   PrinterLineFeed180inch(9);
    putpstring(tnorupp);   PrinterLineFeed180inch(0);
    putpstring(tsubtop);   PrinterLineFeed180inch(9);
    putpstring(tsupmid);   PrinterLineFeed180inch(0);
    putpstring(tsubupp);   PrinterLineFeed180inch(9);
    underlineactive = 1;
    putpstring(tnormid);   PrinterLineFeed180inch(9);
    underlineactive = 0;
    putpstring(tsupbel);   PrinterLineFeed180inch(00);
    underlineactive = 1;
    putpstring(tsubmid);  PrinterLineFeed180inch(9);
    underlineactive = 0;
    putpstring(tnorbel);   PrinterLineFeed180inch(9);
    putpstring(tsubbel);   PrinterLineFeed180inch(00);
*/
/*
    putpstring(tsuptop);   PrinterLineFeed180inch(00);     putpstring(tsupupp);  PrinterLineFeed180inch(10);
    putpstring(tnortop);   PrinterLineFeed180inch(00);     putpstring(tnorupp);  PrinterLineFeed180inch(10);
    putpstring(tsupmid);   PrinterLineFeed180inch(00);
    putpstring(tsubtop);   PrinterLineFeed180inch(00);     putpstring(tsubupp);  PrinterLineFeed180inch(10);
    underlineactive = 1;
    putpstring(tnormid);   PrinterLineFeed180inch(10);
    underlineactive = 0;
    putpstring(tsupbel);   PrinterLineFeed180inch(00);
    underlineactive = 1;
    putpstring(tsubmid);   PrinterLineFeed180inch(10);
    underlineactive = 0;
    putpstring(tnorbel);   PrinterLineFeed180inch(10);
    putpstring(tsubbel);   PrinterLineFeed180inch(00);
*/
}

void putpstring(unsigned char s[])
{
    extern int nlqmode;
    extern int prtcodestd;  /* code of print set in pmenu. */
    int i=0;
    if(nlqmode==0){putp(ESC);putp('x');putp(0);}
    else          {putp(ESC);putp('x');putp(1);}
    printattr(0); /* set to normal print */
    while(s[i] != '\0'){
         if(s[i] == ESC){
           printattr(s[++i]); i++;   /* print followed attribute. */
         }
         else{
             if(prtcodestd == 0) putp(stdtoku(s[i++]));
             else putp(s[i++]);
         }
    }
}
