/*                  EGA/VGA graphics module  for CW 1.20                  */
/*                                                                        */
/*                      Kraisorn  Liansee                                 */
/*              Date:      04/21/1988                                     */
/*              Updated:   04/21/1988                                     */

/* Updated: Suttipong Kanakakorn                                          */
/*          Mon  07-31-1989  19:03:13                                     */
/*          Tue  08-22-1989  01:53:19                                     */

#include <dos.h>

/*   EGA/VGA dependent function                   */
/*      This is a EGA/VGA dependent function      */

#include "cwgrphc.h"
#include "grdetect.h"

void esetgraph(void)
{
    union REGS inregs,outregs;

    graph_seg =  0xA000;

    /* code by Suttipong Kanakakorn */
    if (scrmode == VGA)
        inregs.x.ax = 0x11; /* 640 x 480 graphic on vga display */
    else if (scrmode == EGA64 || scrmode == EGA)
        inregs.x.ax = 0x10; /* 640 x 350 graphic on ega display */
    else
        inregs.x.ax = 0x0F; /* 640 x 350 mono-graphic on mono display */

    int86 (0x10,&inregs,&outregs);

    outportb (0x03C4,0x02);
    if (scrmode != EGAMONO)
        outportb (0x03C5,0x0F);
    else
        /* code by Suttipong Kanakakorn, use 2 plain only on mode 0F */
        outportb (0x03C5,0x03);
}




void esettext(void)
{
       union REGS inregs,outregs;

       inregs.x.ax = 3;
       int86 (0x10,&inregs,&outregs);      /* text mode */
}


unsigned compute_off (register unsigned x, register unsigned y)
{
       return (80*y+x/8);
}
