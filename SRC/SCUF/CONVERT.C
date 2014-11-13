/*
Updated: Suttipong Kanakakorn Sun  08-27-1989  23:07:23
         automatic file all font files
*/

#include <io.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "inc.h"
#include "var.h"
#include "fed.h"
#include "key.h"

#define test_bit(buff,pos)	((buff)&(0x01<<(pos)))
#define set_bit(buff,pos)	(*(buff)|=(0x01<<(pos)))

/*
extern char cuwordpath[],cufontpath[],cuprintpath[];
*/
extern int loaded,load_first;
search_file_mode  font_found;

int load_scr(),load_prn(),load_24(),load_sup_sub(),load_fed();
int save_scr(),save_prn(),save_24(),save_sup_sub(),save_fed();



int (*load_func[])()	={
	load_scr,load_scr,load_prn,load_prn,load_24,load_24,
	load_sup_sub,load_sup_sub,load_fed
};

int (*save_func[])()	={
	save_scr,save_scr,save_prn,save_prn,save_24,save_24,
	save_sup_sub,save_sup_sub,save_fed
};

char *file_name[]=	{
	"normal.fon",
	"italic.fon",
	"normal.prn",
	"italic.prn",
	"normal.p24",
	"italic.p24",
	"normals.p24",
	"italics.p24"
};


load_font()
{
    char *oldfont,oldx=X,oldy=Y;
    unsigned int oldbyte=BYTE;
    char oldfname[MAXPATH];

    strcpy(oldfname,fname);

    if (!first_file)	{
	    if ((oldfont=(char*)malloc(256*BYTE*Y))==NULL)	{
		error_message("Not enough memory11");
		X=oldx;Y=oldy;BYTE=oldbyte;
		strcpy(fname,oldfname);
		return 0;
	    }

	    memcpy(oldfont,all_font,256*BYTE*Y);
    }
    if (new_load==9) {
        /*
	    strcpy(fname,cufontpath);
	    strcat(fname,"*.FED");
        */
        if ((font_found = find_file("*.FED", cuf_dir)) == CW_DIR)
            sprintf(fname, "%s\\%s", cuf_dir, "*.FED");
        else
            strcpy(fname,"*.FED");

        if(fontnamebox(fname,0)==ESC) {
		X=oldx;Y=oldy;BYTE=oldbyte;
		if(!first_file)	{
			memcpy(all_font,oldfont,256*BYTE*Y);
			free(oldfont);
		}
		strcpy(fname,oldfname);
		return 0;
	    }
    }	else	{
        /*
        if (new_load==1 || new_load==2) {
            strcpy(fname,cuwordpath);
            strcat(fname,file_name[new_load-1]);
        }   else    {
            strcpy(fname,cuprintpath);
            strcat(fname,file_name[new_load-1]);
        }
        */
        if ((font_found = find_file(file_name[new_load-1], cuf_dir))
                         == CW_DIR)
            sprintf(fname, "%s\\%s", cuf_dir, file_name[new_load-1]);
        else
            strcpy(fname, file_name[new_load-1]);
    }

    load_first=1;
    if(((*load_func[new_load-1])())==0)	{
		X=oldx;Y=oldy;BYTE=oldbyte;
		if(!first_file)	{
			memcpy(all_font,oldfont,256*BYTE*Y);
			free(oldfont);
		}
			strcpy(fname,oldfname);
		return 0;
    }
    if(!first_file)	{
	free(oldfont);
    }
    return 1;
}


save_font()
{
	return ((*save_func[loaded-1])());
}



save_fed()
/*
	/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
		save font from all_font
	\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
*/
{
	int fd;
	if ((fd=_creat(fname,FA_ARCH))==-1)	{
		error_message("ERROR : CREATING FONT FILE");
		return 0;
	};
	if (_write(fd,SIGN,SIGN_LENGTH)==-1)	{
		error_write();
		return 0;
	};
	if (_write(fd,"\x1a",1)==-1)	{
		error_write();
		return 0;
	};
	if (_write(fd,&X,1)==-1)	{
		error_write();
		return 0;
	};
	if (_write(fd,&Y,1)==-1)	{
		error_write();
		return 0;
	};
	if (_write(fd,all_font,256*BYTE*Y)==-1)	{
		error_write();
		return 0;
	}
	close(fd);
	return ~0;
}


cleanup_load_routine()
{
	pos_x=pos_y=0;
	clearworkarea();
	make_char_block_image();
}

load_fed()
/*
	/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
				malloc all_font
				load font into all_font
				set X, Y
				set pos_x,pos_y to zero
				draw grid
				malloc matrix_image
				set BYTE	=	sizeof X in bytes
	\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
*/

{
	char dum[81];
	int fd;
	if ((fd=_open(fname,O_RDONLY|O_BINARY))==-1)	{
		error_message("ERROR : OPENING FONT FILE");
		return 0;
	};
	if (_read(fd,dum,SIGN_LENGTH)==-1 || strncmp(dum,SIGN,SIGN_LENGTH))  {
		error_message("ERROR : INVALID FONT FILE");
		return 0;
	};
	if (_read(fd,&X,1),_read(fd,&X,1)==-1)	{
		error_read();
		return 0;
	};
	if (_read(fd,&Y,1)==-1)	{
		error_read();
		return 0;
	};
	setup_memo();
	if (_read(fd,all_font,256*BYTE*Y)==-1)	{
		error_read();
		return 0;
	}
	close(fd);
	cleanup_load_routine();
	return ~0;
}


load_24()
{
    char *oldfon;

	int fd,filelen;

	if((fd=_open(fname,O_RDONLY))==-1)	{
		error_message("Open reading error");
		return 0;
	}
	if((filelen=filelength(fd))!=3*18*(256-32))	{
		error_message("Incorrect font file!");
		return 0;
	}

	X=18;Y=24;	/*  font size for 24 pins			*/
			/*  setup before call setup_memo		*/
	setup_memo();	/*  call for allocate spaces for all_font	*/

	if((oldfon=(char*)malloc(filelen))==NULL)	{
		error_message("Memory not enough");
		return 0;
	}

	if(_read(fd,oldfon,filelen)!=filelen)	{
		error_read();
		return 0;
	}
	close(fd);
	con_24_to_fed(oldfon,all_font);
	free(oldfon);
	cleanup_load_routine();
	return ~0;

}




con_24_to_fed(char *oldfon,char *newfon)
{
    static  int ch,i,bit;
	char *newch=newfon+31*3*24;
	static char *new;
	register char *old=oldfon;

	for(ch=32-32;ch<256-32;ch++)	{
		newch+=3*24;
		for(i=0;i<18;i++)	{
			new=newch+i/8;
			bit=7-i%8;
			if(test_bit(*old,7))	set_bit(new,bit);
			if(test_bit(*old,6))	set_bit(new+3,bit);
			if(test_bit(*old,5))	set_bit(new+6,bit);
			if(test_bit(*old,4))	set_bit(new+9,bit);
			if(test_bit(*old,3))	set_bit(new+12,bit);
			if(test_bit(*old,2))	set_bit(new+15,bit);
			if(test_bit(*old,1))	set_bit(new+18,bit);
			if(test_bit(*old,0))	set_bit(new+21,bit);
			if(test_bit(*(++old),7))	set_bit(new+24,bit);
			if(test_bit(*(old),6))	set_bit(new+27,bit);
			if(test_bit(*(old),5))	set_bit(new+30,bit);
			if(test_bit(*(old),4))	set_bit(new+33,bit);
			if(test_bit(*(old),3))	set_bit(new+36,bit);
			if(test_bit(*(old),2))	set_bit(new+39,bit);
			if(test_bit(*(old),1))	set_bit(new+42,bit);
			if(test_bit(*(old),0))	set_bit(new+45,bit);
			if(test_bit(*(++old),7))	set_bit(new+48,bit);
			if(test_bit(*(old),6))	set_bit(new+51,bit);
			if(test_bit(*(old),5))	set_bit(new+54,bit);
			if(test_bit(*(old),4))	set_bit(new+57,bit);
			if(test_bit(*(old),3))	set_bit(new+60,bit);
			if(test_bit(*(old),2))	set_bit(new+63,bit);
			if(test_bit(*(old),1))	set_bit(new+66,bit);
			if(test_bit(*(old),0))	set_bit(new+69,bit);
			++old;
		}
	}
}

save_24()
{
	char *newfon;

	int fd;

	if((newfon=(char*)malloc(18*3*(256-32)))==NULL)	{
		error_message("Memory not enough");
		return 0;
	}
	memset(newfon,0,18*3*(256-32));

	con_fed_to_24(all_font,newfon);
	if((fd=_creat(fname,FA_ARCH))==-1)	{
		error_message("Error creating");
		return 0;
	}
	if(_write(fd,newfon,18*3*(256-32))==-1)	{
		error_write();
		return 0;
	}
	close(fd);
	free(newfon);
	return ~0;
}

con_fed_to_24(char *buff,char *new)
{
	int ch,i,bit;
	char *old,*oldch=buff+31*3*24;

	for (ch=32-32;ch<256-32;ch++)	{
		oldch+=3*24;

		for(i=0;i<18;i++)	{
			old=oldch+i/8;
			bit=7-i%8;
			if(test_bit(*old,bit))	set_bit(new,7);
			if(test_bit(*(old+3),bit))	set_bit(new,6);
			if(test_bit(*(old+6),bit))	set_bit(new,5);
			if(test_bit(*(old+9),bit))	set_bit(new,4);
			if(test_bit(*(old+12),bit))	set_bit(new,3);
			if(test_bit(*(old+15),bit))	set_bit(new,2);
			if(test_bit(*(old+18),bit))	set_bit(new,1);
			if(test_bit(*(old+21),bit))	set_bit(new,0);
			++new;
			if(test_bit(*(old+24),bit))	set_bit(new,7);
			if(test_bit(*(old+27),bit))	set_bit(new,6);
			if(test_bit(*(old+30),bit))	set_bit(new,5);
			if(test_bit(*(old+33),bit))	set_bit(new,4);
			if(test_bit(*(old+36),bit))	set_bit(new,3);
			if(test_bit(*(old+39),bit))	set_bit(new,2);
			if(test_bit(*(old+42),bit))	set_bit(new,1);
			if(test_bit(*(old+45),bit))	set_bit(new,0);
			++new;
			if(test_bit(*(old+48),bit))	set_bit(new,7);
			if(test_bit(*(old+51),bit))	set_bit(new,6);
			if(test_bit(*(old+54),bit))	set_bit(new,5);
			if(test_bit(*(old+57),bit))	set_bit(new,4);
			if(test_bit(*(old+60),bit))	set_bit(new,3);
			if(test_bit(*(old+63),bit))	set_bit(new,2);
			if(test_bit(*(old+66),bit))	set_bit(new,1);
			if(test_bit(*(old+69),bit))	set_bit(new,0);
			++new;
		}
	}
}


save_prn()
{
	int fd;
	char *prn_fon;

	if ((prn_fon=calloc(44,256))==NULL)	{
		error_message("Error : not enough memory");
		return 0;
	}

	conv_fed_to_22x16(all_font,prn_fon);

	if ((fd=_creat(fname,FA_ARCH))==-1)	{
		error_message("Error : writing font file");
		return 0;
	}
	if (_write(fd,prn_fon,44*256)==-1)	{
		error_message("Error : not enough disk space or disk error");
		return 0;
	}
	_close(fd);
	free(prn_fon);
	return ~0;
}

fed_to_prn_ch(char *buff,char *res)
{
	int i,j;

	for(i=0;i<8;i++)
		for (j=0;j<22;j++)
			if (test_bit(*(buff+j/8+i*6),7-(j%8)))
				set_bit((res+j),7-i);
	for(i=0;i<8;i++)
		for (j=0;j<22;j++)
			if (test_bit(*(buff+j/8+i*6+3),7-(j%8)))
				set_bit((res+j+22),7-i);
}


conv_fed_to_22x16(char *buff,char *result)
{
	int i;

	for (i=0;i<=255;i++)
		fed_to_prn_ch(buff+i*3*16,result+i*44);
}


load_prn()
{
	int fd;
	char *prn_fon;

    if ((prn_fon=malloc((unsigned)(44*256)))==NULL) {
		error_message("Error : not enough memory");
        return 0;
    }
	memset(prn_fon,0,44*256);
    if ((fd=_open(fname,O_RDONLY))==-1) {
		error_message("Error : opening font file");
        return 0;
    }
    if (_read(fd,prn_fon,44*256)==-1)   {
		error_read();
        return 0;
    }
	_close(fd);

	X=22;Y=16;
	setup_memo();

	conv_22x16_to_fed(prn_fon,all_font);

	_close(fd);
	free(prn_fon);
	cleanup_load_routine();
	return ~0;
}


prn_to_fed_ch(char *buff,char *res)
{
	int i,j;
	for (j=0;j<8;j++)
		for (i=0;i<22;i++)
			if(test_bit(*(buff+i),7-j))
				set_bit(res+i/8+j*6,7-i%8);
	for (j=0;j<8;j++)
		for (i=22;i<44;i++)
			if(test_bit(*(buff+i),7-j))
				set_bit(res+(i-22)/8+j*6+3,7-(i-22)%8);
}


conv_22x16_to_fed(char *buff,char *result)
{
	int i;

	for (i=0;i<=255;i++)
		prn_to_fed_ch(buff+i*44,result+i*3*16);
}


load_scr()
{
	int fd;

    X=8;Y=20;
    setup_memo();

    if ((fd=_open(fname,O_RDONLY))==-1)	{
	error_message("Error : opening font file");
	return 0;
    }
    if (_read(fd,all_font,20*256)==-1)	{
	error_read();
	return 0;
    }
    _close(fd);
    cleanup_load_routine();
    return ~0;
}

save_scr()
{
	int fd;

    if ((fd=_creat(fname,FA_ARCH))==-1)	{
	error_message("Error : creating font file");
	return 0;
    }
    if (_write(fd,all_font,20*256)==-1)	{
	error_message("Error : not enough disk space or disk error");
	return 0;
    }
    _close(fd);
    return ~0;
}

load_sup_sub()
{
	char *oldfon;

	int fd,filelen;
	if((fd=_open(fname,O_RDONLY))==-1)	{
		error_message("Open reading error");
		return 0;
	}
	if((filelen=filelength(fd))!=2*18*(256-32))	{
		error_message("Invalid font file!");
		return 0;
	}
	if((oldfon=(char*)malloc(filelen))==NULL)	{
		error_message("Memory not enough");
		return 0;
	}
	X=18;Y=12;
	setup_memo();

	if(_read(fd,oldfon,filelen)==-1)	{
		error_read();
		return 0;
	}
	close(fd);
	con_sup_to_fed(oldfon,all_font);
	free(oldfon);
	cleanup_load_routine();
	return ~0;
}

con_sup_to_fed_chr(char *oldch,char *newch)
{
	int i,j;
	for(i=0;i<18;i++)
		for(j=4;j<15;j++)
			if(test_bit(*(oldch+(j/8)+i*2),7-j%8))
				set_bit(newch+(j-4)*3+(i/8),7-i%8);
}


con_sup_to_fed(char *oldfon,char *newfon)
{
	int ch;
	for(ch=32-32;ch<256-32;ch++)
		con_sup_to_fed_chr(oldfon+ch*18*2,newfon+(32+ch)*3*12);
}

save_sup_sub()
{
	char *newfon;

	int fd;
	if((newfon=(char*)malloc(18*2*(256-32)))==NULL)	{
		error_message("Memory not enough");
		return 0;
	}
	memset(newfon,0,18*2*(256-32));

	con_fed_to_sup(all_font,newfon);
	if((fd=_creat(fname,FA_ARCH))==-1)	{
		error_message("Error creating");
		return 0;
	}
	if(write(fd,newfon,18*2*(256-32))==-1)	{
		error_write();
		return 0;
	}
	close(fd);
	free(newfon);
	return ~0;
}

con_fed_to_sup_chr(char *oldch,char *newch)
{
	int i,j;
	for(i=0;i<18;i++)
		for(j=4;j<16;j++)
			if(test_bit( *(oldch+(j-4)*3+(i/8)),7-i%8))
				set_bit(newch+(j/8)+i*2,7-j%8);
}


con_fed_to_sup(char *oldfon,char *newfon)
{
	int ch;
	for(ch=32-32;ch<256-32;ch++)
		con_fed_to_sup_chr(oldfon+(ch+32)*12*3,newfon+ch*2*18);
}
