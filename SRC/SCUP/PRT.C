/* ---------------------------------------------------- */
/* Program     : PRT.C                                  */
/* Called from : PMENU.C                                */
/* Function    : Operate character's occurence on paper */
/* Last update : 22/3/31                                */
/* last update : 11/4/31                                */
/* Last update : 13/5/31                                */
/* Last update : 02/10/31   add cpi funct.              */
/* Last update : 10/4/32    modify converse funct       */
/* Last update : 9/5/32     delete some functions.      */
/* Last update : Mon  08-07-1989  01:53:29              */
/* By Suttipong Kanakakorn                              */
/* Clean all the junk                                   */
/*               Tue  08-15-1989  00:53:30              */
/* better approach to load font                         */
/*               Tue  08-29-1989  18:02:18              */
/* add many function                                    */
/* By CUCC                                              */
/*   Extend Bar                                         */
/* ---------------------------------------------------- */

/*  constant   area   */
#define  WIDTH       24
#define  ADJUST(x) (x>=0) ? x : 256+x
#define  ASCII_NO   256
 /* ------------------- */

/*   heading           */
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#include "inc.h"
#include "24pins.h"
#include "global.ext"

typedef struct {
       char byte1;
       char byte2;
       char byte3;
       } buffer24pin;
buffer24pin *print24buffer;
void converse(char s[]);
void PrinterLoadLine(char pline[])                                   ;
void clearbuffer(void)                                               ;
int adj_attr(font_attr *attr, int *ch)                               ;
void pretobuffer(font_attr *attr, int *ch, int *current)             ;
void blockgraphic(int *pch,int *pcurrent)                            ;
void normal_normal(int *pch,int *pcurrent,int pf,int pu)             ;
void normal_bold(int *pch,int *pcurrent,int pf,int pu)               ;
void normal_enlarge(int *pch,int *pcurrent,int pf,int pu)            ;
void normal_bold_enlarge(int *pch,int *pcurrent,int pf,int pu)       ;
void subscript_normal(int *pch,int *pcurrent,int pu)                 ;
void subscript_bold(int *pch,int *pcurrent,int pu)                   ;
void subscript_enlarge(int *pch,int *pcurrent,int pu)                ;
void subscript_bold_enlarge(int *pch,int *pcurrent,int pu)           ;
void superscript_normal(int *pch,int *pcurrent,int pu)               ;
void superscript_bold(int *pch,int *pcurrent,int pu)                 ;
void superscript_enlarge(int *pch,int *pcurrent,int pu)              ;
void superscript_bold_enlarge(int *pch,int *pcurrent,int pu)         ;
int  islevel(int *pline)                                             ;
void converse(char *ps)                                              ;
void swapwanayuk(char *ps)                                           ;
/* ---------------------------------------------------------------- */
/* --------------------------------- */
/* ®—¥·∫Ëß√À— Õ—°…√‡™Ëπ ” ª Ø ∞ ‡ªÁπ 2 √À—  */
/* --------------------------------- */
void converse(char *s)
{
  int i,j,p    ;
  char c[500]  ;
  int r ;
  i = j = 0 ;
  while( (p = s[i++]) != '\0' ) {
       p = ADJUST(p) ;
       if(p < 0xa0 )
         c[j++] = p ;
       else {
          r = table[p - 0xa0]    ;
          if(r == 0x00)
            c[j++] = p ;
          else {
             if((r & 0x80) != 0x00)  {
               c[j++] = p ;
               c[j++] = r ;
             }
             else {
                c[j++] = r | 0x80 ;
                c[j++] = p        ;
             }
          }
       }
  }
  c[j] = '\0' ;
  strcpy(s,c);
}
/* --------------------------------- */
/* to swap wanayuk and upper wovel   */
/* --------------------------------- */
void swapwanayuk(char *s)
{
  int i,j,p    ;
  char c[500]  ;
  i = j = 0 ;
  while( (p = s[i++]) != '\0' ) {
       p = ADJUST(p) ;
       if(p < 0xe8 || p > 0xec)  /* not wanayuk */
         c[j++] = p ;
       else {
          if(s[i]==0xd1 || (s[i]>0xd2 && s[i]<0xd8)){
            c[j++]=s[i++];   /* exchange position */
            c[j++]=p;        /* ----------------- */
          }
          else c[j++]=p;
       }
  }
  c[j] = '\0' ;
  strcpy(s,c);
}

/* ------------------------ */
/*  Load 3 fonts of printer */
/* ------------------------ */
int PrinterLoadFont9pin(void)
{
    font[0] = cp_loadfont("normal.prn", 11264);
    font[1] = cp_loadfont("italic.prn", 11264);
    fonts   = cp_loadfont("normals.prn", 11264);
}

/* -------------------------------------------- */
/* Print string 'line' to printer               */
/* this module is called from external routine. */
/* -------------------------------------------- */
void PrinterLoadLine9pin(char line[])
{
    extern int printer24pin             ;   /* define in pmenu.c */
    font_attr  attr             ;
    auto int  current                   ;   /* current dot position */
    int  ch                             ;
        current   = 0;
        attr      = 0;
        clearbuffer();
        converse(line);
        swapwanayuk(line);
        while (*line != '\0' ) {
               ch    = ADJUST(*line);  /* adjust to positive  */
               if (ch < 24) {          /* codes under 24 are control or attribute */
                  attr = adj_attr(&attr,&ch) ;
               }
               else {
                  pretobuffer(&attr,&ch,&current);
               }
               line++   ;
         }
     PrintThree9pin();
}

void AllocateBuffer9pin(void)
{
 char *b[12];
 int i;
 static int done=0;                    /* Avoid re-alloc mem */

 if(!done) {
   for (i=0;i<12;i++) {
     b[i]=(char *) calloc(6600,sizeof(char));
     if (b[i]==NULL)
        execerror(" ÀπË«¬§«“¡®”‰¡ËæÕ ! °¥ªÿË¡„¥Ê‡æ◊ËÕ∑”ß“πµËÕ ","");
   }
   bupper1  = b[0];  bupper2  = b[1];  bupper3  = b[2];
   bmiddle1 = b[3];  bmiddle2 = b[4];  bmiddle3 = b[5];
   bbelow1  = b[6];  bbelow2  = b[7];  bbelow3  = b[8];
   extbar1  = b[9];  extbar2  = b[10]; extbar3  = b[11];
   done = 1;
 }
}

void clearbuffer(void)
{
    extern int maxbuffer; /* define in pmenu.c */
    memset(bupper1,0,maxbuffer);
    memset(bupper2,0,maxbuffer);
    memset(bmiddle1,0,maxbuffer);
    memset(bmiddle2,0,maxbuffer);
    memset(bbelow1,0,maxbuffer);
    memset(bbelow2,0,maxbuffer);
    if(extbarprinting) {
      memset(extbar1,0,maxbuffer);
      memset(extbar2,0,maxbuffer);
    }
}
/* ------------------------------------   */
/* This module operate on all attribute   */
/* Some attributes are tokkle to itself   */
/* Some, effect to the others             */
/* Example ,superscript attr, sets itself */
/* and also resets subscript attribute .  */
/* ------------------------------------   */
int adj_attr(font_attr *attr, int *ch)
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
/* Checking type of character appearance   */
/* e.g. block,boldface enlarge,normal ect. */
/* --------------------------------------  */
void pretobuffer(font_attr *attr, int *ch, int *current)
{
  int cont , f,u ;
  /* ......gned)*ch > 0x7e)...  when need to include double line block */

  if(((unsigned)*ch > 0x8e) && ((unsigned) *ch < 0x9e)){
     blockgraphic(ch,current)  ;
  }
  else
    {
     cont  = *attr & 0x0f ;
     f  = (*attr & 0x20)  >> 5 ;
     u  = (*attr & 0x40)  | (*attr & 0x10) | ((*attr & 0x10) << 2)  ;
        switch (cont) {
             case 0x00 :
                       normal_normal(ch,current,f,u) ;
                       break;
             case 0x01 :
                       normal_bold(ch,current,f,u)  ;
                       break;
             case 0x02 :
                       normal_enlarge(ch,current,f,u) ;
                       break;
             case 0x03 :
                       normal_bold_enlarge(ch,current,f,u) ;
                       break;
             case 0x04 :
                       superscript_normal(ch,current,u) ;
                       break;
             case 0x05 :
                       superscript_bold(ch,current,u) ;
                       break;
             case 0x06 :
                       superscript_enlarge(ch,current,u) ;
                       break;
             case 0x07 :
                       superscript_bold_enlarge(ch,current,u) ;
                      break;
             case 0x08 :
                       subscript_normal(ch,current,u) ;
                       break;
             case 0x09 :
                       subscript_bold(ch,current,u) ;
                       break;
             case 0x0a :
                       subscript_enlarge(ch,current,u) ;
                       break;
             case 0x0b :
                       subscript_bold_enlarge(ch,current,u) ;
                       break;
              default  :
                       normal_normal(ch,current,f,u) ;
                       break;
       }
  }
}
/* --------------------------------------------- */
/* Invention of graphic blocks to build tables   */
/* this include both single and double lineblock */
/* such as ò ë ô ì ñ í ö ê õ ï è                 */
/* --------------------------------------------- */
void blockgraphic(int *ch, int *current)
{
       int g,u,m,b,i,pos       ;
       int a                 ;
       a = *ch               ;
       if(a < 0x8f) {                   /* Case of double line block */
         g = tgtable[a - 0x7f] ;         /* Actually, due to the two codes */
         u = 0x86 * ((g & 0x02) >> 1) ; /* '8a','8d'(reform code) is */
         m = a     ;                    /* reserved.So this type of block */
         b = 0x86 * (g & 0x01) ;        /* (code7f-8d) is  unable to use. */
       }
       else {
         g = tgtable[a - 0x8f] ;
         u = (g & 0x02) ? 0x96 : 0x20;
         m = a     ;
         b = (g & 0x01) ? 0x96 : 0x20;
       }
       pos = *current * WIDTH ;
                   for(i=0;i<22;i++) {
                      bupper1[pos]    = font[0][(u * 44)+i]     ;
                      bupper2[pos]    = font[0][(u * 44)+i+22]  ;
                      bmiddle1[pos]   = font[0][(m * 44)+i]     ;
                      bmiddle2[pos]   = font[0][(m * 44)+i+22]  ;
                      bbelow1[pos]    = font[0][(b * 44)+i]     ;
                      bbelow2[pos++]  = font[0][(b * 44)+i+22]  ;
                      if(extbarprinting) {
                        pos--;
                        extbar1[pos]   = font[0][(b * 44)+i]    ;
                        extbar2[pos++] = font[0][(b * 44)+i+22] ;
                      }
                   }
/* More 2 Column ( Actually there are 24 Dots per character ) */
                   bupper1[pos]     =  bupper1[pos-1]   ;
                   bupper2[pos]     =  bupper2[pos-1]   ;
                   bmiddle1[pos]    =  bmiddle1[pos-1]  ;
                   bmiddle2[pos]    =  bmiddle2[pos-1]  ;
                   bbelow1[pos]     =  bbelow1[pos-1]   ;
                   bbelow2[pos]     =  bbelow2[pos-1]   ; pos++;
                   bupper1[pos]     =  bupper1[pos-1]   ;
                   bupper2[pos]     =  bupper2[pos-1]   ;
                   bmiddle1[pos]    =  bmiddle1[pos-1]  ;
                   bmiddle2[pos]    =  bmiddle1[pos-1]  ;
                   bbelow1[pos]     =  bbelow1[pos-1]   ;
                   bbelow2[pos]     =  bbelow2[pos-1]   ; pos++;
                   (*current)++;
}
void normal_normal(int *ch, int *current, int f, int u)
{
       int sh = 0            ;
       int level             ;
       int i               ;
       int pos               ;
       unsigned char a = *ch ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
            case 2 :
                   pos = *current * WIDTH ;
                   for(i=0;i<22;i++) {
                      bmiddle1[pos]   = font[f][((a) * 44)+i]     ;
                      bbelow2[pos] = u ;
                      bmiddle2[pos++] = font[f][((a) * 44)+i+22]    ;
                   }
                   bmiddle1[pos]    = 0    ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]  = 0    ;
                   bmiddle1[pos]    = 0    ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]  = 0    ;
                   (*current)++;
                   break;
            case 1 :
                   pos = --(*current) * WIDTH ;
                   sh = ( (a>0xe7) & (a<0xed) & ((bupper1[pos+15]|bupper2[pos+15])!=0)) ? 4 : 0 ;
                   for(i=0;i<22;i++) {
                      bupper1[pos]   |= font[f][((a) * 44)+i]    << sh    ;
                      bupper2[pos++] |= font[f][((a) * 44)+i+22] << sh    ;
                   }
                   bupper1[pos]    |= 0 ;
                   bupper2[pos++]  |= 0 ;
                   bupper1[pos]    |= 0 ;
                   bupper2[pos++]  |= 0 ;
                   (*current)++;
                   break;
            case 3 :
                   pos = --(*current) * WIDTH ;
                   for(i=0;i<22;i++) {
                      bbelow1[pos]   = font[f][((a) * 44)+i] ;
                      bbelow2[pos++] = font[f][((a) * 44)+i+22] ;
                   }
                   bbelow1[pos]    = 0 ;
                   bbelow2[pos++]  = 0 ;
                   bbelow1[pos]    = 0 ;
                   bbelow2[pos++]  = 0 ;
                   (*current)++;
                   break;
       }  /* end switch */
}

void normal_bold(int *ch, int *current, int f, int u)
{
  int sh    ;
  int level             ;
  int i		     ;
  int pos               ;
  unsigned char a = *ch ;
  level = islevel(ch)   ;        /* get its type of 3-level */
  switch (level) {
       case 2 :
              pos = *current * WIDTH ;
              bmiddle1[pos]    =  font[f][((a) * 44)]       ;
              bbelow2[pos] = u ;
              bmiddle2[pos++]  =  font[f][((a) * 44)+22]     ;
              for(i=1;i<22;i++) {
               bmiddle1[pos]   = font[f][((a) * 44)+i] |
                                 font[f][((a) * 44)+i-1]     ;
               bbelow2[pos] = u ;
               bmiddle2[pos++] = font[f][((a) * 44)+i+22] |
                                 font[f][((a) * 44)+i+21]     ;
              }
              bmiddle1[pos]    = 0    ;
              bbelow2[pos] = u ;
              bmiddle2[pos++]  = 0    ;
              bmiddle1[pos]    = 0    ;
              bbelow2[pos] = u ;
              bmiddle2[pos++]  = 0    ;
              (*current)++;
              break;
       case 1 :
              pos = --(*current) * WIDTH ;
              sh = ( (a>0xe7) & (a<0xed) & ((bupper1[pos+15]|bupper2[pos+15])!=0)) ? 4 : 0 ;
              bupper1[pos]    |=  font[f][((a) * 44)]    << sh;
              bupper2[pos++]  |=  font[f][((a) * 44)+22] << sh;
              for(i=1;i<22;i++) {               		
                 bupper1[pos]   |= (font[f][((a) * 44)+i]    |
                                  font[f][((a) * 44)+i-1]) << sh  ;
                 bupper2[pos++] |= (font[f][((a) * 44)+i+22] |
                                  font[f][((a) * 44)+i+21]) << sh ;
              }
              bupper1[pos]    |= 0 ;
              bupper2[pos++]  |= 0 ;
              bupper1[pos]    |= 0 ;
              bupper2[pos++]  |= 0 ;
              (*current)++;
              break;
       case 3 :
              pos = --(*current) * WIDTH ;
              bbelow1[pos]    =  font[f][((a) * 44)]    ;
              bbelow2[pos++]  =  font[f][((a) * 44)+22]  ;
              for(i=1;i<22;i++) {
                 bbelow1[pos]   = font[f][((a) * 44)+i] |
                                  font[f][((a) * 44)+i-1] ;
                 bbelow2[pos++] = font[f][((a) * 44)+i+22] |
                                  font[f][((a) * 44)+i+21] ;
              }
              bbelow1[pos]    = 0;
              bbelow2[pos++]  = 0;
              bbelow1[pos]    = 0;
              bbelow2[pos++]  = 0;
              (*current)++;
              break;
  }  /* end switch */
}
void normal_enlarge(int *ch, int *current, int f, int u)
{
       int sh  ;
       int level             ;
       int i     ;
       int pos               ;
       unsigned char a = *ch ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
            case 2 :
                   pos = *current * WIDTH ;
                   for(i=0;i<22;i++) {
                      bmiddle1[pos]   = font[f][((a) * 44)+i]     ;
                      bbelow2[pos] = u ;
                      bmiddle2[pos++] = font[f][((a) * 44)+i+22]     ;
                      bmiddle1[pos]   = font[f][((a) * 44)+i]     ;
                      bbelow2[pos] = u ;
                      bmiddle2[pos++] = font[f][((a) * 44)+i+22]     ;
                   }
                   bmiddle1[pos]      = font[f][((a) * 44)+21]     ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]    = font[f][((a) * 44)+43]     ;
                   bmiddle1[pos]      = 0    ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]    = 0    ;
                   bmiddle1[pos]      = 0    ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]    = 0    ;
                   bmiddle1[pos]      = 0    ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]    = 0    ;
                   (*current)++;
                   (*current)++;
                   break;
            case 1 :
                   --(*current) ;
                   pos = --(*current) * WIDTH ;
                   sh = ( (a>0xe7) & (a<0xed) & ((bupper1[pos+30]|bupper2[pos+30])!=0)) ? 4 : 0 ;
                   for(i=0;i<22;i++) {
                      bupper1[pos]   |= font[f][((a) * 44)+i]    << sh ;
                      bupper2[pos++] |= font[f][((a) * 44)+i+22] << sh ;
                      bupper1[pos]   |= font[f][((a) * 44)+i]    << sh ;
                      bupper2[pos++] |= font[f][((a) * 44)+i+22] << sh ;
                   }
                   bupper1[pos]    |= font[f][((a) * 44)+21]  << sh ;
                   bupper2[pos++]  |= font[f][((a) * 44)+43]  << sh ;
                   bupper1[pos]    |= 0;
                   bupper2[pos++]  |= 0;
                   bupper1[pos]    |= 0;
                   bupper2[pos++]  |= 0;
                   bupper1[pos]    |= 0;
                   bupper2[pos++]  |= 0;
                   (*current)++;
                   (*current)++;
                   break;
            case 3 :
                   --(*current)  ;
                   pos = --(*current) * WIDTH ;
                   for(i=0;i<22;i++) {
                      bbelow1[pos]   = font[f][((a) * 44)+i] ;
                      bbelow2[pos++] = font[f][((a) * 44)+i+22] ;
                      bbelow1[pos]   = font[f][((a) * 44)+i] ;
                      bbelow2[pos++] = font[f][((a) * 44)+i+22] ;
                   }
                   bbelow1[pos]    = font[f][((a) * 44)+21]    ;
                   bbelow2[pos++]  = font[f][((a) * 44)+43] ;
                   bbelow1[pos]    = 0;
                   bbelow2[pos++]  = 0;
                   bbelow1[pos]    = 0;
                   bbelow2[pos++]  = 0;
                   bbelow1[pos]    = 0;
                   bbelow2[pos++]  = 0;
                   (*current)++;
                   (*current)++;
                   break;
       }  /* end switch */
}
void normal_bold_enlarge(int *ch, int *current, int f, int u)
{
       int sh  ;
       int level             ;
       int i		     ;
       int pos               ;
       unsigned char a = *ch ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
            case 2 :
                   pos = *current * WIDTH ;
                   bmiddle1[pos]   = font[f][((a) * 44)]     ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++] = font[f][((a) * 44)+22]     ;
                   bmiddle1[pos]   = font[f][((a) * 44)]     ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++] = font[f][((a) * 44)+22]     ;
                   for(i=1;i<22;i++) {
                      bmiddle1[pos]   = font[f][((a) * 44)+i] |
                                        font[f][((a) * 44)+i-1]     ;
                      bbelow2[pos] = u ;
                      bmiddle2[pos++] = font[f][((a) * 44)+i+22] |
                                        font[f][((a) * 44)+i+21]     ;
                      bmiddle1[pos]   = font[f][((a) * 44)+i] |
                                        font[f][((a) * 44)+i-1]     ;
                      bbelow2[pos] = u ;
                      bmiddle2[pos++] = font[f][((a) * 44)+i+22] |
                                        font[f][((a) * 44)+i+21]     ;
                   }
                   bmiddle1[pos]      = font[f][((a) * 44)+21] |
                                        font[f][((a) * 44)+20]     ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]    = font[f][((a) * 44)+43] |
                                        font[f][((a) * 44)+42]     ;
                   bmiddle1[pos]      = 0    ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]    = 0    ;
                   bmiddle1[pos]      = 0    ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]    = 0    ;
                   bmiddle1[pos]      = 0    ;
                   bbelow2[pos] = u ;
                   bmiddle2[pos++]    = 0    ;
                   (*current)++;
                   (*current)++;
                   break;
            case 1 :
                   --(*current) ;
                   pos = --(*current) * WIDTH ;
                   sh = ( (a>0xe7) & (a<0xed) & ((bupper1[pos+30]|bupper2[pos+30])!=0)) ? 4 : 0 ;
                   bupper1[pos]   |= font[f][((a) * 44)]     << sh;
                   bupper2[pos++] |= font[f][((a) * 44)+22]  << sh;
                   bupper1[pos]   |= font[f][((a) * 44)]     << sh;
                   bupper2[pos++] |= font[f][((a) * 44)+22]  << sh;
                   for(i=1;i<22;i++) {
                      bupper1[pos]   |= (font[f][((a) * 44)+i] |
                                        font[f][((a) * 44)+i-1] )<< sh ;
                      bupper2[pos++] |= (font[f][((a) * 44)+i+22] |
                                        font[f][((a) * 44)+i+21])<< sh  ;
                      bupper1[pos]   |= (font[f][((a) * 44)+i] |
                                        font[f][((a) * 44)+i-1] )<< sh ;
                      bupper2[pos++] |= (font[f][((a) * 44)+i+22] |
                                        font[f][((a) * 44)+i+21])<< sh  ;
                   }
                   bupper1[pos]      |= (font[f][((a) * 44)+21] |
                                        font[f][((a) * 44)+20] ) << sh;
                   bupper2[pos++]    |= (font[f][((a) * 44)+43] |
                                        font[f][((a) * 44)+42])  << sh;
                   bupper1[pos]      |= 0 ;
                   bupper2[pos++]    |= 0 ;
                   bupper1[pos]      |= 0 ;
                   bupper2[pos++]    |= 0 ;
                   bupper1[pos]      |= 0 ;
                   bupper2[pos++]    |= 0 ;
                   (*current)++;
                   (*current)++;
                   break;
            case 3 :
                   --(*current)  ;
                   pos = --(*current) * WIDTH ;
                   bbelow1[pos]   = font[f][((a) * 44)]  ;
                   bbelow2[pos++] = font[f][((a) * 44)+22]  ;
                   bbelow1[pos]   = font[f][((a) * 44)]  ;
                   bbelow2[pos++] = font[f][((a) * 44)+22]  ;
                   for(i=1;i<22;i++) {
                      bbelow1[pos]   = font[f][((a) * 44)+i] |
                                        font[f][((a) * 44)+i-1]  ;
                      bbelow2[pos++] = font[f][((a) * 44)+i+22] |
                                        font[f][((a) * 44)+i+21]  ;
                      bbelow1[pos]   = font[f][((a) * 44)+i] |
                                        font[f][((a) * 44)+i-1]  ;
                      bbelow2[pos++] = font[f][((a) * 44)+i+22] |
                                        font[f][((a) * 44)+i+21]  ;
                   }
                   bbelow1[pos]      = font[f][((a) * 44)+21] |
                                        font[f][((a) * 44)+20]  ;
                   bbelow2[pos++]    = font[f][((a) * 44)+43] |
                                        font[f][((a) * 44)+42]  ;
                   bbelow1[pos]      = 0 ;
                   bbelow2[pos++]    = 0 ;
                   bbelow1[pos]      = 0 ;
                   bbelow2[pos++]    = 0 ;
                   bbelow1[pos]      = 0 ;
                   bbelow2[pos++]    = 0 ;
                   (*current)++;
                   (*current)++;
                   break;
       }  /* end switch */
}
void superscript_normal(int *ch, int *current, int u)
{
       int level             ;
       int i     ;
       int pos               ;
       unsigned char a = *ch ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
         case 2 :
            pos = *current *  WIDTH ;
            for(i=0;i<22;i++) {
               bupper1[pos]   = fonts[((a) * 44)+i]          ;
               bmiddle2[pos]  = u ;
               bupper2[pos++] = fonts[((a) * 44)+i+22]       ;
            }
            bupper1[pos]    = 0    ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = 0    ;
            bupper1[pos]    = 0    ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = 0    ;
            (*current)++;
            break;
         case 1 :
            pos = --(*current) * WIDTH ;
            for(i=0;i<22;i++) {
               bupper1[pos]   |= fonts[((a) * 44)+i]     ;
               bupper2[pos++] |= fonts[((a) * 44)+i+22]  ;
            }
            bupper1[pos]    = 0;
            bupper2[pos++]  = 0;
            bupper1[pos]    = 0;
            bupper2[pos++]  = 0;
            (*current)++;
            break;
         case 3 :
            pos = --(*current) * WIDTH ;
            for(i=0;i<22;i++) {
               bmiddle1[pos]   |= fonts[((a) * 44)+i]     ;
               bmiddle2[pos++] |= fonts[((a) * 44)+i+22]  ;
            }
            bmiddle1[pos]    = 0;    /* these four lines may be ignor */
            bmiddle2[pos++]  = 0;
            bmiddle1[pos]    = 0;
            bmiddle2[pos++]  = 0;
            (*current)++;
            break;
       }  /* end switch */
}

void superscript_bold(int *ch, int *current, int u)
{
       int level             ;
       int i		     ;
       int pos               ;
       unsigned char a = *ch ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
         case 2 :
            pos = *current *  WIDTH ;
            bupper1[pos]   = fonts[((a) * 44)]          ;
            bmiddle2[pos]  = u ;
            bupper2[pos++] = fonts[((a) * 44)+22]       ;
            for(i=0;i<21;i++) {
               bupper1[pos]   = fonts[((a) * 44)+i]      |
                                fonts[((a) * 44)+i+1]       ;
               bmiddle2[pos]  = u ;
               bupper2[pos++] = fonts[((a) * 44)+i+22]   |
                                fonts[((a) * 44)+i+23]       ;
            }
            bupper1[pos]    = 0    ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = 0    ;
            bupper1[pos]    = 0    ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = 0    ;
            (*current)++;
            break;
         case 1 :
            pos = --(*current) * WIDTH ;
            bupper1[pos]   |= fonts[((a) * 44)]     ;
            bupper2[pos++] |= fonts[((a) * 44)+22]  ;
            for(i=0;i<21;i++) {
               bupper1[pos]   |= fonts[((a) * 44)+i] | fonts[((a) * 44)+i+1] ;
               bupper2[pos++] |= fonts[((a) * 44)+i+22] | fonts[((a) * 44)+i+23]  ;
            }
            bupper1[pos]    = 0;
            bupper2[pos++]  = 0;
            bupper1[pos]    = 0;
            bupper2[pos++]  = 0;
            (*current)++;
            break;
         case 3 :
            pos = --(*current) * WIDTH ;
            bmiddle1[pos]   |= fonts[((a) * 44)]     ;
            bmiddle2[pos++] |= fonts[((a) * 44)+22]  ;
            for(i=0;i<21;i++) {
               bmiddle1[pos]   |= fonts[((a) * 44)+i] | fonts[((a) * 44)+i+1] ;
               bmiddle2[pos++] |= fonts[((a) * 44)+i+22] | fonts[((a) * 44)+i+23] ;
            }
            bmiddle1[pos]    = 0;    /* these four lines may be ignor */
            bmiddle2[pos++]  = 0;
            bmiddle1[pos]    = 0;
            bmiddle2[pos++]  = 0;
            (*current)++;
            break;
       }  /* end switch */
}
void superscript_enlarge(int *ch, int *current, int u)
{
       int level             ;
       int i		               ;
       int pos               ;
       unsigned char a = *ch ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
         case 2 :
            pos = *current *  WIDTH ;
            for(i=0;i<22;i++) {
               bupper1[pos]   = fonts[((a) * 44)+i]          ;
               bmiddle2[pos]  = u ;
               bupper2[pos++] = fonts[((a) * 44)+i+22]       ;
               bupper1[pos]   = fonts[((a) * 44)+i]          ;
               bmiddle2[pos]  = u ;
               bupper2[pos++] = fonts[((a) * 44)+i+22]       ;
            }
            bupper1[pos]    = fonts[((a) * 44)+21]       ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = fonts[((a) * 44)+43]       ;
            bupper1[pos]    = 0    ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = 0    ;
            (*current)++;
            (*current)++;
            break;
         case 1 :
            --(*current) ;
            pos = --(*current) * WIDTH ;
            for(i=0;i<22;i++) {
               bupper1[pos]   |= fonts[((a) * 44)+i]     ;
               bupper2[pos++] |= fonts[((a) * 44)+i+22]  ;
               bupper1[pos]   |= fonts[((a) * 44)+i]     ;
               bupper2[pos++] |= fonts[((a) * 44)+i+22]  ;
            }
            bupper1[pos]    = fonts[((a) * 44)+21]   ;
            bupper2[pos++]  = fonts[((a) * 44)+43]   ;
            bupper1[pos]    = 0;
            bupper2[pos++]  = 0;
            (*current)++;
            (*current)++;
            break;
         case 3 :
            --(*current) ;
            pos = --(*current) * WIDTH ;
            for(i=0;i<22;i++) {
               bmiddle1[pos]   |= fonts[((a) * 44)+i]     ;
               bmiddle2[pos++] |= fonts[((a) * 44)+i+22]  ;
               bmiddle1[pos]   |= fonts[((a) * 44)+i]     ;
               bmiddle2[pos++] |= fonts[((a) * 44)+i+22]  ;
            }
            bmiddle1[pos]    = fonts[((a) * 44)+21]   ;    /* these four lines may be ignor */
            bmiddle2[pos++]  = fonts[((a) * 44)+43]   ;
            bmiddle1[pos]    = 0;
            bmiddle2[pos++]  = 0;
            (*current)++;
            (*current)++;
            break;
       }  /* end switch */
}
void superscript_bold_enlarge(int *ch, int *current, int u)
{
       int level             ;
       int i		     ;
       int pos               ;
       unsigned char a = *ch ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
         case 2 :
            pos = *current *  WIDTH ;
            bupper1[pos]   = fonts[((a) * 44)]          ;
            bmiddle2[pos]  = u ;
            bupper2[pos++] = fonts[((a) * 44)+22]       ;
            bupper1[pos]   = fonts[((a) * 44)]          ;
            bmiddle2[pos]  = u ;
            bupper2[pos++] = fonts[((a) * 44)+22]       ;
            for(i=0;i<21;i++) {
               bupper1[pos]   = fonts[((a) * 44)+i]      |
                                fonts[((a) * 44)+i+1]       ;
               bmiddle2[pos]  = u ;
               bupper2[pos++] = fonts[((a) * 44)+i+22]   |
                                fonts[((a) * 44)+i+23]       ;
               bupper1[pos]   = fonts[((a) * 44)+i]      |
                                fonts[((a) * 44)+i+1]       ;
               bmiddle2[pos]  = u ;
               bupper2[pos++] = fonts[((a) * 44)+i+22]   |
                                fonts[((a) * 44)+i+23]       ;
            }
            bupper1[pos]    = fonts[((a) * 44)+21]      |
                              fonts[((a) * 44)+20]      ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = fonts[((a) * 44)+43]      |
                              fonts[((a) * 44)+42]       ;
            bupper1[pos]    = 0    ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = 0    ;
            bupper1[pos]    = 0    ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = 0    ;
            bupper1[pos]    = 0    ;
            bmiddle2[pos]  = u ;
            bupper2[pos++]  = 0    ;
            (*current)++;
            (*current)++;
            break;
         case 1 :
            --(*current)  ;
            pos = --(*current) * WIDTH ;
            bupper1[pos]   |= fonts[((a) * 44)]     ;
            bupper2[pos++] |= fonts[((a) * 44)+22]  ;
            bupper1[pos]   |= fonts[((a) * 44)]     ;
            bupper2[pos++] |= fonts[((a) * 44)+22]  ;
            for(i=0;i<21;i++) {
               bupper1[pos]   |= fonts[((a) * 44)+i] | fonts[((a) * 44)+i+1] ;
               bupper2[pos++] |= fonts[((a) * 44)+i+22] | fonts[((a) * 44)+i+23]  ;
               bupper1[pos]   |= fonts[((a) * 44)+i] | fonts[((a) * 44)+i+1] ;
               bupper2[pos++] |= fonts[((a) * 44)+i+22] | fonts[((a) * 44)+i+23]  ;
            }
            bupper1[pos]    |= fonts[((a) * 44)+20] | fonts[((a) * 44)+21] ;
            bupper2[pos++]  |= fonts[((a) * 44)+42] | fonts[((a) * 44)+43] ;
            bupper1[pos]    = 0;
            bupper2[pos++]  = 0;
            bupper1[pos]    = 0;
            bupper2[pos++]  = 0;
            bupper1[pos]    = 0;
            bupper2[pos++]  = 0;
            (*current)++;
            (*current)++;
            break;
         case 3 :
            --(*current)   ;
            pos = --(*current) * WIDTH ;
            bmiddle1[pos]   |= fonts[((a) * 44)]     ;
            bmiddle2[pos++] |= fonts[((a) * 44)+22]  ;
            bmiddle1[pos]   |= fonts[((a) * 44)]     ;
            bmiddle2[pos++] |= fonts[((a) * 44)+22]  ;
            for(i=0;i<21;i++) {
               bmiddle1[pos]   |= fonts[((a) * 44)+i] | fonts[((a) * 44)+i+1] ;
               bmiddle2[pos++] |= fonts[((a) * 44)+i+22] | fonts[((a) * 44)+i+23] ;
               bmiddle1[pos]   |= fonts[((a) * 44)+i] | fonts[((a) * 44)+i+1] ;
               bmiddle2[pos++] |= fonts[((a) * 44)+i+22] | fonts[((a) * 44)+i+23] ;
            }
            bmiddle1[pos]    |= fonts[((a) * 44)+20] | fonts[((a) * 44)+21] ;
            bmiddle2[pos++]  |= fonts[((a) * 44)+42] | fonts[((a) * 44)+43] ;
            bmiddle1[pos]    = 0;
            bmiddle2[pos++]  = 0;
            bmiddle1[pos]    = 0;
            bmiddle2[pos++]  = 0;
            bmiddle1[pos]    = 0;
            bmiddle2[pos++]  = 0;
            (*current)++;
            (*current)++;
            break;
       }  /* end switch */
}

void subscript_normal(int *ch, int *current, int u)
{
       int sh                ;
       int level             ;
       int i               ;
       int pos               ;
       unsigned char a = *ch ;
       u >>= 5               ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
         case 2 :
            pos = *current *  WIDTH ;
            for(i=0;i<22;i++) {
               bbelow1[pos]   = (fonts[((a) * 44)+i]    << 4)     ;
               bbelow2[pos++] = (fonts[((a) * 44)+i+22] << 4) | u ;
            }
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            (*current)++;
            break;
         case 1 :
            pos = --(*current) * WIDTH ;
            sh = ( (a>0xe7) & (a<0xed) & ((bmiddle1[pos+15]|bmiddle2[pos+15])!=0)) ? 2 : 0 ;
            for(i=0;i<22;i++) {
               bmiddle1[pos]   |= (fonts[((a) * 44)+i])    >> (4 - sh) ;
               bmiddle2[pos++] |= (fonts[((a) * 44)+i+22]) >> (4 - sh) ;
            }
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            (*current)++;
            break;
         case 3 :
            pos = --(*current) * WIDTH ;
            for(i=0;i<22;i++) {
               bbelow1[pos]   |= (fonts[((a) * 44)+i])    >> 3 ;
               bbelow2[pos++] |= (fonts[((a) * 44)+i+22]) >> 3 ;
            }
            bbelow1[pos]    = 0;    /* these four lines may be ignor */
            bbelow2[pos++]  = 0;
            bbelow1[pos]    = 0;
            bbelow2[pos++]  = 0;
            (*current)++;
            break;
       }  /* end switch */
}

void subscript_bold(int *ch, int *current, int u)
{
       int sh                ;
       int level             ;
       int i     ;
       int pos               ;
       unsigned char a = *ch ;
       u >>= 5               ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
         case 2 :
            pos = *current *  WIDTH ;
            bbelow1[pos]   = (fonts[((a) * 44)]    << 4)     ;
            bbelow2[pos++] = (fonts[((a) * 44)+22] << 4) | u ;
            for(i=0;i<21;i++) {
               bbelow1[pos]   = ((fonts[((a) * 44)+i]   |
                                 fonts[((a) * 44)+i+1])   << 4)     ;
               bbelow2[pos++] = ((fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23]) << 4) | u ;
            }
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            (*current)++;
            break;
         case 1 :
            pos = --(*current) * WIDTH ;
            sh = ( (a>0xe7) & (a<0xed) & ((bmiddle1[pos+15]|bmiddle2[pos+15])!=0)) ? 2 : 0 ;
            bmiddle1[pos]   |= (fonts[((a) * 44)])    >> (4 - sh) ;
            bmiddle2[pos++] |= (fonts[((a) * 44)+22]) >> (4 - sh) ;
            for(i=0;i<21;i++) {
               bmiddle1[pos]   |= (fonts[((a) * 44)+i]    |
                                   fonts[((a) * 44)+i+1])    >> (4 - sh);
               bmiddle2[pos++] |= (fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23]) >> (4 - sh);
            }
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            (*current)++;
            break;
         case 3 :
            pos = --(*current) * WIDTH ;
            bbelow1[pos]   |= (fonts[((a) * 44)])    >> 3 ;
            bbelow2[pos++] |= (fonts[((a) * 44)+22]) >> 3 ;
            for(i=0;i<21;i++) {
               bbelow1[pos]   |= (fonts[((a) * 44)+i] |
                                  fonts[((a) * 44)+i+1])    >> 3 ;
               bbelow2[pos++] |= (fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23])   >> 3 ;
            }
            bbelow1[pos]    = 0;    /* these four lines may be ignor */
            bbelow2[pos++]  = 0;
            bbelow1[pos]    = 0;
            bbelow2[pos++]  = 0;
            (*current)++;
            break;
       }  /* end switch */
}

void subscript_enlarge(int *ch, int *current, int u)
{
       int sh                ;
       int level             ;
       int i               ;
       int pos               ;
       unsigned char a = *ch ;
       u >>= 5               ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
         case 2 :
            pos = *current *  WIDTH ;
            for(i=0;i<22;i++) {
               bbelow1[pos]   = (fonts[((a) * 44)+i]    << 4)     ;
               bbelow2[pos++] = (fonts[((a) * 44)+i+22] << 4) | u ;
               bbelow1[pos]   = (fonts[((a) * 44)+i]    << 4)     ;
               bbelow2[pos++] = (fonts[((a) * 44)+i+22] << 4) | u ;
            }
            bbelow1[pos]   = (fonts[((a) * 44)+21]    << 4)     ;
            bbelow2[pos++] = (fonts[((a) * 44)+43] << 4) | u ;
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            (*current)++;
            (*current)++;
            break;
         case 1 :
            --(*current)   ;
            pos = --(*current) * WIDTH ;
            sh = ( (a>0xe7) & (a<0xed) & ((bmiddle1[pos+30]|bmiddle2[pos+30])!=0)) ? 2 : 0 ;
            for(i=0;i<22;i++) {
               bmiddle1[pos]   |= (fonts[((a) * 44)+i])    >> (4 - sh) ;
               bmiddle2[pos++] |= (fonts[((a) * 44)+i+22]) >> (4 - sh);
               bmiddle1[pos]   |= (fonts[((a) * 44)+i])    >> (4 - sh);
               bmiddle2[pos++] |= (fonts[((a) * 44)+i+22]) >> (4 - sh);
            }
            bmiddle1[pos]    |= (fonts[((a) * 44)+21])    >> (4 - sh);
            bmiddle2[pos++]  |= (fonts[((a) * 44)+43])    >> (4 - sh);
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            (*current)++;
            (*current)++;
            break;
         case 3 :
            --(*current)      ;
            pos = --(*current) * WIDTH ;
            for(i=0;i<22;i++) {
               bbelow1[pos]   |= (fonts[((a) * 44)+i])    >> 3 ;
               bbelow2[pos++] |= (fonts[((a) * 44)+i+22]) >> 3 ;
               bbelow1[pos]   |= (fonts[((a) * 44)+i])    >> 3 ;
               bbelow2[pos++] |= (fonts[((a) * 44)+i+22]) >> 3 ;
            }
            bbelow1[pos]   |= (fonts[((a) * 44)+21])    >> 3 ;
            bbelow2[pos++] |= (fonts[((a) * 44)+43])    >> 3 ;
            bbelow1[pos]    = 0;    /* these four lines may be ignor */
            bbelow2[pos++]  = 0;
            bbelow1[pos]    = 0;    /* these four lines may be ignor */
            bbelow2[pos++]  = 0;
            bbelow1[pos]    = 0;
            bbelow2[pos++]  = 0;
            (*current)++;
            (*current)++;
            break;
       }  /* end switch */
}

void subscript_bold_enlarge(int *ch, int *current, int u)
{
       int sh                ;
       int level             ;
       int i		               ;
       int pos               ;
       unsigned char a = *ch ;
       u >>= 5               ;
       level = islevel(ch)   ;        /* get its type of 3-level */
       switch (level) {
         case 2 :
            pos = *current *  WIDTH ;
            bbelow1[pos]   = (fonts[((a) * 44)]    << 4)     ;
            bbelow2[pos++] = (fonts[((a) * 44)+22] << 4) | u ;
            bbelow1[pos]   = (fonts[((a) * 44)]    << 4)     ;
            bbelow2[pos++] = (fonts[((a) * 44)+22] << 4) | u ;
            for(i=0;i<21;i++) {
               bbelow1[pos]   = ((fonts[((a) * 44)+i]   |
                                 fonts[((a) * 44)+i+1])   << 4)     ;
               bbelow2[pos++] = ((fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23]) << 4) | u ;
               bbelow1[pos]   = ((fonts[((a) * 44)+i]   |
                                 fonts[((a) * 44)+i+1])   << 4)     ;
               bbelow2[pos++] = ((fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23]) << 4) | u ;
            }
            bbelow1[pos]   = ((fonts[((a) * 44)+21]   |
                               fonts[((a) * 44)+20])   << 4)     ;
            bbelow2[pos++] = ((fonts[((a) * 44)+43] |
                               fonts[((a) * 44)+42])   << 4) | u ;
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            bbelow1[pos]    = 0    ;
            bbelow2[pos++]  = u    ;
            (*current)++;
            (*current)++;
            break;
         case 1 :
            --(*current);
            pos = --(*current) * WIDTH ;
            sh = ( (a>0xe7) & (a<0xed) & ((bmiddle1[pos+30]|bmiddle2[pos+30])!=0)) ? 2 : 0 ;
            bmiddle1[pos]   |= (fonts[((a) * 44)])    >> 4 ;
            bmiddle2[pos++] |= (fonts[((a) * 44)+22]) >> 4 ;
            bmiddle1[pos]   |= (fonts[((a) * 44)])    >> 4 ;
            bmiddle2[pos++] |= (fonts[((a) * 44)+22]) >> 4 ;
            for(i=0;i<21;i++) {
               bmiddle1[pos]   |= (fonts[((a) * 44)+i]    |
                                   fonts[((a) * 44)+i+1])    >> (4 - sh);
               bmiddle2[pos++] |= (fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23]) >> (4 - sh);
               bmiddle1[pos]   |= (fonts[((a) * 44)+i]    |
                                   fonts[((a) * 44)+i+1])    >> (4 - sh);
               bmiddle2[pos++] |= (fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23]) >> (4 - sh);
            }
            bmiddle1[pos]   |= (fonts[((a) * 44)+21] |
                               fonts[((a) * 44)+20])    >> (4 - sh);
            bmiddle2[pos++] |= (fonts[((a) * 44)+43] |
                               fonts[((a) * 44)+42])    >> (4 - sh);
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            bmiddle1[pos]    |= 0;
            bmiddle2[pos++]  |= 0;
            (*current)++;
            (*current)++;
            break;
         case 3 :
            --(*current)  ;
            pos = --(*current) * WIDTH ;
            bbelow1[pos]   |= (fonts[((a) * 44)])    >> 3 ;
            bbelow2[pos++] |= (fonts[((a) * 44)+22]) >> 3 ;
            bbelow1[pos]   |= (fonts[((a) * 44)])    >> 3 ;
            bbelow2[pos++] |= (fonts[((a) * 44)+22]) >> 3 ;
            for(i=0;i<21;i++) {
               bbelow1[pos]   |= (fonts[((a) * 44)+i] |
                                  fonts[((a) * 44)+i+1])    >> 3 ;
               bbelow2[pos++] |= (fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23])   >> 3 ;
               bbelow1[pos]   |= (fonts[((a) * 44)+i] |
                                  fonts[((a) * 44)+i+1])    >> 3 ;
               bbelow2[pos++] |= (fonts[((a) * 44)+i+22] |
                                  fonts[((a) * 44)+i+23])   >> 3 ;
            }
            bbelow1[pos]   |= (fonts[((a) * 44)+21] |
                               fonts[((a) * 44)+20])    >> 3 ;
            bbelow2[pos++] |= (fonts[((a) * 44)+43] |
                               fonts[((a) * 44)+42])   >> 3 ;
            bbelow1[pos]    = 0;    /* these four lines may be ignor */
            bbelow2[pos++]  = 0;
            bbelow1[pos]    = 0;
            bbelow2[pos++]  = 0;
            bbelow1[pos]    = 0;
            bbelow2[pos++]  = 0;
            (*current)++;
            (*current)++;
            break;
       }  /* end switch */
}
/* ---------------------------------- */
/* check if print code is what level, */
/* upper level then return 1          */
/* middle level "     "    2          */
/* below  level "     "    3          */
/* ---------------------------------- */
int islevel(int *line)
{
    register int index ;

    index  = *line ;
    if( *line < 0x00d0)
        return(2);
    else
        return(tablev[index - 0xd0]) ;
}
