/****************************************************************************/
/*  SCRN.C 15 JAN 89                                                        */
/*  Updated: Suttipong Kanakakorn                                           */
/*           Sun  08-06-1989  09:31:59                                      */
/****************************************************************************/
#include "inc.h"
#include "..\common\cwgrphc.h"

void cls()
{
    clsgraph(wind.col,wind.row,wind.col+wind.length-1,wind.row+wind.width-1);
}
