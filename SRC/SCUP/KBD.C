/* keyboard function that different between cuprint.exe and cw.exe
   Suttipong Kanakakorn Mon  08-07-1989  00:18:41
*/

#include "inc.h"
#include "..\common\cwgrphc.h"
#include "global.ext"

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
   register unsigned c;

   c = ebioskey(0);
   if (thaimode)
      return(thaikey(c));
   else
      return(c);
}
