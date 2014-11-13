#include <graphics.h>
#include <alloc.h>
#include "inc.h"
#include "fontmsg.h"
#include "getch.h"
#include "popup.h"
#include "key.h"
#include "fed.h"

extern int herc_align;
extern int center_x,center_y;
int menu_top,menu_left,menu_right,menu_bottom;
char **menu_list;

menu_width()
{
	int i=0,width=0;
	while (menu_list[i])	{
		if (width<thaistrlen(menu_list[i]))
			width=thaistrlen(menu_list[i]);
		i++;
	}
	return width;
}

menu_height()
{
	int height=0;
	while(menu_list[height])	{
		height++;
	}
	return height;
}

remove_menu(char **p)
{
	putimage(menu_left,menu_top,*p,COPY_PUT);
	free(*p);
}


popup_message( char *item[])
{
	int amount=0,i=0,oldmode;
	char key,*buff;
	struct viewporttype tmp;
	int height,width;

	oldmode=get_active_clock();
	stop_clock();
	getviewsettings(&tmp);
	setviewport(0,0,getmaxx(),getmaxy(),1);
	menu_list=item;
	while(item[i])	{
		amount++;i++;
	}
	width=menu_width();
	height=menu_height();
	menu_left=center_x-width*8/2-6;
	menu_top=160-(height*20)/2;
	menu_right=center_x+width*8/2+7;
	menu_bottom=160+((1+height)*20)/2;

	buff=(char*)malloc(imagesize(menu_left,menu_top,menu_right,menu_bottom));
	getimage(menu_left,menu_top,menu_right,menu_bottom,buff);
	putimage(menu_left,menu_top,buff,XOR_PUT);


	/*
	  \/\/\/\/\/\/\/\/\    Draw frame    \/\/\/\/\/\/
	*/
	put_box(menu_left-align,menu_top,menu_right-align,menu_bottom);

	dispstrhgc(item[0],40-width/2,8-height/2,0);
    /*
    line(menu_left,menu_top+20+2,menu_right,menu_top+20+2);
    */
    line(menu_left-herc_align*8,menu_top+20+2,
         menu_right-herc_align*8,menu_top+20+2);
	for(i=1;i<amount;i++)
		dispstrhgc(item[i],40-width/2,8-height/2+i,0);
    /*
	line(menu_left,menu_top+20+2,menu_right,menu_top+20+2);
    */
    line(menu_left-herc_align*8,menu_top+20+2,
         menu_right-herc_align*8,menu_top+20+2);
	cursorkey(&key);
	putimage(menu_left,menu_top,buff,COPY_PUT);
	free(buff);
	setviewport(tmp.left,tmp.top,tmp.right,tmp.bottom,tmp.clip);
	start_clock(oldmode);
}


popup_menu(char *item[],char**buff)
{
	int amount=0,i=0,select=0,oldi;
	int width,height;
	char key;

	menu_list=item;
	while(item[i])	{
		amount++;i++;
	}

	width=menu_width();
	height=menu_height();

	menu_left=center_x-width*8/2-5;
	menu_top=347/2-height*20/2-5;
	menu_right=center_x+width*8/2+5;
	menu_bottom=347/2+height*20/2+5;

	*buff=(char*)malloc(imagesize(menu_left,menu_top,menu_right,menu_bottom));
	getimage(menu_left,menu_top,menu_right,menu_bottom,*buff);
	putimage(menu_left,menu_top,*buff,XOR_PUT);
	put_box(menu_left-align,menu_top,menu_right-align,menu_bottom);

	for(i=0;i<amount;i++)
		dispstrhgc(item[i],40-width/2,8-height/2+i,0);

    /*
	line(menu_left,menu_top+20+2,menu_right,menu_top+20+2);
    */
    line(menu_left-herc_align*8,menu_top+20+2,
         menu_right-herc_align*8,menu_top+20+2);


    dispstrhgc(item[1],40-width/2,8-height/2+1,REVERSEATTR);
    i=1;
    while (!select) {
            oldi=i;
	    cursorkey(&key);
            switch(key) {
                case UP:    i--;break;
                case DOWN:  i++;break;
		case RET:   goto exit;
            }
            if(i==0)    {
		oldi=1;
		i=amount-1;
            }
            if(i>=amount)   {
                i=1;
		oldi=amount-1;
            }
	    dispstrhgc(item[oldi],40-width/2,8-height/2+oldi,0);
	    dispstrhgc(item[i],40-width/2,8-height/2+i,REVERSEATTR);
    }
exit:
    return i-1;
}

