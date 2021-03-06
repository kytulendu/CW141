#include "Thai.h"
#include "Global.h"
#include "TCtype.h"
#include "Routine.h"

unsigned char *SaraURtn(TempIndx)
    register unsigned char *TempIndx;
{
    register unsigned char *plus1 = TempIndx + 1, *minus2 = TempIndx - 2;

    if (findchar(*(plus1), "������������") ||
        *plus1 == PoreSumpao && nstrcmp("��", minus2) ||
        *plus1 == RoreReo && *(TempIndx - 1) != ToreTong &&
        nstrcmp("��", minus2) ||
        *plus1 == HorHeeb && nstrcmp("��", minus2))
        return(TempIndx);
    else if (plus1 <= RightMargin)
        {
            if (isttnl(*(plus1)))
                if (*(plus1) == MaiTri)
                    switch (*(TempIndx - 1))
                        {
                            case JoreJarn:  if (*(TempIndx + 2) != KoreGai && *(TempIndx + 2) != YoreYak)
                                                return(plus1);
                                            break;
                            case ToreTao:   if (!findchar(*(TempIndx + 2), "���"))
                                                return(plus1);
                                            break;
                            case PorePla:   if (!findchar(*(TempIndx + 2), "���"))
                                                return(plus1);
                                            break;
                        }
                else if (TempIndx + 2 <= RightMargin)
                        return(TempIndx + 2);
        }
    if (findchar(*(TempIndx - 1), "��������������������������"))
        return(minus2);
    else
        switch(*(TempIndx - 1))
            {
                case KoreGai:   return((findchar(*minus2, "����")) ? FAIL : minus2);
                case NgorNgoo:  return((*minus2 == HorHeeb || *minus2 == OrAng) ? FAIL : minus2);
                case DoreDek:   return((findchar(*minus2, "���")) ? FAIL : minus2);
                case ToreTao:   if (!nstrcmp("��", TempIndx - 3) || !nstrcmp("��", TempIndx - 3))
                                    return(TempIndx);
                                else
                                    return((*minus2 == JoreJarn) ? FAIL : minus2);
                case ThorToong: return((*minus2 == SoreSeo || *plus1 == Karan) ? FAIL : minus2);
                case ToreTaharn:return((*minus2 == PorePla) ? FAIL : minus2);
                case ToreTong:  return((*minus2 == MoreMar) ? FAIL : minus2);
                case NoreNoo:   return((findchar(*minus2, "�������")) ? FAIL : minus2);
                case MoreMar:   return((findchar(*minus2, "���")) ? FAIL : minus2);
                case YoreYak:   return((findchar(*minus2, "����")) ? FAIL : minus2);
                case RoreReo:   if (*(TempIndx - 3) == SaraA && *(TempIndx - 2) == MoreMar)
                                    return(TempIndx);
                                else
                                    return((findchar(*minus2, "�������������")) ? FAIL : minus2);
                case LoreLing:  return((findchar(*minus2, "�����������")) ? FAIL : minus2);
                case WoreWaan:  return((*minus2 == HorHeeb) ?   FAIL : minus2);
                case SoreSeo:   return((*minus2 == PoreParn || *minus2 == OrAng) ? FAIL : minus2);
                case HorHeeb:   return((*minus2 == PoreParn || *minus2 == LoreLing) ? FAIL : minus2);
                case OrAng:     return((*minus2 == ShoreChang) ? FAIL : minus2);
            }
    return(FAIL);
}