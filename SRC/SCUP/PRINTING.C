/* --------------------------------- */
/* Program PRINTING.C                */
/* Function : actual print data      */
/* DEVISE FROM PMENU.C               */
/* DEV DATE11-11-88                  */
/* Update  15-11-88                  */
/* Update  08-04-89                  */
/* Update  Suttipong Kanakakorn      */
/* Mon  08-07-1989  02:23:55         */
/* Add function prototype            */
/* Tue  08-15-1989  00:29:02         */
/* make printer font loading better  */
/* --------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include <ctype.h>

#include "inc.h"
#include "global.ext"
#include "24pins.h"

/* local function prototype use internal module only */
void seekbeginline(void);
void strfilter(char *s, char cutout);
void printtitle(char str[], int loc);
void pagewait(int i);
void linewait(int *printquit);
void contostd(char *t);
void marginset(char *t);
void preprinterloadline(char s[]);
char *adjust(char c[], int k);
void clear_message(void);

/* Modified: Suttipong Kanakakorn Tue  08-15-1989  00:28:34 */
void initializebufferandfont(void)
{
    extern int printer24pin;
    extern int printer24pininit;
    extern int printer9pininit;

    if (printer24pin==YES) {
       if (printer24pininit==NO) {
          AllocateBuffer9pin();
          cp_init_textbuf();
          cp_init_grpbuf();
          cp_lqnorm   = cp_loadfont ("NORMAL.P24", CP_LQ_TABSIZE);
          cp_lqitalic = cp_loadfont ("ITALIC.P24", CP_LQ_TABSIZE);
          cp_lqscr    = cp_loadfont ("NORMALS.P24", CP_LQSCR_TABSIZE);
          cp_lqscr_italic = cp_loadfont ("ITALICS.P24", CP_LQSCR_TABSIZE);
          cp_lqnorm_comb = cp_create_lqcombine ( cp_lqnorm);
          cp_lqitalic_comb = cp_create_lqcombine ( cp_lqitalic);
          cp_lqscr_comb = cp_create_lqscrcombine ( cp_lqscr);
          cp_lqscr_italic_comb = cp_create_lqscrcombine ( cp_lqscr_italic);
          printer24pininit=YES;
       }
    }  else {
       if (printer9pininit==NO) {
          AllocateBuffer9pin();
          PrinterLoadFont9pin();
          printer9pininit=YES;
       }
    }
}

/* modify by Suttipong Kanakakorn Tue  08-29-1989  23:53:53 */
void printing(void)
{
    extern int blankskip();
    extern int pic_print;
    int i,j   ;
    int printquit;
    char st[5] ;
    char s[500] ;
    char p[500] ;
    char scol   ;
    if (graphicprint==YES)        /* if print in graphic mode */
       initializebufferandfont(); /* allocate graphic buffer and load font */
    scol=7;
    dispstrhgc("กำลังพิมพ์ COPYที่ :     ",10-scol,12,0);
    dispstrhgc("  หน้าที่ :     ",30-scol,12,0);
    dispstrhgc("  บรรทัดที่ :     ",43-scol,12,0);
    dispstrhgc("  หยุดชั่วคราว <P> เลิกพิมพ์ <Q>",58-scol,12,BOLDATTR);
    printquit = NO;
    PrinterInitialize();                     /* initialize printer */
    PrinterMasterSelectMode(0);              /*  Set to 10 Cpi */
    for (i=1; i<=copytoprint; i++) {
        linespace = find_line_space();
        PrinterSetFormLenghtInch(pagelength);
        seekbeginline();
        /*
        itoa(i,st,10);
        dispstrhgc("     ",25 - scol,12,0);
        dispstrhgc(st,30 - strlen(st) - scol,12,2);
        */
        dispprintf(25 - scol, 12 ,2, "%5d", i);
        curpage = pagenumberoffset;
        /*
        itoa(curpage,st,10);
        dispstrhgc("     ",38 - scol ,12,0);
        dispstrhgc(st,43 - strlen(st) - scol ,12,2);
        */
        dispprintf(38 - scol, 12, 0, "%5d", curpage);
        curline = 0;
        pic_print = NO;
        while ((fgets(s,500,fp) != NULL) && (printquit==NO)) {
            strfilter(s,0x8d);         /* delete 0x8d */
            strcpy(p,s);
            j=0;
            while ((p[j]<32) && (p[j]!=NULL)) j++;
            if (p[j] == '.') {    /* -to allow free format.       */
                dotcommand(&p[j]); /* do dotcommand then read next */
                if (newpage == YES) {  /* page break dotcmd (.pa ,.cp) found */
                    newpage = NO;      /* reset pagebreak status */
                    /*
                    PrinterSkipLine(((29+1)-(locpagetitle!=0)-(locheading!=0)
                                   -(locfooting!=0)-curline)*2 );
                    */
                    skip_line((userlineperpage+1) - curline);
                    curline = 0   ;
                    printtitle(footing,locfooting);
                    PrinterFormFeed();
                    if (curpage >= pageend-pagebegin+pagenumberoffset) break;
                    pagewait(0);  /* finish each page */
                    curpage++     ;
                    /*
                    itoa(curpage,st,10);
                    dispstrhgc("     ",38 - scol ,12,0);
                    dispstrhgc(st,43 - strlen(st) - scol ,12,2);
                    */
                    dispprintf(38 - scol, 12, 0, "%5d", curpage);
                }
	    } else {  /* Not dot commands */
                if (mailmergeflag == YES) {
                    mailmerge(s,p);
                    strcpy(s,p);
                }
                if (stdcode == NO) {
                    contostd(s);   /* convert from ku to so-mo-or */
                }
                marginset(s);      /* set left-right margin */
                if  (curline == 0)  {
                    printtitle(heading,locheading);
                    sprintf(p,pageformat,curpage);
                    printtitle(p,locpagetitle);
                    curline++;     /* begin curline = 1  */
                }
                /*
                itoa(curline,st,10);
                dispstrhgc("     ",53 - scol ,12,0);
                dispstrhgc(st,58 - strlen(st) - scol ,12,2);
                dispstrhgc(blankline,2,13,0);
                */
                dispprintf(53 - scol, 12, 0, "%5d", curline);
		dispprintf(2, 13, NORMALATTR, "%77s", " ");  /* Clear Line */
		dispprintf(2, 13, 0, "%-77.77s", s);         /* Disp CurLine */
/*
                dispstrhgc(extbarprinting ? "*":"-",1,1,REVERSEATTR);
*/
                preprinterloadline(s);
                curline++;
                if (curline > userlineperpage) {
                    curline = 0   ;
                    printtitle(footing,locfooting);
                    PrinterFormFeed();
                    if (curpage >= pageend-pagebegin+pagenumberoffset)
                        break;
                    pagewait(0);  /* finish each page */
                    curpage++     ;
                    itoa(curpage,st,10);
                    dispstrhgc("     ",38 - scol ,12,0);
                    dispstrhgc(st,43 - strlen(st) - scol ,12,2);
                }
                linewait(&printquit);
            }  /* end case of non-dotcommand  */
        }     /* end while */
        if (curline != 0) {
            /*
            PrinterSkipLine(((29+1)-(locpagetitle!=0)-(locheading!=0)
                          -(locfooting!=0)-curline) *2 ); /* skip to bottom */
            */
            skip_line((userlineperpage+1) - curline);
            printtitle(footing,locfooting);
            PrinterFormFeed();
        }
        fseek(fp,0L,SEEK_SET);  /* rewind file pointer */
        if (printquit==YES) break; /* exit for loop */
        pagewait(1);  /* finish each copy */
    }
    dispprintf(2,12,0,"%77s", " "); /* clear massage */
    fclose(mfp);           /* close merge file */
    mailmergeflag=NO;      /* each field content ready flag Reset  */
    mergefileexist=NO;     /* .df successful           flag Reset  */
    fieldnameexist=NO;     /* .rv successful           flag Reset  */
}

void marginset(char t[])
{
     int  cw = 1;  /* 2 for enlarge attr, 1 for another */
     char p[500];
     int left,i,k;
     k = (leftmargin==1) ? 0 : leftmargin; /* user remind start at 1 not 0 */
     left = k;                       /* k is pointer , left is counter */
     for(i=0;i<500;i++) p[i] = ' ';  /* set all blank  */
     i = 0;
     while (t[i] != '\0') {
        if (t[i] < 0x20){
           if(t[i] == ENLARGE) cw = (cw == 2) ? 1 : 2;
        }
        else{
            if (whatlevel(t[i])==0){
               left += cw; /* increment if normal level */
               if (left > rightmargin)
                  break;
            }
        }
        p[k++] = t[i++] ;
     }
     p[k] = '\0' ;
     strcpy(t,p);
}

/* modified by Suttipong Kanakakorn */
void contostd(char *t)
{
     while (*t) {
        *t = kutostd(*t) ;
        t++;
     }
}

/* modified by Suttipong Kanakakorn */
void strfilter(char *s, char cutout)
{
     char *r;

     r = s;
     while (*s) {
        if (*s != cutout)
            *r++ = *s;
        s++;
     }
     *r = '\0';
     /*
     int i,j;
     i=j=0;
     while (s[i] != '\0') {
        if (s[i] != cutout)
        s[j++]=s[i];
        i++;
     }
     s[j]=s[i];
     */
}

void seekbeginline(void)
{
      int cdtpgbrk;
      int pagecount = 1;
      int linecount = 0;
      char s[500];
      char temp[500];
      if (pagebegin > 1)
         while (fgets(s,500,fp) != NULL) {
               if (s[0]=='.'){   /* dot command */
                  if(tolower(s[1])=='p'&&tolower(s[2]=='a')){ /* page break */
                    pagecount++;
                    linecount=0;
                  }
                  else{
                      if(tolower(s[1])=='c'&&tolower(s[2]=='p')){/*cond pg-brk*/
                        strcpy(temp,&s[3]);
                        strcpy(temp,&temp[blankskip(temp)]);
                        temp[strlen(temp)-1]='\0';
                        cdtpgbrk = (atoi(temp)>userlineperpage) ?
                                   userlineperpage : atoi(temp);
                         if(linecount + cdtpgbrk > userlineperpage){
                           pagecount++;
                           linecount=0;
                         }
                       }
                  }
               }
               else{
                   linecount++;
                   if(linecount==userlineperpage){
                     pagecount++;
                     linecount=0;
                   }
               }
               if(pagecount>=pagebegin) break;
         }
}

/* modify by Suttipong Kanakakorn Wed  08-30-1989  00:32:38 */
void printtitle(char str[], int loc)
{
     int  newloc;
     char s[500] ;
     if (loc != 0) {
         switch (loc) {
         case 4 :                /* odd <-> right  even <-> left  */
                newloc = (curpage % 2 == 0) ? 1 : 3 ;
                break;
         case 5 :                /* odd <-> left   even <-> right */
                newloc = (curpage % 2 == 0) ? 3 : 1 ;
                break;
         default:
                newloc = loc;
                break;
         }
         strcpy(s,adjust(str,newloc)) ;/* adjust location LEFT,RIGHT,MIDDLE */
         dispprintf(2,13,2,"%-77.77s", s);
         preprinterloadline(s);
     }
}

char *setpageformat(char form[], int max)
{
  char s[500] ;
  int i,j ;
  i =0 ;
  j =0 ;
  while (max-- > 0) {
        if (form[i] == '\0') {
           s[j++] = ' ' ;
           s[j++] = '%' ;
           s[j++] = 'd' ;
           s[j] = '\0' ;
           return(s) ;
        }
        else {
             if (form[i] == '%') {
                s[j++] = form[i++] ;
                s[j++] = 'd' ;
                while (form[i] != '\0' )  {
                      if (form[i] == '%' ) {
                         s[j++] = ' ' ;  /* ignore the %  */
                         i++          ;
                      }
                      else
                         s[j++] = form[i++] ;
                }
                s[j]   = '\0' ;
                return(s)     ;
             }
             else
                s[j++] = form[i++] ;
        }
  }
  s[j]   = '\0' ;
  return(s)     ;
}

char *adjust(char c[], int k)
{
   int maxpaper;
   int len,i,j   ;
   char s[500]  ;
   maxpaper = (smallpaper) ? 80 : 136;
   i=0;
   len = 0;       /* find actual thai string length */
   while(c[i] != '\0') len=(whatlevel(c[i++])==0) ? len+1 : len;
   for(i=0;i<maxpaper;i++) s[i] = ' ' ;
   s[maxpaper] = '\0' ;
   switch (k) {
          case (1) :                  /* left   justify */
               return(c);
          case (2) :                  /* center justify */
               j=(maxpaper-1-len)/2 ;
               i=0 ;
               while (c[i] !='\0')
                    s[j++] = c[i++] ;
               s[j]='\0';
               return(s);
          case (3) :                  /* right  justify */
               j=(maxpaper-1-len);
               i=0 ;
               while (c[i] !='\0')
                     s[j++] = c[i++] ;
               s[j]='\0';
               return(s);
          default:
               return(c);
   }
}

/* modify by Suttipong Kanakakorn Mon  08-28-1989  23:36:26 */
void pagewait(register int i)
{
    if (pagebreak == YES) {
        if (i == 0 || i == 1) {
            clear_message();
            if (i == 0)
                dispstrhgc("กดปุ่มใด ๆ เมื่อหน้ากระดาษพร้อม ...",30,13,2);
            else
                dispstrhgc("สิ้นสุดการพิมพ์แต่ละชุด กดปุ่มใด ๆ ...",30,13,2);
            while (!keypressed());
            while (keypressed())  ebioskey(0) ;  /* clear KBD buffer */
            clear_message();
        }
    }
}

/* modify by Suttipong Kanakakorn Mon  08-28-1989  23:36:41 */
void linewait(int *printquit)
{
     char i;
     if (keypressed()) {
        while (keypressed()) {
           i = tolower(ebioskey(0) & 0x00ff);
           switch (i) {
           case 'p':
              while (keypressed())  ebioskey(0) ;  /* clear KBD buffer */
              clear_message();
              dispstrhgc("กดปุ่ม < P > เพื่อพิมพ์ต่อ, < Q > เลิกพิมพ์...",
                20,13,REVERSEATTR);
              do {
                i = tolower(ebioskey(0) & 0x00ff);
              } while (i!='p' && i!='q');
              clear_message();
              if(i=='q')
                *printquit=YES;
              break;
           case 'q':
              *printquit=YES;                        /* quit printing */
              while (keypressed())  ebioskey(0) ;  /* clear KBD buffer */
              break;
           }
        }
     }
}

/*
void findthreeindex(char s[], int *uindex, int *mindex, int *lindex)
{
 int i,j,cadjust;
 cadjust=1;
 i=*uindex=*mindex=*lindex=0;
 while (s[i] != '\0') {
    if (s[i] < ' ') {
       if (s[i] == ENLARGE) {     /* if enlarge col to add = 2 */
          if (cadjust==1)
             cadjust=2;
          else
             cadjust=1;
       }
    }  else {
       j=s[i];
       switch (islevel(&j)) {
          case 1:
               *uindex=*mindex;
               break;
          case 2:
               *mindex+=cadjust;
               break;
          case 3:
               *lindex=*mindex;
               break;
       }
    }
    i++;
 }
}
*/

void preprinterloadline(char s[])
{
  extern int pic_print;
  extern int printer24pin;
  int    mindex;
  if (graphicprint==YES) {
     if (printer24pin==YES) {
/*        findthreeindex(s,&uindex,&mindex,&lindex); */
        cp_clearbuf();
        cp_split4level(s);
        mindex=cp_printlq();
        PrintThree24pin(mindex,mindex,mindex);
     }
     else
        PrinterLoadLine9pin(s);
  }
  else {
     if (pic_print==NO)
        PrinterLoadLineText(s);
     else {
        if (printer24pin==YES) {
/*           findthreeindex(s,&uindex,&mindex,&lindex); */
           cp_clearbuf();
           cp_split4level(s);
           mindex=cp_printlq();
           PrintThree24pin(mindex,mindex,mindex);
        }
        else
           PrinterLoadLine9pin(s);
     }
  }
}

/* Written by Suttipong Kanakakorn Wed  08-30-1989  00:40:21 */
void clear_message(void)
{
    dispprintf(2, 13, 0, "%77s", " ");
}