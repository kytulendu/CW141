#include <dos.h>

#include "fed.h"
#include "inc.h"
#include "..\common\cwgrphc.h"

static int clock_busy=STOP;

static  void interrupt far (*oldvect28)(void);
static  void interrupt far (*oldvect23)(void);

extern int draw_mode;

int cur_char;

union {
	char far *ptr;
	struct	      {
		unsigned Ofs;
		unsigned Seg;
	} x;
} ActivePtr;

void start_clock(int func)
{
    clock_busy=func;
}

get_active_clock()
{
	return  clock_busy;
}

void stop_clock()
{
	clock_busy=STOP;
}

void interrupt far newint28(void)
{
	static char buff[40];
	enable();
	(*oldvect28)();
    if(clock_busy)
	switch(clock_busy)	{
		case EDIT_FONT:blink(pos_x,pos_y);
			switch(draw_mode)	{
				case 0:dispstrhgc("Move mode",50,16,REVERSEATTR);
					return;
				case 1:dispstrhgc("Draw mode",50,16,REVERSEATTR);
					return;
				case 2:dispstrhgc("Erasemode",50,16,REVERSEATTR);
					return;
			}
			break;
		case SELECT_FONT:
			variable_blink(sel_x,sel_y);
			sprintf(buff,"Character # %3d : 0x%02x",cur_char,cur_char);
			dispstrhgc(buff,55,0,0);
			return;
	}
}

void interrupt far newint23(void){}

void init_clock()
{
    union REGS reg;
	struct SREGS sreg;
	oldvect23=getvect(0x23);
    oldvect28=getvect(0x28);
    reg.x.ax=0x3400;
    intdosx(&reg,&reg,&sreg);
    ActivePtr.x.Seg=sreg.es;
    ActivePtr.x.Ofs=reg.x.bx;
	setvect(0x28,newint28);
	setvect(0x23,newint23);
}

void	flush_clock(void)
{
	clock_busy=STOP;
	setvect(0x28,oldvect28);
	setvect(0x23,oldvect23);
}

