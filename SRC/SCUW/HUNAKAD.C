#include "Thai.h"
#include "Global.h"
#include "TCtype.h"
#include "Routine.h"

unsigned char *HunAkadRtn(TempIndx)
    register unsigned char *TempIndx;
{
    register unsigned char *plus1 = TempIndx + 1, *minus2 = TempIndx - 2;
    if (plus1 <= RightMargin)
        {
            switch(*(plus1))
                {
                    case KoreGai:   if (findchar(*(TempIndx - 1), "¡§ª«´µ¶º»¿ÁÇË"))
                                        return(plus1);
                                    break;
                    case NgorNgoo:  switch(*(TempIndx - 1))
                                        {
                                            case SoreSeo:   if (*(TempIndx + 2) != KorKai && *(TempIndx + 2) != KoreRakung)
                                                                return(plus1);
                                                            break;
                                            case RoreReo:   if (!nstrcmp("¡ÃÑ§´ì", minus2))
                                                                return(TempIndx - 3);
                                                            else
                                                                return(plus1);
                                            default:        return(plus1);
                                        }
                                    break;
                    case DoreDek:   return(plus1);
                    case NoreNoo:   if (*(TempIndx + 3) != Karan)
                                        return(plus1);
                                    break;
                    case YoreYak:   if (!nstrcmp("¹ÑÂ¹ì", TempIndx - 1))
                                        return(minus2);
                                    else
                                        return(plus1);
                    case SoreSeo:   if (*(TempIndx + 4) != Karan)
                                        return(plus1);
                                    break;
                    case WoreWaan:  switch(*(TempIndx + 2))
                                        {
                                            case SaraAh:    if (TempIndx + 2 <= RightMargin)
                                                                return(TempIndx + 2);
                                                            break;
                                            case RoreReo:   if (*(TempIndx + 3) == Karan)
                                                                return(minus2);
                                                            break;
                                            default:        return(plus1);
                                        }
                                    break;
                    default:        if (isttnl(*(plus1)))
                                        if (*(TempIndx + 4) == Karan)
                                                return(minus2);
                                        else
                                            {
                                                if (TempIndx + 2 <= RightMargin)
                                                    return(TempIndx + 2);
                                            }
                }
            switch(*(TempIndx - 1))
                {
                    case NgorNgoo:  return(plus1);
                    case NoreNoo:   if (!findchar(*(plus1), "µ¹Â"))
                                        return(plus1);
                                    break;
                    case MoreMar:   if (!findchar(*(plus1), "¤¹Ç"))
                                        return(plus1);
                                    break;
                    case RoreReo:   if (!findchar(*(plus1), "¡µ¾È"))
                                        return(plus1);
                                    break;
                    case HorHeeb:   if (*(plus1) != ToreTao && *(plus1) != NoreNoo)
                                        return(plus1);
                                    break;
                    case HorNokHook:return(plus1);
                }
        }
    if (findchar(*(TempIndx - 1), "¢¤¦ª«­±³¶¸»¼½¾¿ÈÊÍÎ"))
        return(minus2);
    else
        switch(*(TempIndx - 1))
            {
                case KoreGai:   return(*minus2 == SoreSeo ? FAIL : minus2);
                case NgorNgoo:  switch(*minus2)
                                    {
                                        case HorHeeb:   return(TempIndx - 3);
                                        case SoreSeo:   return(FAIL);
                                        default:        return(minus2);
                                    }
                case JoreJarn:  return((*minus2 == KorKai) ? FAIL : minus2);
                case DoreDek:   return((*minus2 == SoreSeo) ? FAIL : minus2);
                case ToreTao:   return((*minus2 == KoreGai) ? FAIL : minus2);
                case ToreTaharn:switch(*minus2)
                                    {
                                        case HorHeeb:   return(TempIndx - 3);
                                        case WoreWaan:  return(FAIL);
                                        default:        return(minus2);
                                    }
                case BoreBaimai:return((*minus2 == ChorChing) ? TempIndx - 3 : minus2);
                case PoreSumpao:return((*minus2 == OrAng) ? FAIL : minus2);
                case WoreWaan:  return(findchar(*minus2, "¡¢¤¨©µ¶¸ÊË") ? FAIL : minus2);
                case HorHeeb:   return(findchar(*minus2 ,"ÁÃÍ")? FAIL :minus2);
            }
    return(FAIL);
}