/*
Update: Suttipong Kanakakorn
        Sun  07-30-1989  19:50:45
*/

#include <stdlib.h>
#include <alloc.h> /* for farcoreleft */

#include "inc.h"
#include "convert.h"
#include "..\common\cwgrphc.h"

extern int last_menu_on;

/****************************************************************************/
/*  Change code  from keyboard to code of menu.                             */
/****************************************************************************/
unsigned key_to_menu(unsigned key)
{
    int i;
    for (i=1;(command_tab[i] != key) && (command_tab[i] != 0);i+=2);
    return(command_tab[i-1]);
}

/****************************************************************************/
/*  Select menu.                                                            */
/*  input : number origin 1                                                 */
/*          *curmenu origin 1                                               */
/****************************************************************************/

static int special_char = 0;

int selectmenu(unsigned x,unsigned y,unsigned number,
                char *message[],unsigned *curmenu)
{
    int c,length,i,out;
    int special_base;  /****** Modified ********/

    length = thaistrlen(message[0]);

    special_base = 0;
    for (i = 0;i < number;i++) {
      dispstrhgc(message[i],x,y+i,0);
    }

    dispstrhgc(message[(*curmenu) - 1],x,y+(*curmenu)-1,REVERSEATTR);
    box(x*8 - 2,y*20 + 2,(x+length-1)*8 + 9,(y+number)*20 + 5);
    for (;;)    {
      out = YES;
      c = ebioskey(0);
      switch(c) {
      case UPKEY  : dispstrhgc(message[(*curmenu) + special_base - 1],x,y+(*curmenu)-1,0);
                    if (*curmenu != 1)
                       (*curmenu)--;
                    else
                     if (!special_char)        /**** Modified *****/
                       *curmenu = number;
                     else
                     {
                       if (special_base > 0)
                       {
                        special_base --;
                        for (i = 0;i < number;i++) {
                              dispstrhgc(message[i+special_base],x,y+i,0);
                        }
                       }
                     }
                    dispstrhgc(message[(*curmenu) + special_base - 1],x,y+(*curmenu)-1,REVERSEATTR);
                    break;
      case DNKEY  : dispstrhgc(message[(*curmenu) + special_base -1],x,y+(*curmenu)-1,0);
                    if (*curmenu != number)
                       (*curmenu)++;
                    else
                     if (!special_char)         /**** Modified *****/
                       *curmenu = 1;
                     else
                     {
                       if (special_base+(*curmenu) < 18)
                       {
                        special_base++;
                        for (i = 0;i < number;i++) {
                              dispstrhgc(message[i+special_base],x,y+i,0);
                        }
                       }
                      }
                    dispstrhgc(message[(*curmenu) + special_base - 1],x,y+(*curmenu)-1,REVERSEATTR);
                    break;
      case LEKEY  :
      case RIKEY  :
      case ESCKEY :
      case RETKEY :
                     if (special_char)
                        (*curmenu) += special_base;

                     return(c);
      case CNTRL_K : c = ebioskey(0) & 0xff;
                     if (c >= 32) c = _toupper(c);
                     switch(c) {
                     case 19  :
                     case 'S' : *curmenu = 0x1300;
                                break;
                     case 16  :
                     case 'P' : *curmenu = 0x1400;
                                break;
                     case 2   :
                     case 'B' : *curmenu = 0x6100;
                                break;
                     case 11  :
                     case 'K' : *curmenu = 0x6200;
                                break;
                     case 3   :
                     case 'C' : *curmenu = 0x6300;
                                break;
                     case 25  :
                     case 'Y' : *curmenu = 0x6400;
                                break;
                     case 22  :
                     case 'V' : *curmenu = 0x6500;
                                break;
                     case 18  :
                     case 'R' : *curmenu = 0x6600;
                                break;
                     case 23  :
                     case 'W' : *curmenu = 0x6700;
                                break;
                     case 8   :
                     case 'H' : *curmenu = 0x6800;
                                break;
                     default  : out = NO;
                                break;
                     }
                     if (out == YES)
                       return(0);
                     break;
      case CNTRL_O : c = ebioskey(0) & 0xff;
                     if (c >= 32) c = _toupper(c);
                     switch(c) {
                     case 12  :
                     case 'L' : *curmenu = 0x7100;
                                break;
                     case 18  :
                     case 'R' : *curmenu = 0x7200;
                                break;
                     case 9   :
                     case 'I' : *curmenu = 0x7300;
                                break;
                     case 14  :
                     case 'N' : *curmenu = 0x7400;
                                break;
                     case 3   :
                     case 'C' : *curmenu = 0x7500;
                                break;
                     case 16  :
                     case 'P' : *curmenu = 0x7600;
                                break;
                     case 24  :
                     case 'X' : *curmenu = 0x7700;
                                break;
                     default  : out = NO;
                                break;
                     };
                     if (out == YES)
                       return(0);
                     break;
      case CNTRL_Q : c = ebioskey(0) & 0xff;
                     if (c >= 32) c = _toupper(c);
                     switch(c) {
                     case 25  :
                     case 'Y' : *curmenu = 0x3500;
                                break;
                     case 16  :
                     case 'F' : *curmenu = 0x5100;
                                break;
                     case 1   :
                     case 'A' : *curmenu = 0x5200;
                                break;
                     case 12  :
                     case 'L' : *curmenu = 0x8500;
                                break;
                     default  : out = NO;
                                break;
                     };
                     if (out == YES)
                       return(0);
                     break;
      case ALTX    :
      case CNTRL_I :
      case ALTP    :
      case ALTL    :
      case CNTRL_G :
      case BSKEY   :
      case CNTRL_T :
      case CNTRL_Y :
      case CNTRL_N :
      case CNTRL_V :
      case F1KEY   :
      case F2KEY   :
      case F3KEY   :
      case F4KEY   :
      case F5KEY   :
      case F6KEY   :
      case F7KEY   :
      case F8KEY   :
      case F9KEY   :
      case F10KEY  :
      case CNTRL_L :
      case ALTG    :
      case ALTD    :
      case CNTRL_B :
      case ALTM    : if ((i = key_to_menu(c)) != 0) {
                       *curmenu = i;
                       return(0);
                     } else {
                       errorsound();
                     }
                     break;
      }
    }
}

char *menu1lev1[] = {
      "  สร้างแฟ้มข้อมูลใหม่       ",
      "  แก้ไขแฟ้มข้อมูลเก่า       ",
      "  จัดเก็บแฟ้มข้อมูล    ^KS  ",
      "  พิมพ์แฟ้มข้อมูล      ^KP  ",
      "  เปลี่ยนช่องเก็บแฟ้มข้อมูล   ",
      "  เปลี่ยนราก(Directory)  ",
      "  ไประบบปฏิบัติการ        ",
      "  เลิกการทำงาน   ALT-X  ",
      };
char *menu2lev1[] = {
      "  เลื่อนตำแหน่ง          ",
      "  เลื่อนจอภาพ           ",
      "  ไปยังตำแหน่งเริ่มต้น     ",
      "  ไปยังตำแหน่งสุดท้าย     ",
      "  ไปยังตำแหน่ง TAB  ^I  ",
      "  ไปหน้าที่...    ALT-P  ",
      "  ไปบรรทัดที่...  ALT-L  ",
      };
char *menu3lev1[] = {
      "  ลบตัวอักษร         ^G  ",
      "  ลบอักษรทางซ้าย BackSp  ",
      "  ลบคำ             ^T  ",
      "  ลบบรรทัด          ^Y  ",
      "  ลบไปจนหมดบรรทัด  ^QY  ",
      "  แทรกบรรทัด        ^N  ",
      "  พิมพ์แทรก/พิมพ์ทับ    ^V  ",
      };
char *menu4lev1[] = {
      "  ตัวอักษรปกติ       F1  ",
      "  ตัวเอียง          F2  ",
      "  ขีดเส้นใต้ ๑ เส้น   F3  ",
      "  ขีดเส้นใต้ ๒ เส้น   F4  ",
      "  ตัวหนา           F5  ",
      "  ตัวใหญ่      F6  ",
      "  ตัวยกขึ้น          F7  ",
      "  ตัวห้อย           F8  ",
      "  ไทย/ENGLISH    F10  ",
      "  สัญลักษณ์พิเศษ          ",
      "  ออกแบบตัวอักษร...     ",
      };
char *menu5lev1[] = {
      "  ค้นหาคำ          ^QF  ",
      "  ค้นหาและเปลี่ยนคำ  ^QA  ",
      "  ค้นหาคำต่อไป       ^L  ",
      };
char *menu6lev1[] = {
      "  กำหนดตำแหน่งต้นบล็อก   ^KB  ",
      "  กำหนดตำแหน่งท้ายบล็อก  ^KK  ",
      "  คัดลอกบล็อก (Copy)    ^KC  ",
      "  ลบบล็อก              ^KY  ",
      "  เคลื่อนย้ายบล็อก (moVe) ^KV  ",
      "  อ่านบล็อก (Read)      ^KR  ",
      "  จัดเก็บบล็อก (Write)   ^KW  ",
      "  ซ่อน/แสดงบล็อก        ^KH  ",
      };
char *menu7lev1[] = {
      "  ตั้งกั้นหน้า     ^OL  ",
      "  ตั้งกั้นหลัง     ^OR  ",
      "  ตั้ง TAB      ^OI  ",
      "  ลบ TAB      ^ON  ",
      "  จัดกึ่งกลางหน้า ^OC  ",
      "  จัดบรรทัด/หน้า ^OP  ",
      "  ปล่อยขอบ     ^OX  ",
      };
/*
char *menu8lev1[] = {
      "  อ่านแฟ้มรูปภาพ  ALT-G ",
      "  ลบรูปภาพ      ALT-D ",
      };
*/
char *menu9lev1[] = {
      "  จัดย่อหน้า (Reform)     ^B  ",
      "  ตัดคำ                 F9  ",
      "  แทรกรหัสการพิมพ์        ^P  ",
      "  สร้างตาราง                ",
      "  Restore Line        ^QL  ",
      "  คำย่อ              ALT-M  ",
      "  หน่วยความจำที่เหลือ          "
      };

char *menu10lev1[] = {
     "  ตั้งค่าตัวเลือก    ",
     "  อ่านตัวเลือกเก่า  ",
     "  เก็บตัวเลือก     ",
     };

int xlev1[] = {1,9,16,24,31,36,41,49,57};         /*    49,55,63};   */
int ylev1[] = {3,3,3,3,3,3,3,3,3,3};
int nolev1[] = {8,7,7,11,3,8,7,7,3};            /*  2,6,3};       */
char **msglev1[] = {menu1lev1,menu2lev1,menu3lev1,
                    menu4lev1,menu5lev1,menu6lev1,
                    menu7lev1,      /*     menu8lev1   */
                    menu9lev1,
                    menu10lev1};
char *cursormenu[] = {
     "  ไปทางซ้าย      ^S  ",
     "  ไปทางขวา      ^D  ",
     "  เลื่อนขึ้นบน      ^E  ",
     "  เลื่อนลงล่าง     ^X  ",
     "  ไปคำถัดไป      ^F  ",
     "  ไปคำที่แล้ว      ^A  ",
     };
char *scrnmenu[] = {
     "  เลื่อนบรรทัดขึ้น   ^W  ",
     "  เลื่อนบรรทัดลง   ^Z  ",
     "  เลื่อนหน้าขึ้น     ^R  ",
     "  เลื่อนหน้าลง     ^C  ",
     };
char *beginmenu[] = {
     "  ไปต้นบรรทัด   Home  ",
     "  ไปต้นบล็อก     ^QB  ",
     "  ไปต้นแฟ้มข้อมูล  ^QR  ",
     };
char *endmenu[] = {
     "  ไปท้ายบรรทัด   End  ",
     "  ไปท้ายบล็อก    ^QK  ",
     "  ไปท้ายแฟ้มข้อมูล ^QC  ",
     };
char *specialcharmenu[] = {
     "  ALT-A    €  ",
     "  ALT-B      ",
     "  ALT-C      ",
     "  ALT-D      ",
     "  ALT-E    …  ",
     "  ALT-F      ",
     "  ALT-H      ",
     "  ALT-I      ",
     "  ALT-J    ฿  ",
     "  ALT-N    ๚  ",
     "  ALT-O      ",
     "  ALT-Q      ",
     "  ALT-R      ",
     "  ALT-S      ",
     "  ALT-T      ",
     "  ALT-U      ",
     "  ALT-V    ๛  ",
     "  ALT-Y      ",
     };
char *cntrlpmenu[] = {
     "  ตัวเอียง         ^PW  ",
     "  ขีดเส้นใต้ ๑ เส้น  ^PS  ",
     "  ขีดเส้นใต้ ๒ เส้น  ^PR  ",
     "  ตัวหนา          ^PB  ",
     "  ตัวใหญ่     ^PE  ",
     "  ตัวยกขึ้น         ^PT  ",
     "  ตัวห้อย          ^PV  ",
     };

/****************************************************************************/
/*  Write menu-bar.                                                         */
/****************************************************************************/
void headmenu(unsigned topmenu,char attr)
{
   switch(topmenu) {
   case 1 : dispstrhgc(" แฟ้มข้อมูล ",1,1,attr);
            _line(0,24,80,24);
            _line(0,43,80,43);
            break;
   case 2 : dispstrhgc(" เคลื่อนที่ ",9,1,attr);
            _line(72,24,136,24);
            _line(72,43,136,43);
            break;
   case 3 : dispstrhgc(" ลบ/แทรก ",16,1,attr);
            _line(128,24,200,24);
            _line(128,43,200,43);
            break;
   case 4 : dispstrhgc(" ตัวอักษร ",24,1,attr);
            _line(192,24,256,24);
            _line(192,43,256,43);
            break;
   case 5 : dispstrhgc(" ค้นหา ",31,1,attr);
            _line(248,24,296,24);
            _line(248,43,296,43);
            break;
   case 6 : dispstrhgc(" บล็อก ",36,1,attr);
            _line(288,24,336,24);
            _line(288,43,336,43);
            break;
   case 7 : dispstrhgc(" จัดจอภาพ ",41,1,attr);
            _line(328,24,400,24);
            _line(328,43,400,43);
            break;
/*
   case 8 : dispstrhgc(" รูปภาพ ",49,1,attr);
            _line(392,24,448,24);
            _line(392,43,448,43);
            break;
   case 9 : dispstrhgc(" คำสั่งอื่นๆ ",55,1,attr);
            _line(440,24,512,24);
            _line(440,43,512,43);
            break;
   case 10 : dispstrhgc(" ตัวเลือก ",63,1,attr);
            _line(504,24,639,24);
            _line(504,43,639,43);
            break;
*/
   case 8 : dispstrhgc(" คำสั่งอื่นๆ ",49,1,attr);
            _line(392,24,464,24);
            _line(392,43,464,43);
            break;
   case 9 : dispstrhgc(" ตัวเลือก ",57,1,attr);
            _line(456,24,639,24);                  /**** MODIFIED ****/
            _line(456,43,639,43);
            break;
   }
}

/****************************************************************************/
/*  Find current menu.                                                      */
/****************************************************************************/
unsigned findcurmenu(unsigned lev1,unsigned lev2,unsigned lev3,unsigned lev4)
{
    return((lev1 * 16 * 16 * 16) +
           (lev2 * 16 * 16) +
           (lev3 * 16) +
           lev4);
}

/****************************************************************************/
/*  Find current level.                                                     */
/****************************************************************************/
void findcurlevel(unsigned curmenu,unsigned *lev1,unsigned *lev2,unsigned *lev3,unsigned *lev4)
{
    *lev1 = (curmenu/(16 * 16 * 16)) % 16;
    *lev2 = (curmenu/(16 * 16)) % 16;
    *lev3 = (curmenu/16) % 16;
    *lev4 = curmenu % 16;
}

/****************************************************************************/
/* Select menu from pulled-down menu. Recieve value of curmenu by reference */
/* to be displayed on screen. User would select menu so value of curmenu    */
/* should be changed too. Return RETKEY if user press return,else ESCKEY if */
/* escape key is pressed,else COMMAND KEY if command key is pressed.        */
/****************************************************************************/
unsigned pulled_down_menu(unsigned *curmenu,unsigned *x,unsigned *y)
{
    FILE *fp;
    unsigned lev1, lev2, lev3, lev4;
    int i, key, quit;
    char cur_dir[50];
    char *scrnndx, *scrnndx2;

    findcurlevel(*curmenu,&lev1,&lev2,&lev3,&lev4);
    headmenu(lev1,REVERSEATTR);
    do {
      scrnndx = savescrn(xlev1[lev1-1]-1,
                ylev1[lev1-1]-1,
                xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                ylev1[lev1-1]+nolev1[lev1-1]);
      if (lev1 == 9)
         last_menu_on = 1;
      i = selectmenu(xlev1[lev1-1],ylev1[lev1-1],nolev1[lev1-1],msglev1[lev1-1],&lev2);
      last_menu_on = 0;

      if (i == 0) {  /* command key */
        resscrn(scrnndx,
                xlev1[lev1-1]-1,
                ylev1[lev1-1]-1,
                xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                ylev1[lev1-1]+nolev1[lev1-1]);
        headmenu(lev1,0);
        *curmenu = lev2;
        findcurlevel(*curmenu,&lev1,&lev2,&lev3,&lev4);
        headmenu(lev1,REVERSEATTR);
        scrnndx = savescrn(xlev1[lev1-1]-1,
                  ylev1[lev1-1]-1,
                  xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                  ylev1[lev1-1]+nolev1[lev1-1]);
        i = RETKEY;
      }
      if (((lev1 == 2) && ((lev2 == 1) || (lev2 == 2) || (lev2 == 3) || (lev2 == 4))) ||
          ((lev1 == 4) && ((lev2 == 10) || (lev2 == 11))) ||
          ((lev1 == 8) && (lev2 == 3)) ||
          ((lev1 == 9) && ((lev2 == 2)  || (lev2 == 3)))) {
        lev3 = 1;
      } else {
        lev3 = 0;
      }
      if (i == RETKEY) {
        quit = NO;
        *curmenu = findcurmenu(lev1,lev2,lev3,lev4);
        *curmenu = *curmenu & 0xff00;
        switch (*curmenu) {
        case 0x1100 :
            if (filename[0] != '\0') {
            if (changeflag) {
              blockmsg(10);
              dispstrhgc("แฟ้มข้อมูลเดิมยังไม่ได้จัดเก็บ ต้องการจัดเก็บหรือไม่ (Y/N)?"
                          ,22-CENTER_FACTOR,10,2);
              do {
                key = ebioskey(0) & 0xff;
                if ((key == 'y') || (key == 'Y')) {
                  writeblk(filename,sentinel->next,0,sentinel->previous,MAXCOL);
                  abandonfile();
                }
                if ((key == 'n') || (key == 'N'))
                  abandonfile();
              } while ((key != 'n') && (key != 'N') && (key != 'y') && (key != 'Y'));
            } else {
              abandonfile();
            }
            changeflag = NO;
            filename[0] = '\0';
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            cls();
            placekey(RETKEY);
          } else {
            dispstrhgc(" ใส่ชื่อแฟ้มข้อมูลที่ต้องการสร้างใหม่ :                        ",0,5,0);
            box(0,103,424,124);
            i = getname(filename,29,5,22,0);
            if ((i == YES) && (filename[0] != '\0')) {
              if ((fp = fopen(filename,"rt")) != NULL) {
                fclose(fp);
                errorsound();
                blockmsg(10);
                dispstrhgc("แฟ้มข้อมูลนี้มีอยู่แล้ว กดปุ่ม <ESC> เพื่อทำงานต่อ"
                            ,27-CENTER_FACTOR,10,2);
                while (ebioskey(0) != ESCKEY);
                filename[0] = '\0';
              } else {
                setupnode();
                initvalue(x, y);
                quit = YES;
              }
            } else {
              filename[0] = '\0';
            }
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            cls();
          }
          break;
        case 0x1200 :
            if (filename[0] != '\0') {
              if (changeflag) {
                blockmsg(10);
                dispstrhgc("แฟ้มข้อมูลเดิมยังไม่ได้จัดเก็บ ต้องการจัดเก็บหรือไม่ (Y/N)?"
                            ,22-CENTER_FACTOR,10,2);
                key = 0;
                do {
                  key = ebioskey(0) & 0xff;
                  if ((key == 'y') || (key == 'Y')) {
                    writeblk(filename,sentinel->next,0,sentinel->previous,MAXCOL);
                    abandonfile();
                  }
                  if ((key == 'n') || (key == 'N'))
                    abandonfile();
                } while ((key != 'n') && (key != 'N') && (key != 'y') && (key != 'Y'));
              } else {
                abandonfile();
              }
              changeflag = NO;
              filename[0] = '\0';
              resscrn(scrnndx,
                      xlev1[lev1-1]-1,
                      ylev1[lev1-1]-1,
                      xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                      ylev1[lev1-1]+nolev1[lev1-1]);
              cls();
              placekey(RETKEY);
            }
            else {
              initvalue(x,y);
              dispstrhgc(" ใส่ชื่อแฟ้มข้อมูลที่ต้องการแก้ไข :                        ",0,6,0);
              box(0,123,400,144);
              setupnode();
              strcpy(filename,"*.*");
              i = getname(filename,26,6,22,0);
              if ((i == YES) && (filename[0] != '\0')) {
                if (havewild(filename))
                  selectfile(filename);
                if (filename[0] != '\0') {
                  if (read_file(filename) != ERROR) {
                    loadtoline(curline->text);
                    quit = YES;
                  } else {
                    filename[0] = '\0';
                  }
                }
              } else {
                filename[0] = '\0';
              }
              resscrn(scrnndx,
                      xlev1[lev1-1]-1,
                      ylev1[lev1-1]-1,
                      xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                      ylev1[lev1-1]+nolev1[lev1-1]);
              cls();
            }
	    break;
        case 0x1300 :
            if (filename[0] != '\0') {
              savepic();
              writeblk(filename,sentinel->next,0,sentinel->previous,MAXCOL);
              retpic();
              changeflag = NO;
            }
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x1400 :
            print_file();
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x1500 :
            savepic();
            dispstrhgc(" เปลี่ยนช่องเก็บ(Logged Drive)จาก  : เป็น   ",0,9,0);
            prchar('A' + getdisk(),BOLDATTR,30,9);
            box(0,183,312,204);
            waitkbd(37,9);
            key = ebioskey(0) & 0xff;
            if ((key >= 'a') && (key <= 'p'))
              setdisk(key - 'a');
            else
              if ((key >= 'A') && (key <= 'P'))
                setdisk(key - 'A');
            retpic();
            writedriveno();
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x1600 :
            savepic();
            dispstrhgc("                                     ",0,10,0);
            box(0,203,312,224);
            getstring(getcwd(cur_dir,37),1,10,37,0,ENGUPCASE);
            chdir(cur_dir);
            retpic();
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x1700 :
            if (filename[0] != '\0')
              storeline(curline);
            settext();
            if (changeflag == YES)
              puts("*** Warning ! Your file not saved ***");
            puts("Type EXIT to return to CU-Writer");
            system("");
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            initscrn();
            showpageall();
            break;
        case 0x1800 :
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            quitprog = YES;
            quit = YES;
            break;
        case 0x2100 :
            scrnndx2 = savescrn(19,4,20+thaistrlen(cursormenu[0]),5+6);
            i = selectmenu(20,5,6,cursormenu,&lev3);
            if (i == RETKEY) quit = YES;
            resscrn(scrnndx2,19,4,20+thaistrlen(cursormenu[0]),5+6);
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x2200 :
            scrnndx2 = savescrn(19,5,20+thaistrlen(scrnmenu[0]),6+4);
            i = selectmenu(20,6,4,scrnmenu,&lev3);
            if (i == RETKEY) quit = YES;
            resscrn(scrnndx2,19,5,20+thaistrlen(scrnmenu[0]),6+4);
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x2300 :
            scrnndx2 = savescrn(19,6,20+thaistrlen(beginmenu[0]),7+3);
            i = selectmenu(20,7,3,beginmenu,&lev3);
            if (i == RETKEY) quit = YES;
            resscrn(scrnndx2,19,6,20+thaistrlen(beginmenu[0]),7+3);
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x2400 :
            scrnndx2 = savescrn(19,7,20+thaistrlen(endmenu[0]),8+3);
            i = selectmenu(20,8,3,endmenu,&lev3);
            if (i == RETKEY) quit = YES;
            resscrn(scrnndx2,19,7,20+thaistrlen(endmenu[0]),8+3);
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x4a00 :
            scrnndx2 = savescrn(47,2,48+thaistrlen(specialcharmenu[0]),3+13);
            special_char = 1;
            i = selectmenu(48,3,13,specialcharmenu,&lev3);
            special_char = 0;

            /***** Modified ******/
            lev4 = (lev3 >> 4) & 0x0F;
            lev3 &= 0x0F;

            if (i == RETKEY) quit = YES;
            resscrn(scrnndx2,47,2,48+thaistrlen(specialcharmenu[0]),3+13);
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x4b00 :
            edit_font();
            headmenu(lev1,REVERSEATTR);
            break;
        case 0x8300 :
            scrnndx2 = savescrn(39,6,40+thaistrlen(cntrlpmenu[0]),6+8);
            i = selectmenu(40,7,7,cntrlpmenu,&lev3);
            if (i == RETKEY) quit = YES;
            resscrn(scrnndx2,39,6,40+thaistrlen(cntrlpmenu[0]),6+8);
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x8400 :
            savepic();
            dispstrhgc(" ••••••••••••••••••••••••••••••••••••••••••••••• ",20-CENTER_FACTOR,5,2);
            dispstrhgc(" – สร้างตารางโดยการกดปุ่ม Scroll Lock , แล้วกด Shift – ",20-CENTER_FACTOR,6,2);
            dispstrhgc(" – ร่วมกับปุ่มต่างๆทางด้านขวาเพื่อสร้างตารางดังต่อไปนี้       – ",20-CENTER_FACTOR,7,2);
            dispstrhgc(" –                                               – ",20-CENTER_FACTOR,8,2);
            dispstrhgc(" – minus(•) = •    plus(+) =                    – ",20-CENTER_FACTOR,9,2);
            dispstrhgc(" –                                               – ",20-CENTER_FACTOR,10,2);
            dispstrhgc(" – Home(7)  =        8    = ‘      PgDn(9)  =  – ",20-CENTER_FACTOR,11,2);
            dispstrhgc(" –                                               – ",20-CENTER_FACTOR,12,2);
            dispstrhgc(" –    4     = “       5    = –         6     = ’ – ",20-CENTER_FACTOR,13,2);
            dispstrhgc(" –                                               – ",20-CENTER_FACTOR,14,2);
            dispstrhgc(" – End(1)   =        2    =       PgDn(3)  =  – ",20-CENTER_FACTOR,15,2);
            dispstrhgc(" •กด <ESC> เพื่อทำงานต่อ••••••••••••••••••••••••••• ",20-CENTER_FACTOR,16,2);
            while (ebioskey(0) != ESCKEY);
            retpic();
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x8700 :
            scrnndx2 = savescrn(16-CENTER_FACTOR, 5, 51-CENTER_FACTOR, 7);
            framebox(16-CENTER_FACTOR,5,51-CENTER_FACTOR,7,0);
            dispprintf(18-CENTER_FACTOR,6,0,
                       "หน่วยความจำเหลืออยู่ %ld ตัวอักษร",farcoreleft());
            bioskey(0);
            resscrn(scrnndx2, 16-CENTER_FACTOR, 5, 51-CENTER_FACTOR, 7);
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x9100 :
            last_menu_on = 1;
            setoption();
            last_menu_on = 0;
            showpageall();
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        case 0x9200 :
        case 0x9300 :
        {
            char *option_menu[2];
            char buf0[65];
            char buf1[65];
            char tbuf[65];
            search_file_mode smode;

            getcwd(cur_dir, sizeof(cur_dir));
            sprintf(buf0, " ที่ Current  directory (%.30s) ", cur_dir);
            sprintf(buf1, " ที่ Cuwriter directory (%.30s) ", cw_dir);
            if (strlen(buf0) > strlen(buf1)) {
                sprintf(tbuf, "%-*s", strlen(buf0), buf1);
                option_menu[0] = buf0;
                option_menu[1] = tbuf;
            } else {
                sprintf(tbuf, "%-*s", strlen(buf1), buf0);
                option_menu[0] = tbuf;
                option_menu[1] = buf1;
            }
            scrnndx2 = savescrn(20, 5, 20+1+thaistrlen(option_menu[0]), 5+3);
            i = selectmenu(20+1, 5+1, 2, option_menu, &lev3);
            if (i == RETKEY) {
                smode = (lev3 == 1) ? CUR_DIR : CW_DIR;
                savepic();
                dispstrhgc(" ••••••••••••••••••••••••• ",50-CENTER_FACTOR,6,REVERSEATTR);
                dispstrhgc(" ••••••••••••••••••••••••• ",50-CENTER_FACTOR,8,REVERSEATTR);
                if (*curmenu == 0x9200) {
                    dispstrhgc(" – กำลังอ่านตัวเลือก รอสักครู่... – ",50-CENTER_FACTOR,7,REVERSEATTR);
                    readoption(smode);
                } else {
                    dispstrhgc(" – กำลังเก็บตัวเลือก รอสักครู่... – ",50-CENTER_FACTOR,7,REVERSEATTR);
                    saveoption(smode);
                }
                retpic();
            }
            resscrn(scrnndx2, 20, 5, 20+1+thaistrlen(option_menu[0]), 5+3);
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
            break;
        }
        default     :
            quit = YES;
            resscrn(scrnndx,
                    xlev1[lev1-1]-1,
                    ylev1[lev1-1]-1,
                    xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                    ylev1[lev1-1]+nolev1[lev1-1]);
                    break;
        }
        writestatus(*x+firstcol);
        writetab();
        if (quit == YES) {
          headmenu(lev1,0);
          showpageall();
          *curmenu = findcurmenu(lev1,lev2,lev3,lev4);
          return(i);
        }
      } else {
        resscrn(scrnndx,
                xlev1[lev1-1]-1,
                ylev1[lev1-1]-1,
                xlev1[lev1-1]+thaistrlen(*msglev1[lev1-1]),
                ylev1[lev1-1]+nolev1[lev1-1]);
      }
      switch (i) {
      case ESCKEY : writestatus(*x+firstcol);
                    writetab();
                    headmenu(lev1,0);
                    showpageall();
                    *curmenu = findcurmenu(lev1,lev2,lev3,lev4);
                    return(ESCKEY);
      case LEKEY  : headmenu(lev1,0);
                    if (lev1 != 1)
                      lev1--;
                    else
                      lev1 = 9;       /* lev1 = 10;  */
                    lev2 = 1;
                    headmenu(lev1,REVERSEATTR);
                    break;
      case RIKEY  : headmenu(lev1,0);
                    if (lev1 != 9)      /*(lev1 != 10)*/
                      lev1++;
                    else
                      lev1 = 1;
                    lev2 = 1;
                    headmenu(lev1,REVERSEATTR);
                    break;
      }
    } while (1);
}