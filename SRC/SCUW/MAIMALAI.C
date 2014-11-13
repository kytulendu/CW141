#include "Thai.h"
#include "Global.h"
#include "Routine.h"
#include "TCtype.h"

/* ------------ MaiMalai Rtn ------------ */
/*  Programmer  : Subun Yenjaichon
    Date Written    : Thursday, 12 May, 1988 8:54:52 AM
    Last Updated    : Saturday, 14 May, 1988 12:08:05 PM
*/

unsigned char *MaiMalaiRtn(TempIndx)
register unsigned char *TempIndx;
{
    register short      i;
/*  short   MaiMalaiExNum = 2;  */  /* 3-1 */
    short   MaiMalaiEx2Num = 6; /* 7-1 */
    register unsigned char *TempIndxp1 = TempIndx+1;
/*  static unsigned char *MaiMalaiExWord[] = {"‰ª√…≥’¬","‰ø≈∑Ï","‰ª√∑Ï"};*/
    static unsigned char *MaiMalaiExWord2[] = {"‰À¡","‰À¡È","‰Àπ",
                                        "‰∂ß","‰º∑","‰Õ»","‰©π"};

/* TAIL CUT RTN. by rule  ‰c/c  */
    if (TempIndxp1 <= RightMargin && istcon(*(TempIndx+2)))
    {
        if (*(TempIndx+3) == Karan )
            if(TempIndx+3 <= RightMargin)
                return(TempIndx+3);
            else
                return(MaiMalaiFC(TempIndx));
/*
        for(i = 0;i <= MaiMalaiExNum ;++i)
        {   if(!(nstrcmp(MaiMalaiExWord[i],TempIndx)))
                return(MaiMalaiFC(TempIndx));
        }
*/
        if (findchar(*(TempIndx+2),"¬√≈«"))
            return(MaiMalaiFC(TempIndx));
        else
        {
            for (i =0; i <= MaiMalaiEx2Num; ++i)
            {   if(!(nstrcmp(MaiMalaiExWord2[i],TempIndx))) /* if found do */
                    return(TempIndx-1); /* cut before MaiMalai */
            }
            return(TempIndxp1); /* cut after following-consonant */
        }
    }
    else
        if (isttnl(*(TempIndx+2)) && TempIndx+2 <= RightMargin)
            return(TempIndx+2);     /* cut after tonal */

    return(MaiMalaiFC(TempIndx));
} /* end of MaiMalai Rtn */

unsigned char *MaiMalaiFC(TempIndx)
unsigned char *TempIndx;
{
    register short      i;
    register unsigned char *TempIndxm1 = TempIndx-1;
    short SoreSeoMMLExNum = 5;  /* 6-1 */
    static unsigned char ShoreChangEx[]= "™‰¡æ√";
    static unsigned char *SoreSeoMMLExTable[] = {" ‰µ√Í§Ï"," ‰ª√∑Ï"," ‰≈´Ï"," ‰∫"
                                            ," ‰≈¥Ï"," ‰µ≈Ï"};

    switch (*(TempIndxm1))
    {
        case SoreSeo:
            for (i=0; i<= SoreSeoMMLExNum; ++i)
                if (!(nstrcmp(SoreSeoMMLExTable[i],TempIndxm1)))
                    return(TempIndx-2);  /* if same cut before consonant */
            return(TempIndxm1); /* cut before MaiMalai */
        case ShoreChang:
            if (nstrcmp(ShoreChangEx,TempIndxm1))
                return(TempIndxm1); /* cut before MaiMalai */
            else
                return(TempIndx-2); /* cut before leading-consonant */
        /* case MoreMar is Optional: ¡‰À»«√√…Ï ¡‰À»Ÿ√¬Ï */
        default:
            return(TempIndxm1); /* cut before MaiMalai */
    } /* end switch */
} /* end of MaiMalaiFC Rtn */