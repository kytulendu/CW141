/*                  AT&T  graphics module  for CW 1.20                    */
/*                                                                        */
/*                      Kraisorn  Liansee                                 */
/*              Date:      04/21/1988                                     */
/*              Updated:   04/21/1988                                     */

#include <dos.h>


/*   AT&T    dependent function                   */
/*      This is a AT&T dependent function         */


extern unsigned int graph_seg;
extern int is_ega, is_vga;

void asetgraph (void)
{
       union REGS inregs,outregs;

       graph_seg =  0xB800;

       inregs.x.ax = 0x40 ;
       int86 (0x10,&inregs,&outregs);

       /*
       outportb (0x03C4,0x02);
       outportb (0x03C5,0x0F);
       */
}




void asettext (void)
{
       union REGS inregs,outregs;

       inregs.x.ax = 3;
       int86 (0x10,&inregs,&outregs);      /* text mode */
       inregs.x.ax = 3;
       int86 (0x10,&inregs,&outregs);      /* text mode */

}


unsigned int acompute_off (register unsigned x, register unsigned y)
{
       return (0x2000*(y%4) + 80*(y/4) + x/8);
}
