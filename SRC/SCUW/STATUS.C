#include "inc.h"
#include "..\common\cwgrphc.h"
#include "convert.h"

void relmarstatus(void)
{
    if (relmargin == YES) {
      dispstrhgc("����¢ͺ",65-3,2,BOLDATTR);
    } else {
      dispstrhgc("       ",65-3,2,NORMALATTR);
    }
}

void arabictothai(char *st)
{
    while (*st != '\0'){
      if ((*st >= '0') && (*st <= '9')) {
        *st = *st | 0xf0;
      }
      st++;
    }
}

void writepage(void)
{
    char page[4];
    dispstrhgc("˹��    ",4,2,0);
    itoa(((lineno - 1)/lineperpage)+1,page,10);
    arabictothai(page);
    dispstrhgc(page,8,2,0);
}

void writeline(void)
{
    char line[5];
    dispstrhgc("��÷Ѵ    ",11,2,0);
    itoa(((lineno - 1) % lineperpage) + 1,line,10);
    arabictothai(line);
    dispstrhgc(line,17,2,0);
}

void writelineno(void)
{
    char st2[6];
    dispstrhgc("     ��÷Ѵ       ",4,2,0);
    itoa(lineno,st2,10);
    arabictothai(st2);
    dispstrhgc(st2,15,2,0);
}

void writepageline(void)
{
    if (pagebreak) {
      writepage();
      writeline();
    } else {
      writelineno();
    }
}

void writecolno(unsigned x)
{
    char st2[5];
    dispstrhgc("�������",21,2,00);
    itoa(x+1,st2,10);
    dispstrhgc("    ",27,2,00);
    arabictothai(st2);
    dispstrhgc(st2,27,2,0);
}

void writeinsmode(void)
{
    if (insertmode) {
      dispstrhgc("������á",33,2,00);
    } else {
      dispstrhgc(" �����Ѻ ",33,2,00);
    }
}

void writelanguage(void)
{
    if (thaimode) {
      dispstrhgc("  ��  ",42-1,2,BOLDATTR);
    } else {
      dispstrhgc("English",42-1,2,BOLDATTR);
    }
}

void writeattr(void)
{
    dispstrhgc("            ",50-1,2,0);
    if (fontused == 0)
      dispstrhgc(" ����ѡ�û���",50-1,2,fontused);
    else if ((fontused & 0x08) == 0x08)
           dispstrhgc("���¡���",50-1,2,fontused & 0xf7);
         else if ((fontused & 0x10) == 0x10)
                dispstrhgc("�������",50-1,2,fontused & 0xef);
              else
                dispstrhgc("����ѡ��",50-1,2,fontused);
}

void writefilename(void)
{
    dispstrhgc("��������� :                     ",58-SHRINK_FACTOR,0,00);
    if ( filename[0] != '\0') {
      dispstrhgc(filename,68-SHRINK_FACTOR,0,BOLDATTR);
    }
}

void writedriveno(void)
{
    dispstrhgc("�ÿ�  :",50-SHRINK_FACTOR,0,0);
    prchar('A' + getdisk(),BOLDATTR,55-SHRINK_FACTOR,0);
}
void writestatus(unsigned x)
{
    writepageline();
    writecolno(x);
    writeinsmode();
    writelanguage();
    writedriveno();
    writefilename();
    writeattr();
    relmarstatus();
}
