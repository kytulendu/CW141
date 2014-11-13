/****************************************************************************/
/*  tutil1.C -> Thai utility 1 , cut from str.c                             */
/*  updated: Suttipong Kanakakorn                                           */
/*          Wed  08-02-1989  09:52:44                                       */
/*          Sun  08-06-1989  11:24:52                                       */
/****************************************************************************/
#include "inc.h"

/*****************************************************************************/
/* find length of thai string                                                */
/* input                                                                     */
/*      thaist : thai string                                                 */
/* return : length of thai string                                            */
/* Modify : Suttipong  Kanakakakorn                                          */
/*          Mon  07-31-1989  08:32:36   use only pointer no index            */
/*****************************************************************************/
unsigned thaistrlen(register unsigned char *thaist)
{
    register int count = 0;
    font_attr curfont = 0;

    while (*thaist) {
      if (whatlevel(*thaist) == MIDDLE) {
        if (*thaist < 32) {
           togglefont(&curfont,*thaist);
        } else {
          count++;
          if ((curfont & ENLARGEATTR) == ENLARGEATTR) {
            count++;
          }
        }
      }
      thaist++;
    }
    return(count);
}

/****************************************************************************/
/*  Find column of string from column given in Thai,ignore level of Thai    */
/*  character & attribute code of font. Column is origin 0. Find current    */
/*  attribute of font of string via *font too.                              */
/****************************************************************************/
unsigned linearcolumn(char *textstream, unsigned thaicol, font_attr *font)
{
    register unsigned logiccol = 0;  /* origin 0 */
    *font = 0;
    while ((thaicol > 0) && (*textstream != '\0')) {
      if (whatlevel(*textstream) == MIDDLE) {
        if (*textstream < 32) {
          togglefont(font,*textstream);
        } else {
          if ((*font & ENLARGEATTR) == ENLARGEATTR) thaicol--;
          if (thaicol > 0) thaicol--;
        }
      }
      textstream++;
      logiccol++;
    }
    if (*textstream != MIDDLE) {
      while (whatlevel(*(textstream++)) != MIDDLE)
        logiccol++;
    }
    return(logiccol);
}
