#include "inc.h"

/*****************************************************************************/
/* find length of thai string                                                */
/* input                                                                     */
/*      thaist : thai string                                                 */
/* return : length of thai string                                            */
/* Updated: Use fast pointer instead of clumsy array                         */
/* By Suttipong Kanakakorn Mon  08-07-1989  00:40:51                         */
/*****************************************************************************/
int thaistrlen(char *thaist)
{
    register int count = 0;

    while (*thaist) {
        if (whatlevel(*thaist) == MIDDLE)
            count++;
        thaist++;
    }
    return(count);
}
