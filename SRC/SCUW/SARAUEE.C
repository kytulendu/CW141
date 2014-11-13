#include "Thai.h"
#include "Global.h"
#include "TCtype.h"
#include "Routine.h"

/*  ----------------------------
        Written by      Tanat Chirakarn
        Modified by     Subun Yenjaichon
        Date Written          7 May, 1988
        Date Modified       23 May, 1988
        ---------------------------- */

unsigned char *Ueer1(), *Ueer2(), *Ueer3(), *Ueer4();
unsigned char *Ueer5(), *Ueer6();
unsigned char *SaraUeeFC();

unsigned char *SaraUeeRtn(TempIndx)
    register unsigned char *TempIndx;
{
/* if (findchar(*(TempIndx + 1), "¨±¿À"))
        return(FAIL); */    /* corrected by Subun */

    if (isttnl(*(TempIndx + 1)))
        {
            if (*(TempIndx + 2) != OrAng)
            {   if (TempIndx + 2 <= RightMargin)
                    return(TempIndx+2);
/*              return((TempIndx + 2 <= RightMargin) ? (TempIndx + 2) : FAIL); */
            }
            else if (*(TempIndx - 3) == SaraA)  /* *(TempIndx+2) == OrAng */
                switch(*(TempIndx + 1))
                    {
                        case MaiEk:     return(Ueer1(TempIndx)); /* ‡--◊ËÕ */
                        case MaiToe:    return(Ueer2(TempIndx)); /* ‡--◊ÈÕ */
                        default:        return(TempIndx - 4);
                    }
            else if (*(TempIndx - 2) == SaraA)
                switch(*(TempIndx + 1))
                    {
                        case MaiEk:     return(Ueer3(TempIndx)); /* ‡-◊ËÕ */
                        case MaiToe:    return(Ueer4(TempIndx)); /* ‡-◊ÈÕ */
                        default:        return(TempIndx - 3);
                    }
           if(TempIndx + 2 <= RightMargin)
              return(TempIndx + 2);
            else
                return(SaraUeeFC(TempIndx));
/*              return((TempIndx + 2 <= RightMargin) ? TempIndx + 2 : FAIL); */
        }
    else
    if (*(TempIndx + 1) == OrAng)
        {
            if (*(TempIndx - 3) == SaraA)
                return (Ueer5(TempIndx));   /* ‡--◊Õ */
            else if  (*(TempIndx - 2) == SaraA)
                return (Ueer6(TempIndx));   /* ‡-◊Õ */
            else
                if(TempIndx + 1 <= RightMargin)
                    return(TempIndx + 1);
/*              return((TempIndx + 1 <= RightMargin) ? (TempIndx + 1) : FAIL);  */
        }
    if (TempIndx + 1 <= RightMargin)
            return(TempIndx + 1);
    else
            return(SaraUeeFC(TempIndx));

/*      return((TempIndx + 1 <= RightMargin) ? (TempIndx + 1) : FAIL);  */
}/* end of saraUeeRtn() */

unsigned char *SaraUeeFC(TempIndx)
unsigned char *TempIndx;
{
        unsigned char *TempIndxm2 = TempIndx-2;

        switch (*(TempIndx-1))
        {
            case NoreNoo:
            case MoreMar:
            case WoreWaan:
                if (*TempIndxm2 != HorHeeb)
                    return(TempIndxm2); /* cut before NoreNoo */
                else
                    return(TempIndx-3); /* cut before HorHeeb */
                break;
            case RoreReo:
                if (!findchar(*TempIndxm2,"§ªÀ"))
                    return(TempIndxm2);
                break;
            case LoreLing:
                if (!findchar(*TempIndxm2,"°§ªÀ"))
                    return(TempIndxm2);
                break;
            default:
                return(TempIndxm2);
        }   /* end switch */
        return(FAIL);
}   /* end of SaraUeeFC() */

unsigned char *Ueer1(TempIndx) /* ‡--◊ËÕ */
    unsigned char *TempIndx;
{
    if (TempIndx + 2 <= RightMargin &&
        (!findchar(*(TempIndx + 3), "ßπ¡¬") ||
        !nstrcmp("æ√", TempIndx - 2) ||
        !nstrcmp("Àß", TempIndx - 2) ||
        !nstrcmp("À¬", TempIndx - 2)))
            return(TempIndx + 2);
    else if (TempIndx + 3 <= RightMargin)
        return(TempIndx + 3);
    return(TempIndx - 4);
}

unsigned char *Ueer2(TempIndx) /* ‡--◊ÈÕ */
    unsigned char *TempIndx;
{
    if (TempIndx + 2 <= RightMargin)
        if (!nstrcmp("°≈", TempIndx - 2))
            {
                if (*(TempIndx + 3) != NoreNoo)
                    return(TempIndx + 2);
            }
        else
            if (TempIndx + 3 <= RightMargin)
                return(TempIndx + 3);
    return(TempIndx - 4);
}

unsigned char *Ueer3(TempIndx) /* ‡-◊ËÕ */
    unsigned char *TempIndx;
{
        if (findchar(*(TempIndx - 1), "™¥∫ºæø¡¬√ "))
            if (findchar(*(TempIndx + 3), "ßπ¡¬"))
                return(TempIndx - 3);
            else
                return((TempIndx + 2 <= RightMargin)?TempIndx+2 : TempIndx -3);
        else
            return((TempIndx + 3 <= RightMargin)? TempIndx+3 : TempIndx -3);
}

unsigned char *Ueer4(TempIndx) /* ‡-◊ÈÕ */
    unsigned char *TempIndx;
{
    if (TempIndx + 2 <= RightMargin)
        if (findchar(*(TempIndx + 3), "ßπ¡¬"))
            return(TempIndx - 3);
        else
            return(TempIndx + 2);
    return(TempIndx - 3);
}

unsigned char *Ueer5(TempIndx) /* ‡--◊Õ */
    unsigned char *TempIndx;
{
    if (TempIndx + 2 <= RightMargin &&
        findchar(*(TempIndx + 2), "°ßπ∫¡¬") &&
        nstrcmp("°≈", TempIndx - 2) &&
        nstrcmp("§√", TempIndx - 2) &&
        nstrcmp("Àπ", TempIndx - 2) &&
        nstrcmp("À≈", TempIndx - 2))
            return(TempIndx + 2);
    return(TempIndx - 4);
}

unsigned char *Ueer6(TempIndx) /* ‡-◊Õ */
    unsigned char *TempIndx;
{
    if (TempIndx + 2 <= RightMargin &&
        !findchar(*(TempIndx - 1), "¢®∂∫ø¬√ ") &&
        findchar(*(TempIndx + 2), "°ß¥π∫¡¬"))
            return(TempIndx + 2);
    return(TempIndx - 3);
}