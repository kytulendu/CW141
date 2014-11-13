#include "Thai.h"
#include "Ctype.h"
#include "TCtype.h"
#include "routine.h"

/*  -----------------------------------------
    Programmer : Subun & Tanat
    Date Written :  7 May, 1988
    Last Updated : 19 May, 1988

    For Thai Syllable Separation Program.
    Allright Reserved by Subun & Tanat Advanced Computer Incorp. 1988
    Used With Permission, Sahaviriya System.
    ----------------------------------------- */

/*-------------------------Global declaration-------------------------*/
unsigned char *dummy();
unsigned char *CntlChar();
unsigned char *PuncChar();
unsigned char *TermChar();
unsigned char *FCPuncChar();
unsigned char *PaiYarnnoyRtn();
unsigned char *MaiYamokRtn();

/* Pointer to function */
unsigned char *(*FuncPtr[])() = {   dummy,  /*  character 0x00  */
                                            CntlChar,   /*  character 0x01  */
                                            CntlChar,   /*  character 0x02  */
                                            CntlChar,   /*  character 0x03  */
                                            CntlChar,   /*  character 0x04  */
                                            CntlChar,   /*  character 0x05  */
                                            CntlChar,   /*  character 0x06  */
                                            CntlChar,   /*  character 0x07  */

                                            CntlChar,   /*  character 0x08  */
                                            CntlChar,   /*  character 0x09  */
                                            CntlChar,   /*  character 0x0A  */
                                            CntlChar,   /*  character 0x0B  */
                                            CntlChar,   /*  character 0x0C  */
                                            CntlChar,   /*  character 0x0D  */
                                            CntlChar,   /*  character 0x0E  */
                                            CntlChar,   /*  character 0x0F  */

                                            CntlChar,   /*  character 0x10  */
                                            CntlChar,   /*  character 0x11  */
                                            CntlChar,   /*  character 0x12  */
                                            CntlChar,   /*  character 0x13  */
                                            CntlChar,   /*  character 0x14  */
                                            CntlChar,   /*  character 0x15  */
                                            CntlChar,   /*  character 0x16  */
                                            CntlChar,   /*  character 0x17  */

                                            CntlChar,   /*  character 0x18  */
                                            CntlChar,   /*  character 0x19  */
                                            CntlChar,   /*  character 0x1A  */
                                            CntlChar,   /*  character 0x1B  */
                                            CntlChar,   /*  character 0x1C  */
                                            CntlChar,   /*  character 0x1D  */
                                            CntlChar,   /*  character 0x1E  */
                                            CntlChar,   /*  character 0x1F  */

                                            CntlChar,   /*  Blank character 0x20 */
                                            PuncChar,   /*  character 0x21  */
                                            dummy,  /*  character 0x22  */
                                            dummy,  /*  character 0x23  */
                                            PuncChar,   /*  character 0x24  */
                                            PuncChar,   /*  character 0x25  */
                                            dummy,  /*  character 0x26  */
                                            dummy,  /*  character 0x27  */

                                            FCPuncChar, /*  character 0x28  */
                                            PuncChar,   /*  character 0x29  */
                                            dummy,  /*  character 0x2A  */
                                            dummy,  /*  character 0x2B  */
                                            TermChar,   /*  character 0x2C  */
                                            PuncChar,   /*  character 0x2D  */
                                            TermChar,   /*  character 0x2E  */
                                            FCPuncChar, /*  character 0x2F  */

                                            dummy,  /*  character 0x30  */
                                            dummy,  /*  character 0x31  */
                                            dummy,  /*  character 0x32  */
                                            dummy,  /*  character 0x33  */
                                            dummy,  /*  character 0x34  */
                                            dummy,  /*  character 0x35  */
                                            dummy,  /*  character 0x36  */
                                            dummy,  /*  character 0x37  */

                                            dummy,  /*  character 0x38  */
                                            dummy,  /*  character 0x39  */
                                            PuncChar,   /*  character 0x3A  */
                                            PuncChar,   /*  character 0x3B  */
                                            dummy,  /*  character 0x3C  */
                                            dummy,  /*  character 0x3D  */
                                            dummy,  /*  character 0x3E  */
                                            PuncChar,   /*  character 0x3F  */

                                            dummy,  /*  character 0x40  */
                                            dummy,  /*  character 0x41  */
                                            dummy,  /*  character 0x42  */
                                            dummy,  /*  character 0x43  */
                                            dummy,  /*  character 0x44  */
                                            dummy,  /*  character 0x45  */
                                            dummy,  /*  character 0x46  */
                                            dummy,  /*  character 0x47  */

                                            dummy,  /*  character 0x48  */
                                            dummy,  /*  character 0x49  */
                                            dummy,  /*  character 0x4A  */
                                            dummy,  /*  character 0x4B  */
                                            dummy,  /*  character 0x4C  */
                                            dummy,  /*  character 0x4D  */
                                            dummy,  /*  character 0x4E  */
                                            dummy,  /*  character 0x4F  */

                                            dummy,  /*  character 0x50  */
                                            dummy,  /*  character 0x51  */
                                            dummy,  /*  character 0x52  */
                                            dummy,  /*  character 0x53  */
                                            dummy,  /*  character 0x54  */
                                            dummy,  /*  character 0x55  */
                                            dummy,  /*  character 0x56  */
                                            dummy,  /*  character 0x57  */

                                            dummy,  /*  character 0x58  */
                                            dummy,  /*  character 0x59  */
                                            dummy,  /*  character 0x5A  */
                                            FCPuncChar, /*  character 0x5B  */
                                            dummy,  /*  character 0x5C  */
                                            PuncChar,   /*  character 0x5D  */
                                            dummy,  /*  character 0x5E  */
                                            dummy,  /*  character 0x5F  */

                                            dummy,  /*  character 0x60  */
                                            dummy,  /*  character 0x61  */
                                            dummy,  /*  character 0x62  */
                                            dummy,  /*  character 0x63  */
                                            dummy,  /*  character 0x64  */
                                            dummy,  /*  character 0x65  */
                                            dummy,  /*  character 0x66  */
                                            dummy,  /*  character 0x67  */

                                            dummy,  /*  character 0x68  */
                                            dummy,  /*  character 0x69  */
                                            dummy,  /*  character 0x6A  */
                                            dummy,  /*  character 0x6B  */
                                            dummy,  /*  character 0x6C  */
                                            dummy,  /*  character 0x6D  */
                                            dummy,  /*  character 0x6E  */
                                            dummy,  /*  character 0x6F  */

                                            dummy,  /*  character 0x70  */
                                            dummy,  /*  character 0x71  */
                                            dummy,  /*  character 0x72  */
                                            dummy,  /*  character 0x73  */
                                            dummy,  /*  character 0x74  */
                                            dummy,  /*  character 0x75  */
                                            dummy,  /*  character 0x76  */
                                            dummy,  /*  character 0x77  */

                                            dummy,  /*  character 0x78  */
                                            dummy,  /*  character 0x79  */
                                            dummy,  /*  character 0x7A  */
                                            FCPuncChar, /*  character 0x7B  */
                                            FCPuncChar, /*  character 0x7C  */
                                            PuncChar,   /*  character 0x7D  */
                                            dummy,  /*  character 0x7E  */
                                            dummy,  /*  character 0x7F  */

                                            dummy,  /*  character 0x80  */
                                            dummy,  /*  character 0x81  */
                                            dummy,  /*  character 0x82  */
                                            dummy,  /*  character 0x83  */
                                            dummy,  /*  character 0x84  */
                                            dummy,  /*  character 0x85  */
                                            dummy,  /*  character 0x86  */
                                            dummy,  /*  character 0x87  */

                                            dummy,  /*  character 0x88  */
                                            dummy,  /*  character 0x89  */
                                            dummy,  /*  character 0x8A  */
                                            dummy,  /*  character 0x8B  */
                                            dummy,  /*  character 0x8C  */
                                            dummy,  /*  character 0x8D  */
                                            dummy,  /*  character 0x8E  */
                                            dummy,  /*  character 0x8F  */

                                            dummy,  /*  character 0x90  */
                                            dummy,  /*  character 0x91  */
                                            dummy,  /*  character 0x92  */
                                            dummy,  /*  character 0x93  */
                                            dummy,  /*  character 0x94  */
                                            dummy,  /*  character 0x95  */
                                            dummy,  /*  character 0x96  */
                                            dummy,  /*  character 0x97  */

                                            dummy,  /*  character 0x98  */
                                            dummy,  /*  character 0x99  */
                                            dummy,  /*  character 0x9A  */
                                            dummy,  /*  character 0x9B  */
                                            dummy,  /*  character 0x9C  */
                                            dummy,  /*  character 0x9D  */
                                            dummy,  /*  character 0x9E  */
                                            dummy,  /*  character 0x9F  */

                                            dummy,  /*  character 0xA0  */
                                            dummy,  /*  character 0xA1  */
                                            dummy,  /*  character 0xA2  */
                                            dummy,  /*  character 0xA3  */
                                            dummy,  /*  character 0xA4  */
                                            dummy,  /*  character 0xA5  */
                                            dummy,  /*  character 0xA6  */
                                            dummy,  /*  character 0xA7  */

                                            dummy,  /*  character 0xA8  */
                                            LeadConRtn, /*  character 0xA9  */
                                            dummy,  /*  character 0xAA  */
                                            dummy,  /*  character 0xAB  */
                                            dummy,  /*  character 0xAC  */
                                            dummy,  /*  character 0xAD  */
                                            dummy,  /*  character 0xAE  */
                                            dummy,  /*  character 0xAF  */

                                            dummy,  /*  character 0xB0  */
                                            dummy,  /*  character 0xB1  */
                                            dummy,  /*  character 0xB2  */
                                            dummy,  /*  character 0xB3  */
                                            dummy,  /*  character 0xB4  */
                                            dummy,  /*  character 0xB5  */
                                            dummy,  /*  character 0xB6  */
                                            dummy,  /*  character 0xB7  */

                                            dummy,  /*  character 0xB8  */
                                            dummy,  /*  character 0xB9  */
                                            dummy,  /*  character 0xBA  */
                                            dummy,  /*  character 0xBB  */
                                            LeadConRtn, /*  character 0xBC  */
                                            LeadConRtn, /*  character 0xBD  */
                                            dummy,  /*  character 0xBE  */
                                            dummy,  /*  character 0xBF  */

                                            dummy,  /*  character 0xC0  */
                                            dummy,  /*  character 0xC1  */
                                            dummy,  /*  character 0xC2  */
                                            RoreReoRtn, /*  character 0xC3  */
                                            RoreRuRtn,  /*  character 0xC4  */
                                            dummy,  /*  character 0xC5  */
                                            dummy,  /*  character 0xC6  */
                                            WoreWaanRtn,    /*  character 0xC7  */

                                            dummy,  /*  character 0xC8  */
                                            dummy,  /*  character 0xC9  */
                                            dummy,  /*  character 0xCA  */
                                            HorHeebRtn, /*  character 0xCB  */
                                            dummy,  /*  character 0xCC  */
                                            OrAngRtn,   /*  character 0xCD  */
                                            LeadConRtn, /*  character 0xCE  */
                                            PaiYarnnoyRtn,  /*  character 0xCF  */

                                            SaraAhRtn,  /*  character 0xD0  */
                                            HunAkadRtn, /*  character 0xD1  */
                                            SaraRRtn,   /*  character 0xD2  */
                                            SaraUmRtn,  /*  character 0xD3  */
                                            SaraIeRtn,  /*  SaraIe character 0xD4 */
                                            SaraERtn,   /*  character 0xD5  */
                                            SaraUeRtn,  /*  character 0xD6  */
                                            SaraUeeRtn, /*  character 0xD7  */

                                            SaraURtn,   /*  character 0xD8  */
                                            SaraUURtn,  /*  character 0xD9  */
                                            TermChar,   /*  character 0xDA  */
                                            dummy,  /*  character 0xDB  */
                                            dummy,  /*  character 0xDC  */
                                            dummy,  /*  character 0xDD  */
                                            dummy,  /*  character 0xDE  */
                                            PuncChar,   /*  character 0xDF  */

                                            SaraARtn,   /*  character 0xE0  */
                                            SaraAirRtn, /*  character 0xE1  */
                                            SaraORtn,   /*  character 0xE2  */
                                            MaiMuanRtn, /*  character 0xE3  */
                                            MaiMalaiRtn,    /*  character 0xE4  */
                                            dummy,  /*  character 0xE5  */
                                            MaiYamokRtn,    /*  character 0xE6  */
                                            MaiTaiKuuRtn,   /*  character 0xE7  */

                                            dummy,  /*  character 0xE8  */
                                            dummy,  /*  character 0xE9  */
                                            dummy,  /*  character 0xEA  */
                                            dummy,  /*  character 0xEB  */
                                            KaranRtn,   /*  character 0xEC  */
                                            dummy,  /*  character 0xED  */
                                            dummy,  /*  character 0xEE  */
                                            dummy,  /*  character 0xEF  */

                                            dummy,  /*  character 0xF0  */
                                            dummy,  /*  character 0xF1  */
                                            dummy,  /*  character 0xF2  */
                                            dummy,  /*  character 0xF3  */
                                            dummy,  /*  character 0xF4  */
                                            dummy,  /*  character 0xF5  */
                                            dummy,  /*  character 0xF6  */
                                            dummy,  /*  character 0xF7  */

                                            dummy,  /*  character 0xF8  */
                                            dummy,  /*  character 0xF9  */
                                            dummy,  /*  character 0xFA  */
                                            dummy,  /*  character 0xFB  */
                                            dummy,  /*  character 0xFC  */
                                            dummy,  /*  character 0xFD  */
                                            dummy,  /*  character 0xFE  */
                                            dummy   /*  character 0xFF  */
                                        };

/* Bit definition for TCtype.h */
short int _tctype[] = {
    /* control char begin from ASCII 00H */
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* special character begin from ASCII 20H */
                        0x0000, 0x0100, 0x0000, 0x0000, 0x0100, 0x0100, 0x0000, 0x0000,
                        0x0200, 0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0100, 0x0100, 0x0000, 0x0000, 0x0000, 0x0100,
    /* ASCII 40H English char */
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0200, 0x0000, 0x0100, 0x0000, 0x0000,
    /* ASCII 60H */
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0200, 0x0000, 0x0100, 0x0000, 0x0000,
    /* ASCII 80H begin ISO (SoreMoreOr Code) */
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* begin Thai char ASCII 0A0H */
                        0x0000, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080,
                        0x0080, 0x00C0, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080,
                        0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080,
                        0x0080, 0x0080, 0x0080, 0x0080, 0x00C0, 0x00C0, 0x0080, 0x0080,
    /* ASCII 0C0H */
                        0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080,
                        0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x00C0, 0x0002,
                        0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010,
                        0x0010, 0x0010, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000, 0x0100,
    /* ASCII 0E0H */
                        0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0010, 0x0002, 0x0010,
                        0x0008, 0x0008, 0x0008, 0x0008, 0x0004, 0x0010, 0x0000, 0x0000,
                        0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
                        0x0001, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
                      };

unsigned char *Indx, *RightMargin, *EndOfText, *LeftLimit;

/*-------------------------end Global-------------------------*/

#define FAIL 0

unsigned char *dummy(TempIndx)
    unsigned char *TempIndx;
{
    return(FAIL);
}

unsigned char *CntlChar(TempIndx)
    unsigned char *TempIndx;
{
    return(TempIndx);
}

unsigned char *PuncChar(TempIndx)
    unsigned char *TempIndx;
{
    return(TempIndx);
}

unsigned char *TermChar(TempIndx)
unsigned char *TempIndx;
{
    unsigned char *TempIndxp1 = TempIndx+1;

    if (!(isdigit(*TempIndxp1) || istdigit(*TempIndxp1)))
            return(TempIndx);
                /* cut after fullstop and Comma if
                    the following character is digit */
    return(FAIL);
}   /* end TermChar() */

unsigned char *FCPuncChar(TempIndx)
unsigned char *TempIndx;
{
    return(TempIndx-1);
}

unsigned char *PaiYarnnoyRtn(TempIndx)
unsigned char *TempIndx;
{
    unsigned char *TempIndxp1 = TempIndx+1;
    unsigned char *TempIndxm1 = TempIndx-1;

    if (!nstrcmp("оео",TempIndx-2)) return(TempIndx);
    if (!nstrcmp("о╬Ёо",TempIndx-3))    return(TempIndx-4);
    if (!nstrcmp("оео",TempIndx))
        return((TempIndx+2 <= RightMargin)? TempIndx+2 : FAIL);

    if (!nstrcmp("о╬Ёо",TempIndx))  return(TempIndx-1);

    return(TempIndx);   /* cut after PaiYarnnoy */
} /* end PaiYarnnoyRtn() */

unsigned char *MaiYamokRtn(TempIndx)
unsigned char *TempIndx;
{
    return(TempIndx);
}