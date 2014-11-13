#include <string.h>
#include "inc.h"

void disperror(char *s)
{
    clear_message(); /* from printing.obj */
    dispstrhgc(s,40-strlen(s)/2,13,2);
    /* errorsound(); */
    while (!keypressed());
    clear_message(); /* from printing.obj */
}

