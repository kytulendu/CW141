#include <graphics.h>
#include <stdlib.h>

#include "fed.h"

void variable_blink(unsigned x,unsigned y);
void reverse(unsigned x,unsigned y);
void hgcwritech(int x,int y,char ch);
void ewritech(int x,int y,char ch);

void variable_blink(unsigned x,unsigned y)
{
	int i;
	reverse(x,y);
	for (i=0;i<4000;i++);
	reverse(x,y);
}

void reverse(unsigned x,unsigned y)
{
#ifdef NOALIGN
    putimage(16+x*8*BYTE+40,(y*Y)+40,char_block_image,XOR_PUT);
#else
	putimage(16+x*8*BYTE+align,(y*Y)+40,char_block_image,XOR_PUT);
#endif
}


void hgcwritech(int x,int y,char ch)
{
	int i,j;
    extern int herc_align;

    char    far *pos=(char far *)((0xb0000000L)+x+y*90+herc_align);
	char	*chr=all_font+ch*BYTE*Y;
	for(i=0;i<Y/4;i++)	{
		for(j=0;j<BYTE;j++)	{
			*(pos+j+i*90)=*chr;
			chr++;
		}
		for(j=0;j<BYTE;j++)	{
			*(pos+j+i*90+0x2000)=*chr;
			chr++;
		}
		for(j=0;j<BYTE;j++)	{
			*(pos+j+i*90+0x4000)=*chr;
			chr++;
		}
		for(j=0;j<BYTE;j++)	{
			*(pos+j+i*90+0x6000)=*chr;
			chr++;
		}
	}
	switch(Y%4)	{
		case 3 :	for(j=0;j<BYTE;j++)
						*(pos+j+i*90+0x4000)=*(chr+j+2*BYTE);
		case 2 :	for(j=0;j<BYTE;j++)
						*(pos+j+i*90+0x2000)=*(chr+j+BYTE);
		case 1 :	for(j=0;j<BYTE;j++)
						*(pos+j+i*90)=*(chr+j);
	}
}

void ewritech(int x,int y,char ch)
{
	int i,j;
	char	far *pos=(char far*)((0xa0000000L)+x+y*80*4);
	char	*chr=all_font+ch*BYTE*Y;

	for(i=0;i<Y;i++)	{
		for(j=0;j<BYTE;j++)	{
			*(pos+i*80+j)=*chr;
			chr++;
		}
	}
}
