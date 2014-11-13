#include <bios.h>

#include "inc.h"
#include "var.h"
#include "..\common\cwgrphc.h"

void waitkbd(unsigned x, unsigned y)
{
    register unsigned i;

    setcurpos(x,y,thaimode);
    while (!keypressed()) {
        for (i=0; !keypressed() && i < 1200 ; i++)
            ;
        setcurpos(x,y,thaimode);
        for (i=0; !keypressed() && i < 1200 ; i++)
            ;
        setcurpos(x,y,thaimode);
    }
    setcurpos(x,y,thaimode);
}

int readkbd(void)
{
   unsigned c;
   c = bioskey(0);
   switch(c) {
   case ALTA : c = 0x80;
               break;
   case ALTB : c = 0x81;
               break;
   case ALTC : c = 0x88;
               break;
   case ALTD : c = 0x87;
               break;
   case ALTE : c = 0x85;
               break;
   case ALTI : c = 0x84;
               break;
   case ALTO : c = 0x83;
               break;
   case ALTQ : c = 0x8c;
               break;
   case ALTR : c = 0x86;
               break;
   case ALTS : c = 0x8b;
               break;
   case ALTT : c = 0x89;
               break;
   case ALTU : c = 0x8e;
               break;
   case ALTY : c = 0x82;
               break;
   default : if (thaimode) {
               if ((bioskey(2) & 0x20) == 0) {
                 return(thaikey(c));
               } else {
                 if ((c & 0xff00) < 0x4700)
                   return(thaikey(c));
               }
             }
             break;
   }
   return(c);
}
