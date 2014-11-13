#include "Thai.h"
#include "Global.h"
#include "TCtype.h"


/*------------------------------------------------------------------------
Module name  : Syl_Sep  (Syllable Separation)
Parameters   : None
Function     : Find the location of the given text to be separated
Return       : Pointer to that location (Real Right Margin)

Author       :  Tanat Chirakarn
                Subun Yenjaichon

Date Written : Friday, 6 May, 1988 8:38:18 AM
Last Update  : Wednesday, 18 May, 1988
------------------------------------------------------------------------*/


unsigned char *OrAngRtn(TempIndx)
    unsigned char *TempIndx;

{
    unsigned char *TempIndxp1 = TempIndx +1;
    unsigned char *TempIndxm1 = TempIndx -1;
    unsigned char *TempIndxm2 = TempIndx -2;
    unsigned char *TempIndxm3 = TempIndx -3;
    unsigned char *TempIndxm4 = TempIndx -4;

    switch (*(TempIndxm1))
        {
            case MaiEk:
                if (findchar(*(TempIndxm2),"¢¤§©¹»¼ÇÍ"))
                    {
                        if ((TempIndxp1) <= RightMargin)
                            return(TempIndxp1);
                    }
                else
                    {
                        switch (*(TempIndxm2))                  /* Check to cut behind */
                            {
                                case PoreParn:
                                case ForFa:
                                    return(TempIndx);
                                case KoreGai:
                                case HorHeeb:
                                    if (*(TempIndxp1) != NoreNoo)
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case JoreJarn:
                                    if (*(TempIndxp1) != MoreMar)
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case ShoreChang:
                                    if (!findchar(*(TempIndxp1),"§¹"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case ForeFun:
                                    if (!findchar(*(TempIndxp1),"§¹"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case SoreSeo:
                                    if (!findchar(*(TempIndxp1),"§¹"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case SoreSoe:
                                    if (!findchar(*(TempIndxp1),"§¹Á"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case ToreTaharn:
                                    if (!findchar(*(TempIndxp1),"§¹Á"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case ToreTao:
                                    if (!findchar(*(TempIndxp1),"§ÁÂ"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case ThorToong:
                                    if (!findchar(*(TempIndxp1),"§ÁÂ"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case YoreYak:
                                    if (!findchar(*(TempIndxp1),"§¹ÁÂ"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case BoreBaimai:
                                    if (!findchar(*(TempIndxp1),"¹Â"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case MoreMar:
                                    if (!findchar(*(TempIndxp1),"§ÁÂ"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case RoreReo:
                                    if (!findchar(*(TempIndxp1),"§¹ÁÂ"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case LoreLing:
                                    if (!findchar(*(TempIndxp1),"¡§¹ÁÂ"))
                                        return(TempIndx);
                                    break;                  /* Check vowel */
                                case HorNokHook:
                                    if (*(TempIndxp1) != NgorNgoo)
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                default: return(FAIL);
                            }

                        if (istrvwl(*(TempIndx+2)) || isttnl(*(TempIndx+2)))      /* Check vowel routine here */
                            return(TempIndx);
                    }

                switch (*(TempIndxm2))                  /* Check to cut in front */
                    {
                        case RoreReo:
                            if (findchar(*(TempIndxm3),"¡¤¾Í"))
                                return(FAIL);
                            break;                      /* Check vowel */
                        case LoreLing:
                            if (*(TempIndxm3) == HorHeeb)
                                return(TempIndxm4); /* cut before HorHeeb */

                            if (findchar(*(TempIndxm3),"¡¤»¾Í"))
                                return(FAIL);
                            break;                      /* Check vowel */
                        case NoreNoo:
                            if (*(TempIndxm3) == HorHeeb)
                                return(TempIndxm4);
                            break;                      /* Check vowel */
                        case YoreYak:
                            if (*(TempIndxm3) == HorHeeb)
                                return(FAIL);
                            break;                      /* Check vowel */
                        case MoreMar:
                            if (findchar(*(TempIndxm3),"¢Ë"))
                                return(TempIndxm4);
                            break;                      /* Check vowel */
                        case NgorNgoo:
                            if (*(TempIndxm3) == HorHeeb)
                                return(TempIndxm4);
                            break;                      /* Check vowel */
                    } /* end switch */
                return(TempIndxm3);

            case MaiToe:
                if (findchar(*(TempIndxm2),"ª´¹ÂÊ"))
                    {
                        if ((TempIndxp1) <= RightMargin)
                            return(TempIndxp1);
                    }
                else
                    {
                        switch (*(TempIndxm2))                  /* Check to cut behind */
                            {
                                case RoreReo:
                                    if (*(TempIndxm3) != KoreGai && TempIndxp1 <= RightMargin)
                                        return(TempIndxp1);
                                    break;
                                case NgorNgoo:
                                case ChorChing:
                                case HorNokHook:
                                    return(TempIndx);
                                case KoreGai:
                                case JoreJarn:
                                    if (!findchar(*(TempIndxp1),"§¹Â"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case KorKai:
                                    if (!findchar(*(TempIndxp1),"§Â"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case KoreKwai:
                                    if (*(TempIndxp1) != NoreNoo)
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case SoreSoe:
                                case PorePla:
                                    if (!findchar(*(TempIndxp1),"§¹Á"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case ToreTao:
                                    if (!findchar(*(TempIndxp1),"§¹ÁÂ"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case ToreTaharn:
                                case ForeFun:
                                    if (!findchar(*(TempIndxp1),"§¹"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case BoreBaimai:
                                    if (!findchar(*(TempIndxp1),"§Á"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case PoreParn:
                                    if (*(TempIndxp1) != NgorNgoo)
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case LoreLing:
                                    if (!(findchar(*(TempIndxp1),"§Â¹Á")))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case HorHeeb:
                                    if (!findchar(*(TempIndxp1),"§ÁÂ"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                                case OrAng:
                                    if (!findchar(*(TempIndxp1),"¹ÁÂ"))
                                        return(TempIndx);
                                    break;                      /* Check vowel */
                            }   /* end switch */

                        if (istrvwl(*(TempIndx+2)) || isttnl(*(TempIndx+2)))      /* Check vowel routine here */
                            return(TempIndx);
                    }

                switch (*(TempIndxm2))                  /* Check to cut in front */
                    {
                        case RoreReo:
                            if (findchar(*(TempIndxm3),"¡¾Ê"))
                                return(FAIL);
                            break;                      /* Check vowel */
                        case LoreLing:
                            if (*(TempIndxm3) == KoreGai)
                                return(FAIL);
                            if (findchar(*(TempIndxm3),"¤»Ê"))
                                if (TempIndxp1 <= RightMargin)
                                    return(TempIndxp1);
                            break;                      /* Check vowel */
                        case YoreYak:
                            if (*(TempIndxm3) == KorKai)
                                return(TempIndxm4);
                            break;                      /* Check vowel */
                        case MoreMar:
                            if (findchar(*(TempIndxm3),"ªË"))
                                return(TempIndxm4);
                            break;                      /* Check vowel */
                    }
                return(TempIndxm3);
            case MaiTaiKuu:
                {
                    if ((TempIndxp1) <= RightMargin)
                        return(TempIndxp1);
                }

            default:
                {
                    if(!istcon(*(TempIndxm1)))
                        return(FAIL);
                    else
                        {
                            switch(*(TempIndxm2))
                                {
                                    case SaraAir:
                                    case SaraO:
                                    case MaiMuan:
                                    case MaiMalai:
                                        return(TempIndxm1);
                                    case SaraA:
                                        Indx = TempIndxm2;
                                        return((*FuncPtr[*Indx])(Indx));
                                } /* end switch */

                                if(istcon(*TempIndxm2))
                                    switch(*TempIndxm1)
                                    {
                                        case KorKai:
                                        case SoreSoe:
                                        case ForeFun:
                                            return((*TempIndxm2 != OrAng)? TempIndxm2 : FAIL);
                                        case OrAng:
                                            if (findchar(*TempIndxp1,"¡¹¿ÁÊ"))
                                                return(FAIL);
                                            else
                                                return((*TempIndxm2 == LoreLing) ? FAIL: TempIndxm1);
                                        default: return(FAIL);
                                    } /* end switch */
                            }   /* end else */
                    } /* end default */
    }   /* end of Outer Switch */
    return(FAIL);
} /* end of OrAngRtn() */