/*  Written: Suttipong Kanakakorn
            Thu  08-03-1989  10:15:28
    collect date, and time of final program
    we can't define in cw.c 'cause it will reflex the time
    that we compile cw.c not the time when the program is linked
*/
char prog_date[] = __DATE__;
char prog_time[] = __TIME__;
