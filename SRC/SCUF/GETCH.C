#include "key.h"

int cursorkey(char *key)
{
	*key=getch();
	if (*key==ESC) return(EXTEND);
        if (*key!=0) return(NORMAL);
	*key=getch();
	return(EXTEND);
}


int get_pos(x,y,block_x,block_y,width_x,width_y)
int *x,*y,width_x,width_y,block_x,block_y;
{
	char key;
		if(cursorkey(&key)==EXTEND)
		switch(key)	 {
			case UP :       *y-=width_y;
					break;
			case DOWN:      *y+=width_y;
					break;
			case RIGHT:     *x+=width_x;
					break;
			case LEFT:      *x-=width_x;
					break;
			case HOME:      *x-=width_x;*y-=width_y;
					break;
			case END:       *x-=width_x;*y+=width_y;
					break;
			case PGUP:      *x+=width_x;*y-=width_y;
					break;
			case PGDN:      *x+=width_x;*y+=width_y;
					break;
			case ESC:       return(3);
			case INS:       return(1);
			case DEL:       return(2);
			default :	return(key);
		}
			else
				return(key);
		if (*x>width_x*block_x) *x-=width_x;
		if (*x<0)               *x+=width_x;
		if (*y>width_y*block_y) *y-=width_y;
		if (*y<0)               *y+=width_y;

		return(0);
}
