#include "TCtype.h"
#include "Thai.h"
#include "Global.h"

/*  -------------SARA Ie Rtn ------------
    Programmer : Subun Yenjaichon
    Date written : Tuesday, 17 May, 1988 2:53:20 PM
    Last Updated :  Monday, 23 May, 1988
    ----------------------------------  */

unsigned char *SaraIeRtn(TempIndx)
unsigned char *TempIndx;
{
    short   i;
    unsigned char *TempIndxm1 = TempIndx - 1;
    unsigned char *TempIndxm2 = TempIndx - 2;
    unsigned char *TempIndxm3 = TempIndx - 3;
    unsigned char *TempIndxm4 = TempIndx - 4;
    unsigned char *TempIndxp1 = TempIndx +1;
    unsigned char *TempIndxp2 = TempIndx +2;
    static unsigned char *DoreDekSaraIeEx[] = {"¾ÃÃ´Ô","ÇÃÃ´Ô"};
    static unsigned char *ToreTaoSaraIeEx[] = {"ÇÑµÔ","ºÑµÔ","­ÑµÔ","¹ÔµÔ",
                    "ÂØµÔ","ÊÙµÔ","ªÒµÔ","­ÒµÔ","âªµÔ"};
    static unsigned char *JoreJarnSaraIeEx[] = {"ÊÙ¨Ô","ÊØ¨Ô"};

/* start  check SaraA before */
    if (*TempIndxm3 == SaraA)
        if (isttnl(*TempIndxp1) && TempIndxp2 <= RightMargin)
            return(TempIndxp2); /* cut after speller */
        else
        {   Indx = TempIndxm3;
            return((*FuncPtr[*Indx])(Indx)); /* Jmp to SaraARtn */
        }
    else
        if (*TempIndxm2 == SaraA)
        {
            if(isttnl(*TempIndxp1))
                return((TempIndxp2 <=  RightMargin)?  TempIndxp2 : TempIndxm3);
            return((TempIndxp1<= RightMargin)? TempIndxp1 : TempIndxm3);
        }

/* begin SaraIeRtn with out SaraA before */
    if (isttnl(*TempIndxp1) &&  TempIndxp2 <= RightMargin)
        return(TempIndxp2);

    if (findchar(*TempIndxp1,"¦¬®¯±²¶ÀÂÄËÍ"))
        return(TempIndx);

    switch (*TempIndxp1)
    {
            case  KorKai:
                if (*TempIndxm1 != LoreLing)
                    return(TempIndx);
                break;
            case KoreKwai:
                if (!(findchar(*TempIndxm1,"¹»¿ÃÅÊ")))
                    return(TempIndx);
                break;
            case JoreJarn:
                if (!(findchar(*TempIndxm1,"¡¹ÁÍ")))
                    return(TempIndx);
                break;
            case SoreSoe:
                if (*TempIndxm1 != MoreMar)
                    return(TempIndx);
                break;
            case YoreYing:
                if (!(findchar(*TempIndxm1,"¯ÀÃÇÊ")))
                    return(TempIndx);
                break;
            case ThorSantan:
                if (*TempIndxm1 != OrAng)
                    return(TempIndx);
                break;
            case NoreNane:
                if (!(findchar(*TempIndxm1,"µ¾É")))
                    return(TempIndx);
                break;
            case ToreTaharn:
                if (!(findchar(*TempIndxm1,"¹¾ÅÇÊÍ")))
                    return(TempIndx);
                break;
            case ToreTong:
                if(*TempIndxm1 != PoreParn)
                    return(TempIndx);
                break;
            case PoreParn:
                if(!(findchar(*TempIndxm1,"ª·¹Å")))
                    return(TempIndx);
                break;
            case ForeFun:
                if(!(findchar(*TempIndxm1,"¡Å")))
                    return(TempIndx);
                break;
            case LoreLing:
                if(!(findchar(*TempIndxm1,"«¹º¿ÁÃÇÈ")))
                    return(TempIndx);
                break;
            case SoreSala:
                if (!(findchar(*TempIndxm1,"·¾ÃÅÇÍ")))
                    return(TempIndx);
            case SoreRusi:
                if(!(findchar(*TempIndxm1,"¸¹¾ÃÇÈ")))
                    return(TempIndx);
                break;
            case SoreSeo:
                if(!(findchar(*TempIndxm1,"«¹¾¿ÁÃÇÍÎ")))
                    return(TempIndx);
                break;
        } /* end switch */
/*  Front Cut */
    if (findchar(*TempIndxm1,"¡¢¤¦«¬±·»¿ÉÌÍ") || istlcon(*TempIndxm1))
        return(TempIndxm2);  /* cut before consonant */
    switch (*TempIndxm1)
    {
        case JoreJarn:
            for(i=0; i <= 1;++i)
                if (!(nstrcmp(JoreJarnSaraIeEx[i],TempIndxm3)))
                    return(TempIndx);
            return(TempIndxm2); /* cut before JoreJarn */
        case DoreDek:
            for (i=0;i<= 1;++i)
            if (!(nstrcmp(DoreDekSaraIeEx[i],TempIndxm4)))
                return(TempIndx-5);
            if (!(nstrcmp("ÊÇÑÊ´",(TempIndx-5))))
                return(TempIndx-6);
            return(TempIndxm2); /* cut before DoreDeck */
        case ToreTao:
            for (i=0;i<= 8;++i)
                if (!(nstrcmp(ToreTaoSaraIeEx[i],TempIndxm3)))
                    return(TempIndx);
            if (!(findchar(*TempIndxm2,"¡¤¹ÃÄÊ")))
                return(TempIndxm2); /* cut before ToreTao */
            break;
        case TorePootao:
            break;  /* can't cut before TorePootao */
        case NoreNoo:
            if (!(findchar(*TempIndxm2,"¢ª¶ÃÇÊËÍ")))
                return(TempIndxm2); /* cut before norenoo */
            break;
        case RoreReo:
            if(!(findchar(*TempIndxm2,"¡¢¤¨µ¹º»¼¾ÀËÍ")))
                return(TempIndxm2); /* cut before RoreReo */
            break;
        case NgorNgoo:
            if (*TempIndxm2 != HorHeeb)
                return(TempIndxm2); /* cut before NgorNgoo */
            else
                return(TempIndxm3); /* cut before HorHeeb */
        case MoreMar:
            if (!(nstrcmp("ÀÙÁÔ",TempIndxm3)))
                return(TempIndx); /* cut after SaraIe */
            if (!(findchar(*TempIndxm2,"¢·ÊËÍ")))
                return(TempIndxm2); /* cut before MoreMar */
            break;
        case YoreYak:
            if (!(findchar(*TempIndxm2,"¢ÊË")))
                return(TempIndxm2); /* cut before YoreYak */
            break;
        case WoreWaan:
            if(!(findchar(*TempIndxm2,"¢¤¶·ÃÊËÍ")))
                return(TempIndxm2); /* cut before WoreWaan */
            break;
        case LoreLing:
            if (!(findchar(*TempIndxm2,"¡¢ªµ»¼¾ÅÇÊËÍ")))
                return(TempIndxm2); /* cut before LoreLing */
            break;
        case HorHeeb:
            if (*TempIndxm2 != OrAng )
                return(TempIndxm2); /* cut before HorHeeb */
            break;
        case ShoreChang:
            if (*TempIndxm2 != WoreWaan && *TempIndxm2 != KoreKwai)
                return(TempIndxm2); /* cut before ShoreChang */
            break;
        case ThorToong:
            if(*TempIndxm2 != SoreSeo)
                return(TempIndxm2); /* cut before ThorToong */
            break;
        case NoreNane:
            break; /* uncut before norenane */
        case ToreTong:
            if(!(nstrcmp("ÊØ·¸Ô",TempIndxm4)))
                return(TempIndx); /* if found cut after saraIe */
            if(*TempIndxm2 != OrAng)
                return(TempIndxm2); /* cut before ToreTong */
            break; /* uncut before ToreTong */
        case PoreSumpao:
            if (*TempIndxm2 != OrAng)
                return(TempIndxm2); /* cut before PoreSumpao */
            break;
        case SoreSeo:
            if (!(nstrcmp("¡ÊÔ¡Ã",TempIndxm2)))
                return(TempIndxm3); /* cut before KoreGai */
            else
                return(TempIndxm2); /* cut before SoreSeo */
        case SoreSala:
            if (*TempIndxm2 != SoreSala)
                return(TempIndxm2); /* cut before SoreSala */
            break;
        case PoreParn:
            if (*TempIndxm2 != BoreBaimai)
                return(TempIndxm2); /* cut before PoreParn */
            break;
    } /* end switch */

    return(FAIL);
} /* end of SaraIeRtn() */