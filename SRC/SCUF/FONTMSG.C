#include <graphics.h>
#include <ctype.h>
#include <fcntl.h>
#include <io.h>

#include  "inc.h"
#include  "key.h"
#include "fed.h"

#include "..\common\cwgrphc.h"

extern int center_x,center_y;
extern int herc_align;

existfile(char *pname)
{
	int fd;
	if ((fd=open(pname,O_RDONLY))==-1)
		return 0;
	else
		close(fd);
	return !0;
}

/*
void main()
{
	char pname[81]="*.*",ans[2]="y";
	readfont();
	setgraph();
	clsall();
	center_x=320;center_y=348/2;
	fontnamebox(pname);
	error_message("แฟ้มข้อมูลผิดพลาด");
	immediatebox(30,4,"โปรดใส่ข้อมูล",ans);
	settext();
}
*/

put_box(x1,y1,x2,y2)
int x1,y1,x2,y2;
{
	char *buff;
	x1+=align;x2+=align;
	if((buff=(char*)malloc(imagesize(x1,y1,x2,y2)))==NULL)	{
		settext();
		puts("Error memory not enough");
		exit(1);
	}
	getimage(x1,y1,x2,y2,buff);
	putimage(x1,y1,buff,XOR_PUT);
	free(buff);
	x1-=align;x2-=align;
	box(x1,y1,x2,y2);
	box(x1+2,y1+2,x2-2,y2-2);
}

fontnamebox(char*pname,int creat)
/*	use creat=1 when want to creat new file that doesn't exist	*/
/*	    creat=0 when want to display a directory to screen when file
	    not exist							*/
{
	int ret;
	savepic();
	put_box(9*8-4,5*20,20*8+22*8+4,6*20+7);
	dispstrhgc("ใส่ชื่อแฟ้ม --> ",9,5,REVERSEATTR);
	getname(pname,21,5,20,REVERSEATTR);
	if(!creat)	{
		if(!existfile(pname))	{
			if(pname[0]=='\0')	strcpy(pname,"*.*");
			ret=(selectfile(pname)==NO)?ESC:RET;
		}	else
			ret=RET;
	}
	retpic();
	return ret;
}

error_message(char *prompt)
{
	savepic();

	put_box(center_x-16*thaistrlen(prompt)/2-5,8*20-20,
		center_x+16*thaistrlen(prompt)/2+3,8*20+7);
    dispstrhgc(prompt,40-thaistrlen(prompt)+herc_align,7,ENLARGEATTR|REVERSEATTR);
	bioskey(0);
	retpic();
}

_waitkbd(int x,int y)
{
	while(!keypressed())	{
		delay(5);
		setcurpos(x,y,0);
	}
	return getch();
}

prompter(char *ans,int x,int y,char attr)
{
	int k;
	dispstrhgc(ans,x,y,attr);
	while(!strchr("yYnN\r\x1b",(k=_waitkbd(x,y))));
	dispstrhgc(ans,x,y,attr);
	return toupper(k);
}

immediatebox(int x,int y,char *str,char*ans)
{
	static char buff[100];
	int ret;
	savepic();
	strcpy(buff,str);
	strcat(buff,"    ");
	put_box(x*8-4,y*20,
		x*8+8*thaistrlen(buff)+4,y*20+27);
	dispstrhgc(buff,x,y,REVERSEATTR);
	ret=(prompter(ans,x+thaistrlen(buff)-2,y,REVERSEATTR));
	retpic();
	if(ret=='\r')	ret=toupper(*ans);
	switch(ret)	{
		case 'Y':return 1;
		case 'N':return 0;
		case  27:return -1;
	}
}
