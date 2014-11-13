/****************************************************************************/
/*  tutil1.C 15 Fri  08-25-1989  21:41:55                                   */
/****************************************************************************/
#include "inc.h"
/*****************************************************************************/
/* find length of thai string                                                */
/* input                                                                     */
/*      thaist : thai string                                                 */
/* return : length of thai string                                            */
/*****************************************************************************/
unsigned thaistrlen(char thaist[])
{
    register unsigned index = 0,count = 0;
    char curfont = 0;
    while (thaist[index] != '\0') {
      if (whatlevel(thaist[index]) == MIDDLE) {
          count++;
          if ((curfont & ENLARGEATTR) == ENLARGEATTR) {
            count++;
          }
      }
      index++;
    }
    return(count);
}
