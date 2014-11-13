/****************************************************************************/
/*  SEARCH.C 15 JAN 89                                                      */
/****************************************************************************/
#include "inc.h"
#include "..\common\cwgrphc.h"
#include "convert.h"

/****************************************************************************/
/*  Searching string                                                        */
/*  require                                                                 */
/*  (1)  source    ->  source string for search                             */
/*  (2)  replace   ->  string to replace                                    */
/*   3)  option    ->  option that will be                                  */
/*                     G  ->  global search                                 */
/*                     U  ->  mix lower & upper                             */
/*                     N  ->  no pause                                      */
/*                     W  ->  full word                                     */
/*  return 0 if error occur                                                 */
/****************************************************************************/
int searchonlyinfo(void)
{
    int i;
    dispstrhgc(" •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••• ",8-CENTER_FACTOR,4,2);
    dispstrhgc(" – ใส่คำที่ต้องการค้นหา :                                                    – ",8-CENTER_FACTOR,5,2);
    dispstrhgc(" – ค้นหาแบบไหน (ใส่ตัวอักษรของตัวเลือกที่ต้องการ) :                              – ",8-CENTER_FACTOR,6,2);
    dispstrhgc(" – ตัวเลือก : G = เริ่มค้นหาตั้งแต่ต้นแฟ้มข้อมูล                                    – ",8-CENTER_FACTOR,7,2);
    dispstrhgc(" –          U = ไม่สนใจตัวเล็กหรือตัวใหญ่ (เช่น a กับ A)                        – ",8-CENTER_FACTOR,8,2);
    dispstrhgc(" –          W = ค้นหาเป็นคำ (หรือเป็นประโยคในภาษาไทย)                       – ",8-CENTER_FACTOR,9,2);
    dispstrhgc(" •<ESC> ยกเลิก•••••••••••••••••••••••••••••••••••••••••••••••••••••••••• ",8-CENTER_FACTOR,10,2);
    dispstrhgc(source,29-CENTER_FACTOR,5,REVERSEATTR);
    dispstrhgc(option,51-CENTER_FACTOR,6,REVERSEATTR);
    pagecomplete = NO;
    while (1) {
      i = getstring(source,29-CENTER_FACTOR,5,50,2,THAIENG);
      switch(i) {
      case ESCKEY : return(ESCKEY);
      case DNKEY  :
      case YES    : do {
                      i = getstring(option,51-CENTER_FACTOR,6,3,2,ENGLISH);
                      switch(i) {
                      case ESCKEY : return(ESCKEY);
                      case YES    : strupr(option);    /* to upper case   */
                                    return(YES);
                      }
                    } while (i != UPKEY);
                    break;
      }
    }
    return 0;
}

int searchreplaceinfo(void)
{
    int i,j;
    dispstrhgc(" •••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••• ",8-CENTER_FACTOR,4,2);
    dispstrhgc(" – ใส่คำที่ต้องการค้นหา :                                                    – ",8-CENTER_FACTOR,5,2);
    dispstrhgc(" – ใส่คำที่ต้องการแทนที่ :                                                    – ",8-CENTER_FACTOR,6,2);
    dispstrhgc(" – ค้นหาแบบไหน (ใส่ตัวอักษรของตัวเลือกที่ต้องการ) :                              – ",8-CENTER_FACTOR,7,2);
    dispstrhgc(" – ตัวเลือก : G = ค้นหา/แทนที่ทั้งหมดตั้งแต่ต้นจนจบ                                – ",8-CENTER_FACTOR,8,2);
    dispstrhgc(" –          U = ไม่สนใจตัวเล็กหรือตัวใหญ่ (เช่น a กับ A)                        – ",8-CENTER_FACTOR,9,2);
    dispstrhgc(" –          W = ค้นหาเป็นคำ (หรือเป็นประโยคในภาษาไทย)                       – ",8-CENTER_FACTOR,10,2);
    dispstrhgc(" –          N = ไม่ต้องหยุดถามว่าจะแทนที่หรือไม่                                – ",8-CENTER_FACTOR,11,2);
    dispstrhgc(" •<ESC> ยกเลิก•••••••••••••••••••••••••••••••••••••••••••••••••••••••••• ",8-CENTER_FACTOR,12,2);
    dispstrhgc(source,29-CENTER_FACTOR,5,REVERSEATTR);
    dispstrhgc(replace,29-CENTER_FACTOR,6,REVERSEATTR);
    dispstrhgc(option,51-CENTER_FACTOR,7,REVERSEATTR);
    pagecomplete = NO;
    while (1) {
      i = getstring(source,29-CENTER_FACTOR,5,50,2,THAIENG);
      switch(i) {
        case ESCKEY : return(ESCKEY);
        case DNKEY  :
        case YES    : do {
                        i = getstring(replace,29-CENTER_FACTOR,6,50,2,THAIENG);
                        switch (i) {
                        case ESCKEY : return(ESCKEY);
                        case DNKEY  :
                        case YES    : do {
                                        j = getstring(option,51-CENTER_FACTOR,7,3,2,ENGLISH);
                                        switch(j) {
                                        case ESCKEY : return(ESCKEY);
                                        case YES    : strupr(option);
                                                      return(YES);
                                        }
                                      } while (j != UPKEY);
                                      break;
                        }
                      } while (i != UPKEY);
                      break;
      }
    }
    return 0;
}

int optionglobal(void)
{
     int i = 0;
     while (option[i] != '\0'){
           if (option[i] == 'G')
              return(YES);
           i++;
     }
     return(NO);
}

int optionupper(void)
{
     int i = 0;
     while (option[i] != '\0'){
           if (option[i] == 'U')
              return(YES);
           i++;
     }
     return(NO);
}

int optionword(void)
{
     int i = 0;
     while (option[i] != '\0'){
           if (option[i] == 'W')
              return(YES);
           i++;
     }
     return(NO);
}

int optionnoask(void)
{
     int i = 0;
     while (option[i] != '\0'){
           if (option[i] == 'N')
              return(YES);
           i++;
     }
     return(NO);
}

char *searchline(char *textline,int startpos) /* startpos origin 0 */
{
    int i;
    static char buffer[MAXCOL*5];
    char *point,*buffaddr,*textaddr;
    while ((startpos > 0) && (*textline != '\0')) {
      if ((*textline < 32) && (*textline != '\0')) {
        textline++;
      }
      textline++;
      startpos--;
    }
    textaddr = textline;
    i = 0;
    while (*textline != '\0') {
      buffer[i++] = *(textline++);
    }
    buffer[i] = ' ';
    buffer[i+1] = '\0';
    if (optionupper()) {
      strupr(buffer);
    }
    if ((point = strstr(buffer,source)) != NULL) {
      buffaddr = buffer;
      while (buffaddr++ != point) {
        textaddr++;
      }
      return(textaddr);
    } else {
      return(NULL);
    }
}

void wordnotfound(void)
{
    errorsound();
    dispstrhgc(" •••••••••••••••••••••••••••••• ",27-CENTER_FACTOR,5,2);
    dispstrhgc(" – หาคำไม่พบ ! กด ESC เพื่อทำงานต่อ – ",27-CENTER_FACTOR,6,2);
    dispstrhgc(" •••••••••••••••••••••••••••••• ",27-CENTER_FACTOR,7,2);
    while (ebioskey(0) != ESCKEY);
    pagecomplete = NO;
}
int searchfwd(unsigned *x,unsigned *y)
{
    struct line_node *savepage,*templine;
    int linecount,savecol,i,enlargeflag,startpos;
    font_attr font = 0;
    char *foundpoint,*addr;
    savecol = firstcol;
    savepage = curpage;
    storeline(curline);
    templine = curpage;
    linecount = wind.width - 1;
    while (templine != curline) {
      linecount--;
      templine = templine->next;
    }
    startpos = linearcolumn(curline->text,*x + firstcol,&font);
    foundpoint = NULL;
    while ((foundpoint == NULL) && (curline != sentinel)) {
      foundpoint = searchline(curline->text,startpos);
      if (foundpoint != NULL) {           /* found */
        while ((*y = findrow()) >= wind.width) {
          curpage = curpage->next;
        }
        *x = 0;
        firstcol = 0;
        addr = curline->text;
        enlargeflag = NO;
        while (addr != foundpoint) {
          if ((whatlevel(*addr) == MIDDLE) && (*addr >= 32)) {
            if (enlargeflag) {
              (*x)++;
            }
            (*x)++;
          } else {
            if (*addr == ENLARGECODE) {
              enlargeflag = enlargeflag ^ 1;
            }
          }
          addr++;
        }
        for (i = 0;source[i] != '\0';i++) {
          if (whatlevel(source[i]) == MIDDLE) {
            if (enlargeflag) {
              (*x)++;
            }
            (*x)++;
          }
        }
        while (*x >= wind.length) {
          firstcol = firstcol + wind.length;
          *x = *x - wind.length;
        }
      } else {
        if (linecount > 0) {
          linecount--;
          curline = curline->next;
          lineno++;
        } else {
          curline = curline->next;
          lineno++;
          curpage = curline;
          linecount = wind.width - 1;
        }
        startpos = 0;
      }
    }
    if (foundpoint == NULL) {
      curline = sentinel->previous;
      curpage = curline;
      loadtoline(curline->text);
      endline(x);
    }
    loadtoline(curline->text);
    if (savepage != curpage) {
      showpageall();
    } else {
      if (firstcol != savecol) {
        showpageall();
      }
    }
    if (foundpoint != NULL) {
      return(YES);
    } else {
      return(NO);
    }
}

void addblank(void)
{
    int i;
    i = 79;
    while (i != 0) {
      source[i] = source[i-1];
      i--;
    }
    source[0] = ' ';
    i = strlen(source);
    source[i] = ' ';
    source[i+1] = '\0';
}

void searching(unsigned *x,unsigned *y)
{
    storeline(curline);
    if (searchonlyinfo() == YES) {
      showpageall();
      replaceflag = NO;
      if (optionword())
        addblank();
      if (optionupper())
        strupr(source);
      if (optionglobal()){
        topfile(x);
        showpageall();
      }
      if (!searchfwd(x,y))
        wordnotfound();
    }
}

void replaceword(unsigned *x,unsigned *y)
{
    int i;
    for (i = strlen(source);i != 0;i--)
      backspace(x);
    while (replace[i] != '\0') {
      insert_char(replace[i],x,y);
      i++;
    }
    refreshline(0,*y);
}

int searchreplace(unsigned *x,unsigned *y)
{
    int ok = 0,i,ask = YES,global = NO,found = NO;
    if (optionnoask()) {
      ok = 'y';
      ask = NO;
    }
    if (optionglobal()) {
      topfile(x);
      showpageall();
      global = YES;
    }
    do {
      if (searchfwd(x,y)){
        found = YES;
        if (ask) {
          dispstrhgc("แทนที่หรือไม่ ? (Y/N) ",1,2,BOLDATTR);
          while ((ok != 'y') && (ok != 'n') &&
                (ok != 'Y') && (ok != 'N')) {
            while (!keypressed()) {
              setcurpos(19,2,thaimode);
              for(i=0;i<15000;i++);
                setcurpos(19,2,thaimode);
              for(i=0;i<15000;i++);
                setcurpos(wind.col + *x,wind.row + *y,thaimode);
              for(i=0;i<15000;i++);
                setcurpos(wind.col + *x,wind.row + *y,thaimode);
              for(i=0;i<15000;i++);
            } /* while !keypressed */
            ok = ebioskey(0);
            if ((ok != CNTRL_U) && (ok != ESCKEY))
              ok = ok & 0xff;
            else
              return(CNTRL_U);
          } /* while ok */
        } else {
          if (keypressed()) {
            ok = ebioskey(0);
            if ((ok == CNTRL_U) || (ok == ESCKEY))
              return(CNTRL_U);
          }
          ok = 'y';
        }
        if ((ok == 'y') || (ok == 'Y'))
          replaceword(x,y);
        ok = 0;
      } else {
       global = NO;
      }
    } while (global);
    return(found);
}

void replacing(unsigned *x,unsigned *y)
{
    storeline(curline);
    if (searchreplaceinfo() == YES) {
      showpageall();
      replaceflag = YES;
      if (optionword())
        addblank();
      if (optionupper())
        strupr(source);
      if (!searchreplace(x,y))
        wordnotfound();
    }
}
