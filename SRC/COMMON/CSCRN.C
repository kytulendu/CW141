/* Common screen utility for both cuprint.exe and cw.exe */
/* Extract from ..\scuw\scrn.c by Suttipong Kanakakorn   */
/*              Sun  08-06-1989  09:35:17                */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include "cwtype.h"
#include "proto.h"
#include "cwgrphc.h"

int sign(int x)
{
    if (abs(x) != x) {
      return(-1);
    } else {
      if (x == 0)
        return(0);
      else
        return(1);
    }
}

void _line(int x1,int y1,int x2,int y2)
{
    int x,y;
    register int delx,dely;
    int s1,s2,change,weight,temp,i;

    x = x1;y = y1;
    delx = abs(x2-x1);
    dely = abs(y2-y1);
    s1 = sign(x2-x1);
    s2 = sign(y2-y1);
    if (dely > delx){
       temp = delx;
       delx = dely;
       dely = temp;
       change = 1;
    }
    else
       change = 0;
    weight = dely + dely - delx;
    for (i=1;i<=delx;i++) {
        plot(x,y);
        while (weight >= 0) {
              if (change == 1)
                 x += s1;
              else
                 y += s2;
              weight -= delx + delx;
        }
        if (change == 1)
           y += s2;
        else
           x += s1;
        weight += dely + dely;
    }
}

void box(int x1,int y1,int x2,int y2)
{
    _line(x1,y1,x1,y2);
    _line(x1,y1,x2,y1);
    _line(x2,y1,x2,y2);
    _line(x1,y2,x2,y2);
}

/*****************************************************************************/
/* display string on hercules graphic card                                   */
/* input                                                                     */
/*      st   : string to be displayed                                        */
/*      x    : vertical position ( 0 - 89 )                                  */
/*      y    : horisontal position ( 0 - 16 )                                */
/*      attr : attribute of string                                           */
/*****************************************************************************/
void dispstrhgc(char *st, unsigned x, unsigned y, font_attr attr)
{
    while ((*st != '\0') && (x < 90)) {
      if (*st < 32) {
        togglefont(&attr,*st);
      } else {
        if (whatlevel(*st) == MIDDLE) {
          prchar(*st,attr,x,y);
          if ((attr & ENLARGEATTR) == ENLARGEATTR) {
            x++;
          }
          x++;
        } else {
          if ((attr & ENLARGEATTR) == ENLARGEATTR) {
            if (x >= 2) {
              prchar(*st,attr,x-2,y);
            }
          } else {
            if (x >= 1) {
              prchar(*st,attr,x-1,y);
            }
          }
        }
      }
      st++;
    }
}

/****************************************************************************/
/* display string with printf capability                                    */
/* Written: Suttipong Kanakakorn                                            */
/*          Wed  08-02-1989  00:33:25                                       */
/* input                                                                    */
/*      x    : vertical position ( 0 - 89 )                                 */
/*      y    : horisontal position ( 0 - 16 )                               */
/*      attr : attribute of string                                          */
/*      st   : string to be displayed                                       */
/****************************************************************************/
void dispprintf(unsigned x, unsigned y, font_attr attr, char *format,...)
{
     va_list argptr;
     char tstring[240];

     va_start(argptr, format);
     vsprintf(tstring, format, argptr);
     va_end(argptr);
     dispstrhgc(tstring, x, y, attr);
}

/*****************************************************************************/
/* display blank                                                             */
/* input                                                                     */
/*      x     : vertical position ( 0 - 89 )                                 */
/*      y     : horisontal position ( 0 - 16 )                               */
/*      count : number of blank to be displayed                              */
/*      attr  : attribute of blank                                           */
/*****************************************************************************/
void dispblank(unsigned x, unsigned y, unsigned count, font_attr attr)
{
    count++;
    while (count--) {
      prchar(' ',attr,x++,y);
    }
}

/****************************************************************************/
/*  Set bit in attribute byte on/off from ascii control code given.         */
/****************************************************************************/
void togglefont(font_attr *curfont, font_code code)
{
    if (code == ONELINECODE)
      *curfont = *curfont ^ ONELINEATTR;
    if (code == SUPERCODE)
      *curfont = *curfont ^ SUPERATTR;
    if (code == SUBCODE)
      *curfont = *curfont ^ SUBATTR;
    if (code == ITALICCODE)
      *curfont = *curfont ^ ITALICATTR;
    if (code == BOLDCODE)
      *curfont = *curfont ^ BOLDATTR;
    if (code == TWOLINECODE)
      *curfont = *curfont ^ TWOLINEATTR;
    if (code == ENLARGECODE)
      *curfont = *curfont ^ ENLARGEATTR;
}

/*
Draw frame box on screen
Written : Suttipong Kanakakorn Fri  08-04-1989  23:39:16
*/
void framebox(unsigned x1, unsigned y1,
              unsigned x2, unsigned y2,
              unsigned attr)
{
    register unsigned i, j;

    i = x1;
    prchar(' ', attr, i++, y1);
    prchar('˜', attr ,i ,  y1);
    for (i++, j = x2 - 1; i < j; i++)
        prchar('•', attr, i, y1);
    prchar('™', attr, i++, y1);
    prchar(' ', attr, i,   y1);

    for (j = y1+1; j < y2; j++) {
        for (i=x1; i <= x2; i++)
            prchar(' ', attr, i, j);
        prchar('–', attr, x2-1, j);
        prchar('–', attr, x1+1, j);
    }
    i = x1;
    prchar(' ', attr, i++, y2);
    prchar('š' , attr,i , y2);
    for (i++, j = x2 - 1; i < j; i++)
        prchar('•', attr, i, y2);
    prchar('›', attr, i++, y2);
    prchar(' ', attr, i,   y2);
}

void blockmsg(int y)
{
    framebox(13, y-1, 13+53, y+1, REVERSEATTR);
}

void showerrno(void)
{
    errorsound();
    blockmsg(10);
    switch(errno) {
    case ENOENT  :  dispstrhgc("ËÒá¿éÁ¢éÍÁÙÅäÁè¾º ! ¡´»ØèÁã´æà¾×èÍ·Ó§Ò¹µèÍ..."
                               ,21, 10, REVERSEATTR);
                    break;
    case ENOMEM  :  dispstrhgc("Ë¹èÇÂ¤ÇÒÁ¨ÓäÁè¾Í ! ¡´»ØèÁã´æà¾×èÍ·Ó§Ò¹µèÍ..."
                               ,23, 10, REVERSEATTR);
                    break;
        default  :  framebox(13,9,66,12,REVERSEATTR);
                    dispstrhgc("à¡Ô´¤ÇÒÁ¼Ô´¾ÅÒ´ÀÒÂã¹ÃÐºº ¡ÃØ³Òá¨é§ä»·Õè ¨ØÌÒÏ"
                               ,23, 10, REVERSEATTR);
                    dispstrhgc(sys_errlist[errno],23 ,11, REVERSEATTR);
    }
    ebioskey(0);
}

/****************************************************************************/
/*  Save content of screen. Return pointer to content of screen.            */
/****************************************************************************/
char *savescrn(int x1,int y1,int x2,int y2)
{
    char *scrnindex;
    scrnindex = (char *) malloc((x2-x1+1) * (y2-y1+1) * 20);
    if (scrnindex == NULL) {
      savepic();
      errorsound();
      blockmsg(10);
      dispstrhgc("Ë¹èÇÂ¤ÇÒÁ¨ÓäÁè¾Í ! ¡´»ØèÁã´æà¾×èÍ·Ó§Ò¹µèÍ...",23,10,REVERSEATTR);
      ebioskey(0);
      return(NULL);
    }

    (*getwind_ptr)(x1,y1,(y2-y1+1)*20,(x2-x1+1),scrnindex);
    return(scrnindex);
}

/****************************************************************************/
/*  Restore content of screen.                                              */
/****************************************************************************/
void resscrn(char *scrnindex,int x1,int y1,int x2,int y2)
{
    if (scrnindex == NULL) {
      retpic();
      return;
    }
    (*putwind_ptr)(x1,y1,(y2-y1+1)*20,(x2-x1+1),scrnindex);
    free(scrnindex);
}