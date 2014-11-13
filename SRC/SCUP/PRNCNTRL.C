/* ----------------------------------------------- */
/* Program      : PRNCNTRL.C                       */
/* Create Date  : 9 May 1989                       */
/* Programmer   : Mr. Prasart Topitumatukhun       */
/* Updated : Suttipong Kanakakorn                  */
/*   increse speed and add function prototype      */
/* Function     : control printer                  */
/* ----------------------------------------------- */

/* header file */
#include <dos.h>
#include <bios.h>
#include "const.h"
#include "cuptype.h"

void putp(unsigned c)
{
    /*
    union REGS regs;
    regs.h.ah = 0;
    regs.h.al = c;
    regs.h.dh = 0;
    regs.h.dl = 0;
    intr(0x17,&regs,&regs);
    */
    biosprint(0, c, 0);
}  

int PrinterReadstatus(void)
{
    /*
    union REGS regs;
    regs.h.ah = 0x02;
    regs.h.al = 0;
    regs.h.dh = 0;
    regs.h.dl = 0;
    return(int86(0x17,&regs,&regs));
    */
    return(biosprint(2, 0, 0));
}

void PrinterInitialize(void)
{
    putp(ESC);
    putp('@');
}

/*
void PrinterSetFormLenghtLine(register unsigned n)
{
    putp(ESC);
    putp('C');
    putp(n);
}
*/

void PrinterSetFormLenghtInch(register unsigned n)
{
    putp(ESC);
    putp('C');
    putp(0);
    putp(n);
}

void PrinterSkipLine(register int n)
{
    for (;n>0; n--) {
        putp(CR);
        putp(LF);
    }
}

void PrinterFormFeed(void)
{
    putp(FF);
}

/* modify by Suttipong Kanakakorn Tue  08-29-1989  12:56:39 */
void PrinterLineFeed(register unsigned n)
{
    putp(CR);
    if (n == 0)
        return;
    putp(ESC);
    putp('J');
    putp(n);    /* will feed n/180 inches on 24 pins printer and */
                /*           n/216 inches on  9 pins printer */
}

void PrinterMasterSelectMode(register unsigned n)
{
    putp(ESC);
    putp('!');
    putp(n);
}

void PrinterSetGraphicMode9pinQuadrupleDensity(register unsigned n)
{
    putp(ESC);
    putp('Z');
    putp(n%256);
    putp(n/256);
}

void PrinterSetGraphicMode24pinTripleDensity(register unsigned n)
{
    putp(ESC);
    putp('*');
    putp(39);
    putp(n%256);
    putp(n/256);
}

void PrinterSet10cpi(void)
{
    putp(ESC);
    putp('P');
}
