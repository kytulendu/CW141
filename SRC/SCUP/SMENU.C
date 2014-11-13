#include <stdio.h>
#include "inc.h"
#include "..\common\kbdcode.h"
#include "..\common\cwgrphc.h"

/*
    smenu -> select menu
    - popup menu for user to select, programmer must save and restore
      the screen by himslef

    - input x, y, number of messege, messege, curmenu
    - output curmenu
    - return ESCKEY or RETKEY

    Written: Suttipong Kanakakorn
    Date: Fri  08-18-1989  09:16:37
*/

int smenu(register int x, register int y, int number,
            char *message[], int *curmenu)
{
    int i;
    int c, length;
    char fmt[7];

    length = find_menu_len(number, message);
    sprintf(fmt, "%%-%ds", length);
    /*
    scrndx = savescrn(x-1, y-1, x+1+length, y+number+1);
    */
    for (i = 0; i < number; i++) {
          dispprintf(x, y+i, NORMALATTR, fmt, message[i]);
    }
    i = *curmenu - 1;
    dispprintf(x, y+i, REVERSEATTR, fmt, message[i]);
    box(x*8 - 2, y*20 + 2, (x+length-1)*8 + 9, (y+number)*20 + 5);
    while (1) {
        c = ebioskey(0);
        switch(c) {
        case UPKEY  :
            dispprintf(x, y+i, NORMALATTR, fmt, message[i]);
            i = (i) ? i-1 : number - 1;
            dispprintf(x, y+i, REVERSEATTR, fmt, message[i]);
            break;
        case DNKEY  :
            dispprintf(x, y+i, NORMALATTR, fmt, message[i]);
            i = (i != number - 1) ? i+1 : 0;
            dispprintf(x, y+i, REVERSEATTR, fmt, message[i]);
            break;
        case ESCKEY :
        case RETKEY :
            /*
            resscrn(scrndx, x-1, y-1, x+1+length, y+number+1);
            */
            *curmenu = i + 1;
            return(c);
        }
    }
}

unsigned find_menu_len(register int number, char *message[])
{
    register unsigned length;
    int i;
    char **m;

    length = thaistrlen(message[0]);
    for (m = message+1; m < message + number; m++) {
        if ((i = thaistrlen(*m)) > length)
            length = i;
    }
    return length;
}
