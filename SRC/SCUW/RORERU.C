#include "Thai.h"
#include "Global.h"
#include "Routine.h"

/* ------------ RoreRu  Rtn ------------ */
/*  Programmer  : Subun Yenjaichon
    Date Written    : Monday, 16 May, 1988 10:55:24 AM
    Last Updated    : Monday, 16 May, 1988 10:55:48 AM
*/

unsigned char *RoreRuRtn(TempIndx)
unsigned char *TempIndx;
{
    unsigned char *TempIndxm1 = TempIndx -1;
    short   i;
    short   RoreRuWdCnt = 7; /* 8-1 */
    static unsigned char *RoreRuWd[] = {"Ä¡Éì","Ä´Ù","Ä´Õ","ÄªÒ","Ä¤àÇ·","Ä·ÑÂ","ÄÉÕ","Ä·¸Ôì"
                                ,"Ä·¸Õ"};

    if(!(findchar(*TempIndxm1,"¡¤µ·¹»¾ÁÈÊ")))
        return(TempIndxm1); /* cut before RoreRu */
    else
    {   /*  possible leading consonants is found  do */
        if (*TempIndxm1 == PoreParn && !(nstrcmp("¾Ä¡Éì",TempIndxm1)))
            return(TempIndx -2); /* cut before ¾Ä¡Éì */

        for (i=0; i<= RoreRuWdCnt; ++i)
            if(!(nstrcmp(RoreRuWd[i],TempIndx)))
                return(TempIndxm1); /* if found,  cut before RoreRu */
        return(TempIndx - 2); /* cut before possible leading consonants */
    }
} /* end RoreRuRtn() */