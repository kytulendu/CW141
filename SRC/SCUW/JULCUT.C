#include "Thai.h"
#include "Global.h"


/*------------------------------------------------------------------------
Module name  : Syl_Sep  (Syllable Separation)
Parameters   : None
Function     : Find the location of the given text to be separated
Return       : Pointer to that location (Real Right Margin)

Author       :  Tanat Chirakarn
                Subun Yenjaichon

Date Written : Friday, 6 May, 1988 8:38:18 AM
Last Update  : Monday, 9 May, 1988 2:19:20 PM
------------------------------------------------------------------------*/



unsigned char *MaiTaiKuuRtn(TempIndx)
    unsigned char *TempIndx;
{
    if (*(TempIndx+1) == OrAng)
        if (TempIndx+2 <= RightMargin)
            return(TempIndx+2);
        else
            return(FAIL);
    else
        if (*(TempIndx-1)==KoreGai)
            if (*(TempIndx-2)==SaraA || *(TempIndx-2)==SaraAir)
                if ((TempIndx+2)<=RightMargin)
                    return(TempIndx+2);
                else
                    return(TempIndx-3);
            else
                return(TempIndx);
        else
            if (*(TempIndx-2) == SaraA || *(TempIndx-2) == SaraAir
              || *(TempIndx-3)==SaraA || *(TempIndx-3)==SaraAir)
                if (*(TempIndx+3) == Karan)
                    if ((TempIndx+3) <= RightMargin)
                        return(TempIndx+3);
                    else
                        return(TempIndx-3);
                else
                    if ((TempIndx+1) <= RightMargin)
                        return(TempIndx+1);
                    else
                      if (*(TempIndx-2)==SaraA || *(TempIndx-2)==SaraAir)
                        return(TempIndx-3);
                      else
                        return(TempIndx-4);
            else
                return(FAIL);
}

unsigned char *SaraUURtn(TempIndx)
    unsigned char *TempIndx;
{
    if ((TempIndx + 1) <= EndOfText)
        switch (*(TempIndx + 1))
        {
            case ToreTao:
                if (!(*(TempIndx-1)==PoreSumpao || *(TempIndx-1)==ToreTaharn || *(TempIndx-1)==SoreSeo))
                    return(TempIndx);
                break;
            case PorePla:
                if (!(*(TempIndx-1)==RoreReo || *(TempIndx-1)==ToreTong || *(TempIndx-1)==ThorToong))
                    return(TempIndx);
                break;
            case KorKai:
            case KoreKwai:
            case JoreJarn:
            case ShoreChang:
            case SoreSoe:
            case ToreTong:
            case PoreParn:
            case PoreSumpao:
            case YoreYak:
            case WoreWaan:
            case ForeFun:
            case SoreSala:
            case SoreSeo:
            case OrAng:
            case ChorChing:
            case PorPeng:
            case ForFa:
            case HorNokHook:
                return(TempIndx);
            case MaiToe:
                if ((TempIndx+2)<=EndOfText)
                if (!(*(TempIndx+2)==DoreDek || *(TempIndx+2)==ToreTao || *(TempIndx+2)==ForeFun))
                  if (TempIndx+1<=RightMargin)
                    return(TempIndx+1);
                break;
            case MaiTri:
                if ((TempIndx+2)<=EndOfText)
                if (!(*(TempIndx+2)==DoreDek || *(TempIndx+2)==ToreTao
                    || *(TempIndx+2)==ToreTaharn || *(TempIndx+2)==PorePla
                    || *(TempIndx+2)==ForFa))
                      if (TempIndx+1<=RightMargin)
                        return(TempIndx+1);
                break;
            case MaiEk:
            case MaiJattawa:
                if (TempIndx+1<=RightMargin)
                    return(TempIndx+1);
                else
                    return(FAIL);
        }
    switch (*(TempIndx-1))
    {
        case KorKai:
        case KoreKwai:
        case KoreRakung:
        case NgorNgoo:
        case JoreJarn:
        case ShoreChang:
        case SoreSoe:
        case ShoreKracher:
        case YoreYing:
        case DoreChada:
        case TorePatak:
        case ToreMontoe:
        case TorePootao:
        case ToreTaharn:
        case ToreTong:
        case PorePla:
        case PoreParn:
        case ForeFun:
        case PoreSumpao:
        case SoreSala:
        case SoreRusi:
        case LorChula:
        case OrAng:
        case ChorChing:
        case PorPeng:
        case ForFa:
        case HorNokHook:
            return(TempIndx-2);
        case NoreNoo:
            if (!(*(TempIndx-2)==MoreMar || *(TempIndx-2)==HorHeeb || *(TempIndx-2)==ToreTong))
                return(TempIndx-2);
            break;
        case RoreReo:
            if (!(*(TempIndx-2)==PorePla || *(TempIndx-2)==HorHeeb ||
            *(TempIndx-2)==KoreKwai || *(TempIndx-2)==KoreGai || *(TempIndx-2)==JoreJarn
            || *(TempIndx-2)==ToreTao || *(TempIndx-2)==PoreParn))
                return(TempIndx-2);
            break;
        case MoreMar:
            if (!(*(TempIndx-2)==HorHeeb || *(TempIndx-2)==JoreJarn))
                return(TempIndx-2);
            break;
        case YoreYak:
            if (!(*(TempIndx-2)==OrAng || *(TempIndx-2)==HorHeeb))
                return(TempIndx-2);
            break;
        case WoreWaan:
            if (*(TempIndx-2)!=HorHeeb)
                return(TempIndx-2);
            break;
        case LoreLing:
            if (!(*(TempIndx-2)==PorePla || *(TempIndx-2)==HorHeeb || *(TempIndx-2)==OrAng
              || *(TempIndx-2)==ForeFun || *(TempIndx-2)==PoreParn || *(TempIndx-2)==ShoreChang
              || *(TempIndx-2)==KoreGai || *(TempIndx-2)==PorPeng || *(TempIndx-2)==ChorChing))
                return(TempIndx-2);
            break;
        case ToreTao:
            if (*(TempIndx-2)!=SoreSeo)
                return(TempIndx-2);
            break;
        case HorHeeb:
            if (*(TempIndx-2)!=PoreParn)
                return(TempIndx-2);
            break;
        case BoreBaimai:
            if (*(TempIndx-2)!=SoreSeo)
                return(TempIndx-2);
            break;
        case NoreNane:
            if (*(TempIndx-2)!=OrAng)
                return(TempIndx-2);
            break;
        case KoreGai:
            if (*(TempIndx-2)!=SoreSeo)
                return(TempIndx-2);
            break;
        case DoreDek:
            if (*(TempIndx-2)==RoreRu)
                return(TempIndx-3);
            else
                return(TempIndx-2);
        case SoreSeo:
            if (!(*(TempIndx-2)==OrAng))
                return(TempIndx-2);
            break;
        case ThorToong:
            if (*(TempIndx-2)!=SoreSeo)
                return(TempIndx-2);
            break;
    }
    return(FAIL);
}

unsigned char *SaraERtn(TempIndx)
    unsigned char *TempIndx;
{
    unsigned char *charPtr;
    unsigned char *CheckOther();

    if ((TempIndx + 1) <= EndOfText)
        switch (*(TempIndx + 1))
        {
            case SoreSoe:
                if (*(TempIndx-1)!=RoreReo)
                    return(TempIndx);
                break;
            case ForeFun:
                if (*(TempIndx-1)!=OrAng)
                    return(TempIndx);
                break;
            case HorHeeb:
                if (*(TempIndx-1)!=SoreSeo)
                    return(TempIndx);
                break;
            case KoreKwai:
                if (!(*(TempIndx-1)==LoreLing || *(TempIndx-1)==WoreWaan))
                    return(TempIndx);
                break;
            case SoreSeo:
                if (!(*(TempIndx-1)==YoreYak || *(TempIndx-1)==ToreTong || *(TempIndx-1)==OrAng))
                    return(TempIndx);
                break;
            case ShoreChang:
                if (*(TempIndx-1)!=PoreParn)
                    return(TempIndx);
                break;
            case LoreLing:
                if (!(*(TempIndx-1)==NoreNoo || *(TempIndx-1)==ForeFun
                  || *(TempIndx-1)==ToreTong || *(TempIndx-1)==SoreSala))
                    return(TempIndx);
                break;
            case KoreRakung:
                if (*(TempIndx-1)!=ToreTaharn)
                    return(TempIndx);
                break;
            case WoreWaan:
            case PoreParn:
                if (*(TempIndx-1)!=ShoreChang)
                    return(TempIndx);
                break;
            case KoreGai:
                if (!(*(TempIndx-1)==ChorChing || *(TempIndx-1)==SoreSoe
                  || *(TempIndx-1)==PorePla || *(TempIndx-1)==OrAng))
                    return(TempIndx);
                break;
            case MoreMar:
                if (!(*(TempIndx-1)==KoreKwai || *(TempIndx-1)==ToreTaharn
                  || *(TempIndx-1)==RoreReo))
                    return(TempIndx);
                break;
            case ToreTao:
                if (!(*(TempIndx-1)==KoreKwai || *(TempIndx-1)==NoreNane
                  || *(TempIndx-1)==RoreReo))
                    return(TempIndx);
                break;
            case PorePla:
                if (!(*(TempIndx-1)==KorKai || *(TempIndx-1)==ToreTaharn
                  || *(TempIndx-1)==WoreWaan))
                    return(TempIndx);
                break;
            case DoreDek:
                switch (*(TempIndx-1))
                {
                    case RoreReo:
                        if (!(*(TempIndx-2)==KoreGai || *(TempIndx-2)==HorHeeb))
                            return(TempIndx-2);
                        break;
                    case WoreWaan:
                        if (*(TempIndx-2)!=HorHeeb)
                            return(TempIndx-2);
                        break;
                }
            case NoreNoo:
                switch (*(TempIndx-1))
                {
                    case RoreReo:
                        if (*(TempIndx-2)!=KoreGai)
                            return(TempIndx-2);
                        break;
                }
            case RoreReo:
                break;
            case YoreYak:
                charPtr = CheckOther(TempIndx);
                if (charPtr!=NULL)
                    return(charPtr);
                else
                    switch (*(TempIndx-1))
                    {
                        case RoreReo:
                            if (!(*(TempIndx-2)==ToreTao || *(TempIndx-2)==PorePla
                              || *(TempIndx-2)==HorHeeb))
                                return(TempIndx-2);
                            break;
                        case NoreNoo:
                            if (!(*(TempIndx-1)==SoreSeo))
                                return(TempIndx-2);
                            break;
                    }
                break;
            case BoreBaimai:
                switch (*(TempIndx-1))
                {
                    case LoreLing:
                        if (*(TempIndx-2)!=KoreGai)
                            return(TempIndx-2);
                        break;
                    case RoreReo:
                        if (*(TempIndx-2)!=KoreKwai)
                            return(TempIndx-2);
                        break;
                }
            case MaiTri:
                if (TempIndx+2<=RightMargin)
                    if (*(TempIndx+2)!=YoreYak)
                        return(FAIL);
                    else
                        return(CheckOther(TempIndx));
                break;
            case MaiEk:
            case MaiToe:
            case MaiJattawa:
                if (TempIndx+2<=RightMargin)
                    if (*(TempIndx+2)!=YoreYak)
                        return(TempIndx+1);
                    else
                        return(CheckOther(TempIndx));
                break;
            default:
                return(TempIndx);
        }
    switch (*(TempIndx-1))
    {
        case KorKai:
        case KoreKwai:
        case KoreRakung:
        case NgorNgoo:
        case ShoreChang:
        case SoreSoe:
        case ShoreKracher:
        case YoreYing:
        case DoreChada:
        case TorePatak:
        case ThorSantan:
        case TorePootao:
        case ToreTao:
        case ThorToong:
        case ToreTaharn:
        case ToreTong:
        case PorePla:
        case ForeFun:
        case PoreSumpao:
        case SoreSala:
        case SoreRusi:
        case SoreSeo:
        case LorChula:
        case OrAng:
        case ChorChing:
        case PorPeng:
        case ForFa:
        case HorNokHook:
            return(TempIndx-2);
        case KoreGai:
            if (*(TempIndx-2)!=SoreSeo)
                return(TempIndx-2);
            break;
        case JoreJarn:
            if (!(*(TempIndx-2)==KorKai || *(TempIndx-2)==WoreWaan))
                return(TempIndx-2);
            break;
        case YoreYak:
        case NoreNoo:
            if (*(TempIndx-2)!=HorHeeb)
                return(TempIndx-2);
            break;
        case MoreMar:
            if (!(*(TempIndx-2)==HorHeeb || *(TempIndx-2)==SoreSeo))
                return(TempIndx-2);
            break;
        case PoreParn:
            if (*(TempIndx-2)!=RoreReo)
                return(TempIndx-2);
            break;
    }
    return(FAIL);
}

unsigned char *CheckOther(TempIndx)
    unsigned char *TempIndx;
{
    unsigned char *SaraErEkRtn();
    unsigned char *SaraErToeRtn();
    unsigned char *SaraErRtn();
    unsigned char *SaraAEEkRtn();
    unsigned char *SaraAEToeRtn();
    unsigned char *SaraAETriRtn();
    unsigned char *SaraAEJatRtn();
    unsigned char *SaraAERtn();

    if (*(TempIndx-3)==SaraA)
        switch (*(TempIndx+1))
        {
            case MaiEk:
                return(SaraErEkRtn(TempIndx));
            case MaiToe:
                return(SaraErToeRtn(TempIndx));
            case YoreYak:
                return(SaraErRtn(TempIndx));
        }
    else
        if (*(TempIndx-2)==SaraA)
            switch (*(TempIndx+1))
            {
                case MaiEk:
                    return(SaraAEEkRtn(TempIndx));
                case MaiToe:
                    return(SaraAEToeRtn(TempIndx));
                case MaiTri:
                    return(SaraAETriRtn(TempIndx));
                case MaiJattawa:
                    return(SaraAEJatRtn(TempIndx));
                case YoreYak:
                    return(SaraAERtn(TempIndx));
            }
        else
            switch (*(TempIndx+1))
            {
                case MaiEk:
                case MaiToe:
                case MaiJattawa:
                    return(TempIndx+1);
                default:
                    return(FAIL);
            }
}

unsigned char *SaraAEEkRtn(TempIndx)
    unsigned char *TempIndx;
{
    if ((TempIndx+2)<=RightMargin)
        if ((TempIndx+3)<=RightMargin)
            switch (*(TempIndx+3))
            {
                case NgorNgoo:
                case WoreWaan:
                case NoreNoo:
                case MoreMar:
                    if (!(*(TempIndx-1)==KorKai || *(TempIndx-1)==ToreTao
                      || *(TempIndx-1)==RoreReo || *(TempIndx-1)==SoreSeo
                      || *(TempIndx-1)==HorHeeb))
                        return(TempIndx+3);
                    break;
                default:
                    return(TempIndx+2);
            }
    return(TempIndx-3);
}

unsigned char *SaraAEToeRtn(TempIndx)
    unsigned char *TempIndx;
{
    if (TempIndx+2<=RightMargin)
        if (*(TempIndx+3)==NgorNgoo || *(TempIndx+3)==MoreMar || *(TempIndx+3)
          ==NoreNoo || *(TempIndx+3)==WoreWaan || *(TempIndx+3)==DoreDek)
            if (TempIndx+3<=RightMargin)
                if (*(TempIndx-1)==ToreTao || *(TempIndx-1)==PorePla
                  || *(TempIndx-1)==HorHeeb)
                    return(TempIndx-3);
                else
                    return(TempIndx+3);
            else
                return(FAIL);
        else
            return(TempIndx+2);
    else
        return(TempIndx-3);
}

unsigned char *SaraAETriRtn(TempIndx)
    unsigned char *TempIndx;
{
    if (TempIndx+3<=RightMargin)
        return(TempIndx+3);
    else
        return(TempIndx-3);
}

unsigned char *SaraAEJatRtn(TempIndx)
    unsigned char *TempIndx;
{
    if (TempIndx+3<=RightMargin)
        if (*(TempIndx-1)!=KoreGai)
            return(TempIndx+3);
    return(TempIndx-3);
}

unsigned char *SaraAERtn(TempIndx)
    unsigned char *TempIndx;
{
    if (TempIndx+1<=RightMargin)
        switch (*(TempIndx+2))
        {
            case KoreGai:
                if (!(*(TempIndx-1)==KoreGai || *(TempIndx-1)==PorePla
                  || *(TempIndx-1)==RoreReo))
                    return(TempIndx+1);
                break;
            case JoreJarn:
                if (*(TempIndx-1)!=KoreGai)
                    return(TempIndx+1);
                break;
            case NgorNgoo:
            case BoreBaimai:
            case DoreDek:
            case NoreNoo:
            case MoreMar:
            case RoreReo:
            case WoreWaan:
            case YoreYing:
                break;
            default:
                return(TempIndx+1);
        }
    return(TempIndx-3);
}

unsigned char *SaraErEkRtn(TempIndx)
    unsigned char *TempIndx;
{
    if (TempIndx+3<=RightMargin)
        if (*(TempIndx+3)==NgorNgoo || *(TempIndx+3)==NoreNoo
          || *(TempIndx+3)==MoreMar || *(TempIndx+3)==WoreWaan)
            if (!((*(TempIndx-1)==LoreLing && *(TempIndx-2)==ChorChing) ||
              (*(TempIndx-1)==LoreLing && *(TempIndx-2)==KoreGai)))
                return(TempIndx+3);
    return(FAIL);
}

unsigned char *SaraErToeRtn(TempIndx)
    unsigned char *TempIndx;
{
    if (TempIndx+2<=RightMargin)
        if (*(TempIndx+3)==NgorNgoo || *(TempIndx+3)==WoreWaan)
            if (*(TempIndx-1)==LoreLing)
                if (!(*(TempIndx-2)==KoreGai || *(TempIndx-2)==PorePla
                  || *(TempIndx-2)==PoreParn))
                    return(TempIndx+2);
                else
                    return(FAIL);
            else
                return(TempIndx+2);
    return(FAIL);
}

unsigned char *SaraErRtn(TempIndx)
    unsigned char *TempIndx;
{
    if (TempIndx+2<=RightMargin)
        switch (*(TempIndx+2))
        {
            case Karan:
                return(TempIndx+2);
            case KoreGai:
            case NgorNgoo:
            case NoreNoo:
            case BoreBaimai:
            case MoreMar:
            case WoreWaan:
            case KoreKwai:
            case YoreYing:
            case RoreReo:
                if (!(*(TempIndx-1)==LoreLing && *(TempIndx-2)==PoreParn))
                    return(TempIndx+2);
                break;
        }
    return(FAIL);
}