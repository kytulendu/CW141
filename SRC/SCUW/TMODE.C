#include <stdio.h>
#include "..\common\grdetect.h"

void main ()
{
     int mode;

     graph_detecthardware (&mode);
     printf ("Mode = %d\n",mode);
}
