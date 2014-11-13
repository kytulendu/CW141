#include "TCtype.h"
#include "Thai.h"
#include "Global.h"
#include "Routine.h"

/* ------------ LeadCon Rtn ------------ */
/*  Programmer  : Subun Yenjaichon
    Date Written    : Friday, 13 May, 1988 10:47:33 AM
    Last Updated    : Friday, 13 May, 1988 10:47:51 AM
*/

unsigned char *LeadConRtn(TempIndx)
register unsigned char *TempIndx;
{
    unsigned char *TempIndxm1 = TempIndx -1;

    if (istlvwl(*(TempIndxm1)))
        return(TempIndx-2); /* cut before leading vowel */
    else
        return(TempIndxm1); /* cut before leading Consonant */
}/* end of LeadConRtn() */