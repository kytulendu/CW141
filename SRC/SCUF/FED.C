#include <math.h>
#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
/* update: Suttipong Kanakakorn
   Sun  07-30-1989  20:11:04
   Thu  08-03-1989  10:13:44
*/

#include "inc.h"
#include "..\common\cwgrphc.h"
#include "write.h"
#include "key.h"
#include "fed.h"
#include "getch.h"
#include "popup.h"

int first_file=1,new_load=0;
char cufontpath[127];
char cuwordpath[127];
char cuprintpath[127];

int draw_mode=0;
int load_first=1;
int align=0;
int loaded=0;

char *char_block_image;

char X;          /*      Size of char        */
char Y;

unsigned sel_x=0,sel_y=0;

char WIDTH_X = 6;    /*      Size of grid        */
char WIDTH_Y = 4;

int center_x,center_y;
char *dot_image,*square_image,*matrix_image,*white_square_image;

char *all_font;             /*      keep all font       */

char edit_font[MAX_X][MAX_Y];       /*      keep editing font   */
                                    /*      bit by bit          */
char backup_font[MAX_X][MAX_Y];

unsigned int pos_x,pos_y,BYTE;
int lower_limit;

char fname[MAXPATH];

/* ======================================================*/
/* function prototype, add by Suttipong Kanakakorn
                       Sat  08-26-1989  01:47:11
*/
void setup_screen(void);
void init_graph_and_cursor(void);
void fed(void);
int _ceil(int x,int y);
void clearworkarea(void);
void invert(void);
void mirror(void);
void backup_char(void);
void undo(void);
void del_col(char col);
void insert_col(char col);
void del_row(char row);
void insert_row(char row);
void pan_left(void);
void pan_right(void);
void pan_up(void);
void pan_down(void);
void draw_char(void);
void update_char (char ch);
void take_char(char ch);
void blink(unsigned x, unsigned y);
void put_dot(unsigned x, unsigned y);
void xor_dot(unsigned x, unsigned y);
void square(unsigned x, unsigned y);
void draw_grid(unsigned x,unsigned y);
void error_write(void);
void error_read(void);
int char_per_row(void);
int char_per_col(void);
void print_page(int pageno,int char_per_page);
void draw_frame(int x1,int y1,int x2,int y2);
void make_char_block_image(void);
void calculate_page_frame(int *x1,int *y1,int *x2,int *y2);
void print_table(int page);
int which_page(int ch);
void insert_char(int ch);
void delete_char(int ch);
void or_char(int ch1,int ch2);
int select_font(int *page,char *status);
void help(void);
void help_select(void);
void edit(int ch);
void setup_memo(void);
void free_all(void);
void terminate(void);
void file_menu(void);
void main_menu(void);
/* ======================================================*/
int main(int argc,char *argv[])
{
    cufsetup(argc, argv);
    setup_screen();
    fed();
    return 0;
}

int (*writech)(int ,int ,char );

/*
        /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
                setvideomode
                init_cursor
        \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
*/
void setup_screen(void)
{
    extern int herc_align;

    registerfarbgidriver (EGAVGA_driver_far);
    registerfarbgidriver (Herc_driver_far);

    setgraph();
    if (scrmode == HERCMONO) {
        writech = hgcwritech;
        if (herc_align)
            align = 40;
        /* else align = 0 (init already) */
    } else {
        writech = ewritech;
        herc_align = 0;
    }

    init_graph_and_cursor();

    clsall();
    /*
    box(0+align,0,639+align,347);
    */
    box(0,0,639,347);
    prakeaw();
    dispstrhgc("จุฬาลงกรณ์มหาวิทยาลัย",6,0,BOLDATTR);
    /*
    _line(0+align,24,639+align,24);
    */
    _line(0,24,639,24);
}

/*
    malloc square_image,white_square_image,dot_image
    init_clock
*/
void init_graph_and_cursor(void)
{
    int graphdriver,graphmode;
    extern void flush_clock(void);

    if (scrmode == EGA || scrmode == EGA64) {
       graphdriver = EGA64;
       graphmode = EGA64HI;
    } else if (scrmode == EGAMONO) {
       graphdriver = EGAMONO;
       graphmode = EGAMONOHI;
    } else {
       graphdriver = HERCMONO;
       graphmode = HERCMONOHI;
    }
    initgraph(&graphdriver,&graphmode,"");
    /*
    if (scrmode == EGAMONO)
        esetgraph(); /* use 2 plains only (TC use 4 plains) */
    */
    setfillstyle(SOLID_FILL,WHITE);
    moveto(0,0);lineto(2+WIDTH_X,0);lineto(WIDTH_X+2,WIDTH_Y+2);
    lineto(0,WIDTH_Y+2);lineto(0,0);
    square_image=(char *)malloc(imagesize(0,0,WIDTH_X+2,WIDTH_Y+2));
    getimage(0,0,WIDTH_X+2,WIDTH_Y+2,square_image);
    floodfill(1,1,-1);
    white_square_image=(char *)malloc(imagesize(0,0,WIDTH_X+2,WIDTH_Y+2));
    getimage(0,0,WIDTH_X+2,WIDTH_Y+2,white_square_image);
    setcolor(0);
    moveto(0,0);lineto(2+WIDTH_X,0);lineto(WIDTH_X+2,WIDTH_Y+2);
    lineto(0,WIDTH_Y+2);lineto(0,0);
    dot_image=(char *)malloc(imagesize(0,0,WIDTH_X+2,WIDTH_Y+2));
    getimage(0,0,WIDTH_X+2,WIDTH_Y+2,dot_image);
    setcolor(1);
    clearviewport();
    init_clock();
    atexit (flush_clock);
}

void fed(void)
{
    cufontpath[0]='\0';
    cuwordpath[0]='\0';
    cuprintpath[0]='\0';

    matrix_image=(char*)malloc(1);
    char_block_image=(char*)malloc(1);
    center_x=320+align;
    center_y=347/2;
    main_menu();
}

int _ceil(int x,int y)
{
    return ((x/y)+((x%y)?1:0));
}

void clearworkarea(void)
{
    struct viewporttype t;
    getviewsettings(&t);
    setviewport(1+align,25,638+align,346,0);
    clearviewport();
    setviewport(t.left,t.top,t.right,t.bottom,t.clip);
}

void invert(void)
{
        int i,j;
        for(i=0;i<X;i++)
                for(j=0;j<Y;j++)
                        edit_font[i][j] = ~edit_font[i][j];
}

void mirror(void)
{
        char tmp[MAX_X/2][MAX_Y];
        int i,j;
        for(i=0;i<X/2;i++)
                for(j=0;j<Y;j++)
                        tmp[i][j]=edit_font[X-i-1][j];
        for(i=0;i<X/2;i++)
                for(j=0;j<Y;j++)
                        edit_font[X-i-1][j]=edit_font[i][j];
        for(i=0;i<X/2;i++)
                for(j=0;j<Y;j++)
                        edit_font[i][j]=tmp[i][j];
}

void backup_char(void)
{
        int i,j;
        for(i=0;i<X;i++)
                for(j=0;j<Y;j++)
                        backup_font[i][j]=edit_font[i][j];
}

void undo(void)
{
        int i,j;
        for(i=0;i<Y;i++)
                for(j=0;j<X;j++)
                        edit_font[j][i]=backup_font[j][i];
}

void del_col(char col)
{
        int i,j;
        for(i=0;i<Y;i++)
                for(j=col;j<X-1;j++)
                        edit_font[j][i]=edit_font[j+1][i];
        for(i=0;i<Y;i++)
                edit_font[j][i]=0;
}

void insert_col(char col)
{
        int i,j;
        for(i=0;i<Y;i++)
                for(j=X-1;j>col;j--)
                        edit_font[j][i]=edit_font[j-1][i];
        for(i=0;i<Y;i++)
                edit_font[j][i]=0;
}

void del_row(char row)
{
        int i,j;
        for(i=row;i<Y-1;i++)
                for(j=0;j<X;j++)
                        edit_font[j][i]=edit_font[j][i+1];
        for(j=0;j<X;j++)
                edit_font[j][i]=0;
}

void insert_row(char row)
{
        int i,j;
        for(i=Y-1;i>row;i--)
                for(j=0;j<X;j++)
                        edit_font[j][i]=edit_font[j][i-1];
        for(j=0;j<X;j++)
                edit_font[j][row]=0;
}

void pan_left(void)
{
        del_col(0);
}

void pan_right(void)
{
        insert_col(0);
}

void pan_up(void)
{
        del_row(0);
}

void pan_down(void)
{
        insert_row(0);
}

void draw_char(void)
{
        static int first=1;
        int i,j;
        if (load_first)         {
                first=1;
                load_first=0;
        }
        if (!first)
                putimage(0+align,0,matrix_image,COPY_PUT);
        else    {
                draw_grid(X,Y);
                matrix_image=(char *)realloc(matrix_image,
                imagesize(0+align,0,(WIDTH_X+2)*X+align,(WIDTH_Y+2)*Y));
                getimage(0+align,0,(WIDTH_X+2)*X+align,
                        (WIDTH_Y+2)*Y,matrix_image);
        }

        for(j=0;j<Y;j++)
                for(i=0;i<X;i++)
                        if(edit_font[i][j])     put_dot(i,j);
                        else
                                if(first)       square(i,j);
        first=0;
}

void update_char (char ch)
{
    char *i;
        int j,m,l,k=0,n;
    for (i=all_font+(ch*BYTE*Y),j=0;j<Y;j++)    {
        m = X / 8 ;     l = 0;
        while ( l < m*8 )       {
                *(i+k) = 0 ;
                *(i+k) |= (edit_font[l++][j]) ? 0x80 : 0x00;
                *(i+k) |= (edit_font[l++][j]) ? 0x40 : 0x00;
                *(i+k) |= (edit_font[l++][j]) ? 0x20 : 0x00;
                *(i+k) |= (edit_font[l++][j]) ? 0x10 : 0x00;
                *(i+k) |= (edit_font[l++][j]) ? 0x08 : 0x00;
                *(i+k) |= (edit_font[l++][j]) ? 0x04 : 0x00;
                *(i+k) |= (edit_font[l++][j]) ? 0x02 : 0x00;
                *(i+k) |= (edit_font[l++][j]) ? 0x01 : 0x00;
                k++;
        }
        n = X % 8 ;
        if ( n != 0 )   {
                *(i+k) = 0 ;
                switch (l=n+m*8-1 , n )         {
                        case 7 : *(i+k) |= (edit_font[l--][j]) ? 0x02 : 0x00;
                        case 6 : *(i+k) |= (edit_font[l--][j]) ? 0x04 : 0x00;
                        case 5 : *(i+k) |= (edit_font[l--][j]) ? 0x08 : 0x00;
                        case 4 : *(i+k) |= (edit_font[l--][j]) ? 0x10 : 0x00;
                        case 3 : *(i+k) |= (edit_font[l--][j]) ? 0x20 : 0x00;
                        case 2 : *(i+k) |= (edit_font[l--][j]) ? 0x40 : 0x00;
                        case 1 : *(i+k) |= (edit_font[l--][j]) ? 0x80 : 0x00;
                        k++;
                }
        }
    }
}

void take_char(char ch)
{
    char *i;
        int j,m=X/8,k=0,l,n=X%8;
    for (i=all_font+(ch*BYTE*Y),j=0;j<Y;j++)    {
                l = 0;
                while ( l < m*8 )       {
                        edit_font[l++][j] = (*(i+k) & 0x80) ? ~0 : 0;
                        edit_font[l++][j] = (*(i+k) & 0x40) ? ~0 : 0;
                        edit_font[l++][j] = (*(i+k) & 0x20) ? ~0 : 0;
                        edit_font[l++][j] = (*(i+k) & 0x10) ? ~0 : 0;
                        edit_font[l++][j] = (*(i+k) & 0x08) ? ~0 : 0;
                        edit_font[l++][j] = (*(i+k) & 0x04) ? ~0 : 0;
                        edit_font[l++][j] = (*(i+k) & 0x02) ? ~0 : 0;
                        edit_font[l++][j] = (*(i+k) & 0x01) ? ~0 : 0;
                        k++;
                }
                if ( n != 0 )   {
                        switch (l=m*8+n-1 , n )         {
                                case 7 : edit_font[l--][j] = (*(i+k)&0x02) ? ~0 : 0;
                                case 6 : edit_font[l--][j] = (*(i+k)&0x04) ? ~0 : 0;
                                case 5 : edit_font[l--][j] = (*(i+k)&0x08) ? ~0 : 0;
                                case 4 : edit_font[l--][j] = (*(i+k)&0x10) ? ~0 : 0;
                                case 3 : edit_font[l--][j] = (*(i+k)&0x20) ? ~0 : 0;
                                case 2 : edit_font[l--][j] = (*(i+k)&0x40) ? ~0 : 0;
                                case 1 : edit_font[l--][j] = (*(i+k)&0x80) ? ~0 : 0;
                                k++;
                        }
                }
    }
}

void blink(unsigned x, unsigned y)
{
        static int i;
        putimage(align+x*(2+WIDTH_X),y*(2+WIDTH_Y),dot_image,XOR_PUT);
        for(i=0;i<4000;i++);
        putimage(align+x*(2+WIDTH_X),y*(2+WIDTH_Y),dot_image,XOR_PUT);
}

void put_dot(unsigned x, unsigned y)
{
    putimage(align+x*(2+WIDTH_X),y*(2+WIDTH_Y),white_square_image,COPY_PUT);
}

void xor_dot(unsigned x, unsigned y)
{
    putimage(align+x*(2+WIDTH_X),y*(2+WIDTH_Y),dot_image,XOR_PUT);
    edit_font[x][y]=~edit_font[x][y];
}

void square(unsigned x, unsigned y)
{
        putimage(align+x*(2+WIDTH_X),y*(2+WIDTH_Y),square_image,COPY_PUT);
}

void draw_grid(unsigned x,unsigned y)
{
    unsigned  i,j;
    for(i=0;i<y;i++)
                for(j=0;j<x;j++)
                        square(j,i);
}

void error_write(void)
{
        error_message("ERROR : WRITING  FONT FILE");
}

void error_read(void)
{
        error_message("ERROR : READING  FONT FILE");
}

int char_per_row(void)
{
        return 640/8/BYTE;
}

int char_per_col(void)
{
        return (300) / (4* ( _ceil ((Y),4)));
}

void print_page(int pageno,int char_per_page)
{
        int x,y,z=_ceil((Y),4);

        pageno*=char_per_page;

        for (x=0;x<79;x+=BYTE)
                for (y=0;y<(lower_limit/4-10);y+=z)     {
                        if(pageno>255)  return;
                        /*
                        writech(x,y+10,pageno++);
                        */
                        writech(x+2,y+10,pageno++);
                }
}

void draw_frame(int x1,int y1,int x2,int y2)
{
    struct linesettingstype tmp;
    getlinesettings(&tmp);
    setlinestyle(SOLID_LINE,0,THICK_WIDTH);setwritemode(0);
    y1+=4;
#ifdef NOALIGN
    x1 += 40; x2 += 40;
#else
    x1 += align; x2 += align;
#endif
    moveto(x1,y1);
    lineto(x2,y1);lineto(x2,y2);
    lineto(x1,y2);lineto(x1,y1);
    moveto(x1+1,y1+1);
    setlinestyle(tmp.linestyle,tmp.upattern,tmp.thickness);
}

void make_char_block_image(void)
{
        setcolor(1);
        moveto(4+align,30);lineto(X+3+align,30);lineto(X+3+align,Y+29);
        lineto(4+align,Y+29);lineto(4+align,30);
        char_block_image=(char*)realloc(char_block_image,
                imagesize(4+align,30,X+3+align,Y+29));
        getimage(4+align,30,X+3+align,Y+29,char_block_image);
        putimage(4+align,30,char_block_image,XOR_PUT);
}

void calculate_page_frame(int *x1,int *y1,int *x2,int *y2)
{
        *x1=8;*y1=28;
        *x2=256/char_per_col()*BYTE*8+BYTE*8+10+*x1;
        *y2=char_per_col()*Y+6+*y1+8;
}

void print_table(int page)
{
        int x1,y1,x2,y2;
        calculate_page_frame(&x1,&y1,&x2,&y2);
        draw_frame(x1,y1,x2,y2);
        lower_limit=y2;
        print_page(page,char_per_row() * char_per_col());
}

int which_page(int ch)
{
        return (char_per_row() * char_per_col() > ch) ? 0 : 1;
}

void insert_char(int ch)
{
        memmove(all_font+(ch+1)*BYTE*Y,all_font+ch*BYTE*Y,(255-ch)*BYTE*Y);
        memset(all_font+ch*BYTE*Y,0,BYTE*Y);
        print_table(which_page(ch));
}


void delete_char(int ch)
{
        memcpy(all_font+ch*BYTE*Y,all_font+(ch+1)*BYTE*Y,(255-ch)*BYTE*Y);
        print_table(which_page(ch));
}

void or_char(int ch1,int ch2)
{
        char *i1=all_font+ch1*BYTE*Y,*j1=i1+BYTE*Y,
                *i2=all_font+ch2*BYTE*Y;
        for(;i1<j1;i1++)        {
                *i1|=*i2;
                i2++;
        }
}


int select_font(int *page,char *status)
{
        static int ch=0;
        char tmp;
        int dum;
        extern int cur_char;
        unsigned maxx,maxy;
        int save=get_active_clock();
        static new_page=-1;
        char key,ch2;

        start_clock(SELECT_FONT);

        maxx=256/char_per_col();
        maxy=char_per_col();
        cur_char=ch;
        while ( !((key=get_pos(&sel_x,&sel_y,maxx,maxy-1,1,1)) ==RET ||
                        key == 3))      {
                ch=sel_y+sel_x*char_per_col();
                if (ch>255)     {
                        sel_x=maxx;
                        sel_y=char_per_col()-((char_per_col()*(1+sel_x))-255);
                        ch=255;
                };
                cur_char=ch;
                if(new_page==-1)
                        switch(tolower(key))    {
                                case 'o' :      new_page=0;
                                                ch2=select_font(&dum,&tmp);
                                                if(*status==RET)        {
                                                        or_char(ch,ch2);
                                                        print_table(which_page(ch));
                                                }
                                                new_page=-1;
                                                break;
                                case '?' :      help_select();break;
                                case 1 : insert_char(ch);break;
                                case 2 : delete_char(ch);break;
                        }
        }
        start_clock(save);
        *status=key;
        return ch;
}

void help(void)
{
    static char *editing_menu[]    =   {
                                                "Editing Menu",
                                                "<ESC> to exit and make change",
                                                "<Insert> to toggle dot,undot",
                                                "<M>irror  <I>nvert",
                                                "<U>ndo",
                                                "<R>ow insert <E>rase row",
                                                "<C>olumninsert <D>eletecolumn",
                                                "'.' change mode",
                                                "Arrow key to move cursor",
                                                "Shift+Arrow key",0};
        popup_message(editing_menu);
}

void help_select(void)
{
    static char *select_menu[]    =   {
                                                "Select font Menu",
                                                "Arrow key to move cursor",
                                                "<Ins> to insert a blank character",
                                                "<Del> to delete character",
                                                "'O' to OR first char with the seconds",
                                                "<ESC> to exit",
                                                0};
        popup_message(select_menu);
}


void edit(int ch)
{
        struct viewporttype tmp;
        char key,*buff;

    take_char((char)ch);
    backup_char();
    getviewsettings(&tmp);
    setviewport(400,30,getmaxx(),getmaxy(),1);
    buff=(char*)malloc(imagesize(0+align,0,X*(2+WIDTH_X)+align,Y*(2+WIDTH_Y)));
    getimage(0+align,0,X*(2+WIDTH_X)+align,Y*(2+WIDTH_Y),buff);
    draw_char();
    start_clock(EDIT_FONT);

    while((key=get_pos(&pos_x,&pos_y,X-1,Y-1,1,1))!=3)    {
        switch(tolower(key)) {
            case 1      :       xor_dot(pos_x,pos_y);
                                        continue;

            case '8'    :       pan_up();
                                                draw_char();
                                                break;

            case '2'    :       pan_down();
                                                draw_char();
                                                break;

            case '4'    :       pan_left();
                                                draw_char();
                                                break;

            case '6'    :       pan_right();
                                                draw_char();
                                                break;

            case 'r'    :       insert_row(pos_y);
                                                draw_char();
                                                break;

            case 'c'    :       insert_col(pos_x);
                                                draw_char();
                                                break;

            case 'e'    :       del_row(pos_y);
                                                draw_char();
                                                break;

            case 'd'    :       del_col(pos_x);
                                                draw_char();
                                                break;

            case 'i'    :       invert();
                                                draw_char();
                                                break;

            case 'm'    :       mirror();
                                                draw_char();
                                                break;

            case '.'    :       ++draw_mode;
                                switch(draw_mode)       {
                                        case 0 : break;
                                        case 1 : put_dot(pos_x,pos_y);
                                                 edit_font[pos_x][pos_y]=~0;
                                                 break;
                                        case 2 : square(pos_x,pos_y);
                                                 edit_font[pos_x][pos_y]=0;
                                                 break;
                                        case 3 : draw_mode = 0;
                                }
                                continue;

                case 'u'        :       undo();
                                draw_char();
                                goto exit;

                case '?'        :       help();
                                                break;

            default     :       if (draw_mode==1)       {
                                        put_dot(pos_x,pos_y);
                                        edit_font[pos_x][pos_y]=~0;
                                };
                                if (draw_mode==2)       {
                                        square(pos_x,pos_y);
                                        edit_font[pos_x][pos_y]=0;
                                }

        }
    }
    update_char((char)ch);
exit:
        putimage(0+align,0,buff,COPY_PUT);
        free(buff);
        stop_clock();
        dispblank(50,16,strlen("Move mode"),0);
        setviewport(tmp.left,tmp.top,tmp.right,tmp.bottom,tmp.clip);
        print_table(which_page(ch));
}

void setup_memo(void)
/*
        /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
                                draw grid
                                malloc all_font
                                set BYTE        =       sizeof X in bytes
        \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
*/
{
        static int first=1;


        BYTE=BYTE_X(X);
        if(first)       {
                if((all_font=(char *)malloc(256*BYTE*Y))==NULL)         {
                        error_message("Not enough memory!");
                        exit(0);
                }
                first=0;
        }       else    {
                if((all_font=(char*)realloc(all_font,256*BYTE*Y))==NULL)        {
                        error_message("Not enough memory!");
                        exit(0);
                }
        }
        memset(all_font,0,256*BYTE*Y);
}


void free_all(void)
{
        free(dot_image);
        free(square_image);
        free(white_square_image);
        free(all_font);
        free(matrix_image);
        free(char_block_image);
}

void terminate(void)
{
        free_all();
        restorecrtmode();
        closegraph();
        settext ();
}

#define NOTSAVEPROMPT "File not saved: Save it ? "

void file_menu(void)
{
        static saved=1;
        char *buff,ans[2]="y";
        static char *menu[]     ={
                "Main Menu",
                " 1.load screen font",
                " 2.load italic screen font",
                " 3.load 9 pins font",
                " 4.load italic 9 pins font",
                " 5.load 24 pins font",
                " 6.load italic 24 pins font",
                " 7.load super&subscript font",
                " 8.load italic superscript",
                " 9.load .FED file",
                "10.save font file",
                "11.continue working",
                "12.exit",
                0};
while(1)        {
        switch(popup_menu(menu,&buff))  {
                case 0: new_load=SCR_FONT;
                        break;
                case 1: new_load=ITALIC_SCR_FONT;
                        break;
                case 2: new_load=PRN_FONT;
                        break;
                case 3: new_load=ITALIC_PRN_FONT;
                        break;
                case 4: new_load=LQ_FONT;
                        break;
                case 5: new_load=ITALIC_LQ_FONT;
                        break;
                case 6: new_load=SUP_SUB_LQ_FONT;
                        break;
                case 7:         new_load=ITALIC_SUP_SUB_LQ_FONT;
                        break;
                case 8: new_load=FED_FONT;
                        break;
                case 9: if(loaded)      {
                                if(save_font())
                                        saved=!0;
                        }       else    {
                                error_message("You haven't load font yet!");
                        }
                        remove_menu(&buff);
                        continue;
                case 10:
                        if(first_file)  {
                                error_message("You haven't load font yet!");
                                remove_menu(&buff);
                                continue;
                        }
                        remove_menu(&buff);
                        new_load=0;
                        first_file=0;
                        return;
                case 11: if(!saved)     {
                                switch(immediatebox(40,10,NOTSAVEPROMPT,ans))   {
                                        case 1:         if(save_font())         {
                                                        terminate();
                                                        exit(0);
                                                }       else    {
                                                        remove_menu(&buff);
                                                        continue;
                                                }
                                        case 0:         terminate();
                                                exit(0);
                                        case -1:remove_menu(&buff);
                                                return;
                                }
                        }       else    {
                                terminate();
                                exit(0);
                        }
        }
        if(new_load)    {
                if(!saved)      {
                        switch(immediatebox(40,10,NOTSAVEPROMPT,ans))   {
                                case 1:         if(save_font())         {
                                                saved=!0;
                                        }       else    {
                                                remove_menu(&buff);
                                                new_load=0;
                                                continue;
                                        }
                                        break;
                                case 0:         break;
                                case -1:remove_menu(&buff);
                                        new_load=0;
                                        continue;
                        }
                }
                if(load_font())         {
                        clearworkarea();
                        print_table(0);
                        loaded=new_load;
                        new_load=0;
                        first_file=0;
                        saved=0;
                        free(buff);
                        return;
                }       else    {
                        remove_menu(&buff);
                        continue;
                }
        }
}
}


void main_menu(void)
{
        static int pageno=0;char status;
        char ch;
        file_menu();
        while(1)        {
                ch=select_font(&pageno,&status);
                switch(status)  {
                        case RET:       edit(ch);break;
                        case 3  :       file_menu();
                }
        }
}
