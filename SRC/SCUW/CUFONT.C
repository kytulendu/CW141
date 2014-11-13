#include <stdio.h>
#include <conio.h>

main (argc,argv)
int argc;
char *argv[];
{
    register int temp;
    temp = 0;
    while (argc -- > 0)
    {
     printf ("argv %d = %s\n",temp,argv[temp]);
     temp++;
    }
    getch ();
}