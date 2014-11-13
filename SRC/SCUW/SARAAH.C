#include "Thai.h"
#include "Global.h"
#include "TCtype.h"

unsigned char *SaraAhRtn(TempIndx)
    register unsigned char *TempIndx;
{
    unsigned char *TempIndxm2 = TempIndx -2;

        if (*(TempIndx+1) == HorHeeb && *(TempIndx + 2) == Karan) /* กรณีนี้เป็น ห์ เช่น เคราะห์ */
            {return((TempIndx + 2 <= RightMargin) ? (TempIndx + 2) : FAIL);}
        else
            {if (TempIndx <= RightMargin)
                return(TempIndx);   /* กรณีทั่ว ๆ ไป เช่น จะ, พระ   */
else
        if (istcon(*TempIndxm2))
            switch (*(TempIndx-1))
            {
                case NoreNoo:
                    if (*TempIndxm2 != ShoreChang)
                        return(TempIndxm2); /* cut before NoreNue */
                    break;
                case RoreReo:
                    if (!findchar(*TempIndxm2,"กขตทปพสห"))
                        return(TempIndxm2);
                    break;
                case WoreWaan:
                    if (!findchar(*TempIndxm2,"สห"))
                        return(TempIndxm2);
                    break;
                case LoreLing:
                    if (!findchar(*TempIndxm2,"คผสห"))
                        return(TempIndxm2);
                    break;
                case NoreNane:
                    if (!findchar(*TempIndxm2,"ขคษ"))
                        return(TempIndxm2);
                    break;
                default:
                    return(TempIndxm2);
            }   /* end switch */
        return(FAIL);}
} /* end saraAh Rtn */