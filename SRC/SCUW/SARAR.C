#include "TCtype.h"
#include "Thai.h"
#include "Global.h"
#include "Routine.h"

/*  -------------SARA AR Rtn ------------
    Programmer : Subun Yenjaichon
    Date written : Monday, 9 May, 1988 3:09:08 PM
    Last Updated : Friday, 20 May, 1988 12:01:14 PM
    ----------------------------------  */

unsigned char *SaraRRtn(TempIndx)
register unsigned char *TempIndx;
 {
    register unsigned char *TempIndxm3 = TempIndx -3; /* for return point before A-2 */
    register unsigned char *charPtr;
    unsigned char chbufm1 = *(TempIndx - 1);
    unsigned char chbufm3 = *TempIndxm3;

    if  (istcon(chbufm1))
        if  (*(TempIndx - 2)== SaraA)
 /* After this comment is   �_� */
            switch (*(TempIndx +1))
            {
                case  SaraAh: /* �_�� */
                    if  ((TempIndx + 1) <=  RightMargin)
                        if (*(TempIndx -1)  ==  ToreMontoe)     /* ��� */
                            return(TempIndxm3);     /* cut before a-2 */
                        else return (TempIndx + 1);     /*cut after a+1*/
                    else return(TempIndxm3);        /* cut before a-2*/
                case    RoreReo:
                    if  (chbufm1== SoreSeo || chbufm1 == KoreKwai)  /* �����, ��þ*/
                        return(TempIndxm3);     /* cut before a-2  (/�_�) */
                    else return(TempIndx);          /* cut after a */
                case NoreNoo:
                    if (chbufm1 == KoreKwai || chbufm1 == PorePla   )   /* �ҹ�,�ҹ�*/
                        return(TempIndxm3);     /* cut before a-2 */
                    else return(TempIndx);          /* cut after a */
                case WoreWaan:
                    if (chbufm1 == SoreSeo || chbufm1== ShoreChang || chbufm1 == YoreYak)
                        return(TempIndxm3);     /* cut before a-2 */
                    else return(TempIndx);          /* cut after a */
                default:
                    return(TempIndx);   /* cut after a (�_�/) */

            }   /* end switch */
        else /* isn't SaraA */
            if (chbufm3 == SaraA)
                {   Indx = TempIndxm3;
                    return((*FuncPtr[*Indx])(Indx));    }   /* Jmp to SaraARtn */
            else
                return(SaraRWithoutTnl(TempIndx));
                    /* Sara ArRtn without tonal mark section */
    else
        switch (chbufm1)
        {
            case MaiTri:
                if (*(TempIndx -3)== SaraA)
                    if(*(TempIndx +1) == SaraAh && TempIndx +1 <= RightMargin)
                        return(TempIndx+1);     /*cut after a+1*/
                    else return(TempIndx-4);
                else
                    return(SaraRWithMaiTri(TempIndx));
            case MaiEk :
            case MaiToe :
            case MaiJattawa :
                if (*(TempIndx-3) == SaraA)
                    return(TempIndx);       /* cut after a*/
                else
                    if (*(TempIndx -4) == SaraA)
                    {   Indx = TempIndx - 4;
                        return((*FuncPtr[*Indx])(Indx));    } /* Jmp to Sara A Rtn */
                    else
                        return(SaraRWithTonal(TempIndx));
            default:
                return(NULL);
        } /* End Switch */

} /* end SaraRRtn */

/* SARA AR Without Tonal mark Rtn
    Date Written : Monday, 9 May, 1988 3:16:47 PM */

unsigned    char *SaraRWithoutTnl(TempIndx)
    register unsigned char *TempIndx;
{
    register unsigned char *TempIndxm2 = TempIndx -2;   /* for return point before A-1 */
    register unsigned char *TempIndxm3 = TempIndx -3;   /* for return point before A-2 */
    unsigned char chbufp1 = *(TempIndx+1);
    unsigned char chbufm1 = *(TempIndx-1);
    unsigned char chbufm2 = *TempIndxm2;

    if (findchar(chbufp1,"��������" )|| istlcon(chbufp1))
        return(TempIndx);
    else
        switch  (chbufp1)
        {
            case  PoreParn:
                if (findchar(chbufm1,"���") == 0) /* �Ҿ */
                    return(TempIndx);  /* cut after a */
                break;
            case KoreKwai:
                if (findchar(chbufm1,"����")== 0) /* �Ҥ */
                    return(TempIndx);
                break;
            case PorePla:
                if (findchar(chbufm1,"������") == 0) /* �һ */
                    return(TempIndx);
                break;
            case SoreSeo:
                if (findchar(chbufm1,"���������") == 0) /* ��� */
                    return(TempIndx);
                break;
            case KoreRakung:
                if (chbufm1 != MoreMar) /* �Ҧ */
                    return(TempIndx);
                break;
            case JoreJarn:
                if (findchar(chbufm1,"���") == 0) /* �Ҩ */
                    return(TempIndx);
                break;
            case ShoreChang:
                if (chbufm1 != RoreReo && chbufm1 != OrAng) /* �Ҫ�� */
                    return(TempIndx);
                break;
            case YoreYing:
                /* this mean if not found do (same as 'if (findchar(chbufm1,"�����")==0) */
                if (!(findchar(chbufm1,"�����"))) /* �ҭ�� */
                    return(TempIndx);
                break;
            case NoreNane:
                if (chbufm1 != YoreYing && chbufm1 != MoreMar) /* �ҳ*/
                    return(TempIndx);
                break;
            case ThorToong:
                if (chbufm1 != NoreNoo) /* �Ҷ */
                    return(TempIndx);
                break;
            case ToreTong:
                if (chbufm1 != YoreYak && chbufm1 != PoreParn)  /* �Ҹ */
                    return(TempIndx);
                break;
            case ToreTaharn:
                if (!(findchar(chbufm1,"������")))  /* �ҷ */
                    return(TempIndx);
                break;
            case ForeFun:
                if (chbufm1 != RoreReo && chbufm1 != LoreLing)  /* ��ҿ */
                    return(TempIndx);
                break;
            case PoreSumpao:
                if (chbufm1 != LoreLing)    /* ��� */
                    return(TempIndx);
                break;
            case SoreSala:
                if (!(findchar(chbufm1,"�ù��")))   /* �ҡ�� */
                    return(TempIndx);
                break;
            case SoreRusi:
                if (chbufm1 != DoreDek && chbufm1 != PoreSumpao)    /* ��ɴ� */
                    return(TempIndx);
                break;
        }   /*end switch */
    /* end if */

/* Front Cut Section */
    if (findchar(chbufm1,"�����������������") || istlcon(chbufm1))
        return(TempIndxm2); /* cut before A-1 */
    else
    {
        if (*TempIndxm2 == HorHeeb && findchar(chbufm1,"������"))
            return(TempIndxm3); /* cut before HorHeeb */
        switch (chbufm1)
        {
            case KoreGai:
                if (chbufm2 != PorPeng && chbufm2 != SoreSeo)   /* ��� */
                    return(TempIndxm2); /* cut before a-1 */
                break;
            case NgorNgoo:
                if (findchar(chbufp1,"�����"))
                {   if (chbufm2 != PorPeng && chbufm1 != HorHeeb)
                        return(TempIndxm2); } /*cut before a-1 */
                else return(TempIndx);  /* cut after a */
                break;
            case JoreJarn:
                if (!(chbufm2 == KorKai && chbufp1 == YoreYak)) /*  ���� */
                    return (TempIndxm2); /* cut before a -1 */
                else return(TempIndx-3); /* cut before KorKai */
            case PorePla:
                if (chbufm2  != SoreSeo)    /* ʻ�� */
                    return (TempIndxm2);    /* cut before a-1 */
                break;
            case NoreNane:
                if (chbufm2 != PorePla) /* ���� */
                    return (TempIndxm2);    /* cut before a-1 */
                else
                    if (chbufp1 != MoreMar)
                        return(TempIndx);
                break;
            case    NoreNoo:
                if (!(findchar(chbufm2,"���������")))   /* ��ҹ */
                    return (TempIndxm2);    /* cut before a-1 */
                break;
            case SoreSeo:
                if (chbufm2 != SoreSeo || chbufp1 !=  RoreReo ) /* ���� */
                    return (TempIndxm2);    /* cut before a-1 */
                break;
            case RoreReo:
                if (!(findchar(chbufm2,"����������������")))    /* ���� */
                    return (TempIndxm2);    /* cut before a-1 */
                break;
            case MoreMar:
                if (!(findchar(chbufm2,"�����")))   /* ��Ҥ� */
                    return(TempIndxm2); /* cut before a-1 */
                break;
            case YoreYak:
                if (!(findchar(chbufm2,"������")))  /* ���� */
                    return(TempIndxm2); /* cut before a-1 */
                break;
            case WoreWaan:
                if (!(findchar(chbufm2,"������������")))        /* ��ҧ */
                    return(TempIndxm2); /* cut before a-1 */
                break;
            case LoreLing:
                if (!(findchar(chbufm2,"�����������������")))   /* ��ҧ */
                    return(TempIndxm2); /*cut before a-1 */
                break;
            case ToreTaharn:
                if (chbufm2 != PorePla && chbufm2 !=  KoreGai && chbufm2 != KoreKwai) /* ��� */
                    return (TempIndxm2);    /* cut before a-1 */
                break;
            case ToreTao:
                if (chbufm2 != SoreSeo && chbufm2 != KoreGai)   /* ʵҧ�� */
                    return (TempIndxm2);    /* cut before a-1 */
                break;
            case HorHeeb:
                if (chbufm2 != MoreMar && chbufm2 != SoreSeo && chbufm2 != ToreTaharn) /* ���� */
                    return (TempIndxm2);    /* cut before a -1 */
                break;
            case BoreBaimai:
                if (chbufm2 != SoreSeo && chbufm2 != ShoreChang) /* ʺ�� */
                    return (TempIndxm2);    /* cut before a-1 */
                break;
            case PoreParn:
                if (chbufm2 != SoreSeo) /* ʾҹ */
                    return (TempIndxm2);    /* cut before a -1 */
                break;
            case ThorToong:
                if (chbufm2 != SoreSeo && chbufm2 != ToreTao)   /*  ��� */
                    return (TempIndxm2);    /* cut before a -1 */
                break;
            case PoreSumpao:
                if (chbufm2 != SoreSeo && chbufm2 != NoreNoo)   /* ��� */
                    return (TempIndxm2);    /* cut before a-1 */
                break;
            case DoreChada:
                if (chbufm2 != ShoreChang) /* ��� */
                    return(TempIndxm2); /* cut before a-1 */
                break;
        } /* end switch */
    }
    /* end if */
    return (NULL);
}  /* end  SARA AR Without Tonal */

unsigned char *SaraRWithMaiTri(TempIndx)
register unsigned char *TempIndx;
{
    unsigned char  chbufm2 = *(TempIndx -2);
    register unsigned char *TempIndxm3 = TempIndx -3;
    /* _�� Rtn */
        if (chbufm2 != RoreReo && chbufm2 != WoreWaan)  /* ���ҿ ���ҹ */
            return(TempIndxm3);
    /* end if */
    return(NULL);
}   /* end of SaraRWithMaiTri() */

unsigned char *SaraRWithTonal(TempIndx)
register unsigned char *TempIndx;
{
    register unsigned char *TempIndxm2 = TempIndx -2;   /* for return point before A-1 */
    register unsigned char *TempIndxm4 = TempIndx -4 ;
    unsigned char chbufm1 = *(TempIndx-1);
    unsigned char  chbufp1 = *(TempIndx +1);
    /* _��  _��  _�� Rtn */

    switch (*(TempIndx -1))
    {
        case MaiEk: /* _�� */
            switch (*(TempIndx -2))
            {
                case HorNokHook:
                case KoreRakung:
                    return(TempIndx);
                case KorKai:
                case NgorNgoo:
                    if (!(findchar(chbufp1 ,"�����"))) /* ��ҧ ��ҧ*/
                        return (TempIndx);
                    break;
                case PoreParn:
                case ToreTao:
                case ThorToong:
                    if (findchar(chbufp1,"���"))    /* ��� ��� ��� */
                        return((TempIndx+1) <= RightMargin)? TempIndx+1:TempIndx -3;
                        /* cut after a+1 or before a-2 */
                    break;
                case KoreKwai:
                case ChorChing:
                    if (chbufp1 != NgorNgoo && chbufp1 != YoreYak)
                        return(TempIndx);
                    break;
                case HorHeeb:
                case YoreYak:
                    if (chbufp1 != NgorNgoo &&
                        chbufp1 != NoreNoo && chbufp1 != MoreMar)   /*  ��ҧ */
                        return(TempIndx);
                    break;
                case KoreGai:
                case ForFa:
                    if (chbufp1 !=  YoreYak) /* ���� */
                        return(TempIndx);
                    break;
                case JoreJarn:
                case BoreBaimai:
                    if (chbufp1 != NgorNgoo &&
                        chbufp1 != YoreYak && chbufp1 != WoreWaan)  /* ���� */
                        return (TempIndx);
                    break;
                case ShoreChang:
                    if (chbufp1 != NgorNgoo)
                        return(TempIndx);
                    return;
                case SoreSoe:
                    if (chbufp1 != NoreNoo) /* ��ҹ */
                        return(TempIndx);
                    break;
                case DoreDek:
                case PorPeng:
                case OrAng:
                    if (chbufp1 != NgorNgoo &&
                        chbufp1 != NoreNoo && chbufp1 != WoreWaan)  /* ���� */
                        return(TempIndx);
                    break;
                case ToreTaharn:
                    if (chbufp1 != NoreNoo && chbufp1 != MoreMar) /* ���� ��ҹ */
                        return (TempIndx);
                    break;
                case NoreNoo:
                case MoreMar:
                    if (chbufp1 != NoreNoo && chbufp1 != YoreYak)   /* ��ҹ ���� */
                        return(TempIndx);
                    break;
                case PorePla:
                case WoreWaan:
                    if (!(findchar(chbufp1,"����")))    /*  ���� ��ҹ*/
                        return(TempIndx);
                    break;
                case RoreReo:
                    if (!(findchar(chbufp1,"����")))    /* ��ҹ */
                        return(TempIndx);
                    break;
                case LoreLing:
                    if (chbufp1 != NgorNgoo &&
                        chbufp1 != MoreMar && chbufp1 !=WoreWaan)   /* ����� ���� */
                        return(TempIndx);
                    break;
                case SoreSeo:
                    if (chbufp1 != NgorNgoo &&
                        chbufp1 != NoreNoo && chbufp1 != YoreYak)   /* ��ҹ */
                    break;
            }   /* end switch  inner  _�� */
            if (findchar(*TempIndxm2,"������������") || istlcon(*TempIndxm2))
                return(TempIndx-3); /* cut before A-2 */
            break;
            case  MaiToe:   /*  _�� */
            {
                switch (*(TempIndx-2))
                {
                    case    YoreYing:
                    case ThorToong:
                    case ForeFun:
                    case    HorNokHook:
                        return(TempIndx);
                    case DoreDek:
                    case RoreReo:
                        if (!(findchar(chbufp1,"�����")))
                            return(TempIndx);
                        break;
                    case KoreGai:
                        if (!(findchar(chbufp1,"����")))    /* ���� */
                            return (TempIndx);
                        break;
                    case MoreMar:
                    case LoreLing:
                        if (!(findchar(chbufp1,"����")))    /* ����� */
                            return(TempIndx);
                        break;
                    case KorKai:
                        if (!(findchar(chbufp1,"�����")))
                            return(TempIndx);
                        break;
                    case HorHeeb:
                        if (chbufp1 != NgorNgoo &&
                            chbufp1 != MoreMar && chbufp1 != WoreWaan) /* ���� */
                            return(TempIndx);
                        break;
                    case KoreKwai:
                        if (chbufp1 != NoreNoo && chbufp1 != NgorNgoo)  /* ��ҧ */
                            return(TempIndx);
                        break;
                    case NgorNgoo:
                        if (chbufp1 != NgorNgoo && chbufp1 != WoreWaan) /* ���� */
                            return(TempIndx);
                        break;
                    case JoreJarn:
                        if (chbufp1 != NgorNgoo &&
                            chbufp1 != WoreWaan && chbufp1 != NoreNoo)  /* ��ҧ */
                            return(TempIndx);
                        break;
                    case ShoreChang:
                        if (chbufp1 != NgorNgoo)    /* ��ҧ */
                            return (TempIndx);
                        break;
                    case SoreSoe:
                    case PorPeng:
                    case ForFa:
                    case YoreYak:
                        if (chbufp1 != YoreYak) /* ���� */
                            return(TempIndx);
                        break;
                    case    ToreTao:
                        if (chbufp1 != YoreYak && chbufp1 != NoreNoo)   /* ��ҹ */
                            return(TempIndx);
                    break;
                    case ToreTaharn:
                        if (chbufp1 !=  YoreYak &&
                            chbufp1 != NgorNgoo && chbufp1 != WoreWaan) /* ���� */
                            return(TempIndx);
                        break;
                    case NoreNoo:
                        if (chbufp1 !=  WoreWaan)   /* ���� */
                            return(TempIndx);
                        break;
                    case BoreBaimai:
                    case PorePla:
                    case WoreWaan:
                        if (chbufp1 !=  YoreYak && chbufp1 != NgorNgoo &&
                            chbufp1 != NoreNoo )    /* ���� */
                            return(TempIndx);
                        break;
                    case OrAng:
                        if (chbufp1 !=  YoreYak && chbufp1 != NgorNgoo) /* ��ҧ */
                            return(TempIndx);
                        break;
                }   /* end switch inner  _�� */
                if (findchar(*TempIndxm2,"������������") || istlcon(*TempIndxm2))
                    return(TempIndx-3); /* cut before A-2 */
                break;
            }
                case MaiJattawa:    /*  _�� */
                        return(TempIndx);
        } /* end switch out */
    if (*(TempIndx-3) == HorHeeb && findchar(*TempIndxm2,"������"))
        return(TempIndxm4); /* cut before HorHeeb */
    return(NULL);
}
