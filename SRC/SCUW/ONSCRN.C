/* Update: Suttipong Kanakakorn
            Tue  08-01-1989  13:57:19
   CUCC :  Garbage Collection
*/

#include "inc.h"
#include "..\common\cwgrphc.h"
#include "convert.h"

/****************************************************************************/
/*  Write tab.                                                              */
/****************************************************************************/
void writetab(void)
{
    int i = 0;
    prblank(wind.col,3);
    while (((i + firstcol + 1) < leftmar) && (i < wind.length)) {
      prblank(wind.col + i,3);
      i++;
    }
    if ((i < wind.length) && ((i + firstcol + 1) == leftmar)) {
      prchar('L',0,wind.col + i,3);
      i++;
    }
    while (((i + firstcol + 1) < rightmar) && (i < wind.length)) {
      if (tab[i+firstcol] == NO)
        prchar('-',0,wind.col + i,3);
      else
        prchar('!',0,wind.col + i,3);
      i++;
    }
    if ((rightmar - 1 - firstcol) < wind.length) {
      prchar('R',0,wind.col + rightmar - 1 - firstcol,3);
    }
    i++;
    while (i < wind.length) {
      prblank(wind.col + i,3);
      i++;
    }
}
/****************************************************************************/
/*  Center line.                                                            */
/****************************************************************************/
void centerline(unsigned y)
{
    int i,j,k,count;
    for(i=1;(workline.middle[i] == ' ') || (workline.middle[i] == WRAPBLANK);i++);
    for(j = strlen(workline.middle) - 1;
        (workline.middle[j] == ' ') || (workline.middle[j] == WRAPBLANK);j--);
    if ((rightmar - leftmar) > (j - i)) {
      k = leftmar + ((rightmar - leftmar) - (j - i))/2;
      if (k < i) {
        for (count = i - k;count > 0;count--)
          delete_char(0);
      } else {
        for (count = k - i;count > 0;count--)
          insertblank(1,' ');
      }
      refreshline(0,y);
    }
}

void doonscrn(register unsigned key,unsigned x,unsigned y)
{
    register int j;
    char st[4],invmsg[60];

    key &= 0xff;
    if (!isalpha(key) && !iscntrl(key))
        return;
    key &= 0x1f;
    switch(key) {
    case 'l' - 'a' + 1 :
      itoa(leftmar,st,10);
      framebox(25-CENTER_FACTOR, 4, 25-CENTER_FACTOR+39, 6,REVERSEATTR);
      dispstrhgc("ต้องการตั้งกั้นซ้ายที่คอลัมน์ที่เท่าไหร่ ?",25-CENTER_FACTOR+4,5,REVERSEATTR);
      if (getstring(st,58-CENTER_FACTOR,5,3,2,NUMBER)) {
        j = atoi(st);
        if ((j < rightmar) && (j > 0)) {
          leftmar = j;
        }
	else {                                   /* Invalid Left Margin */
	  if (j<1)
	    strcpy(invmsg,"กั้นซ้ายน้อยกว่า 1 !");
	  else
	    strcpy(invmsg,"กั้นซ้ายมากกว่ากั้นขวา !");
	  strcat(invmsg," กดปุ่มใดๆเพื่อทำงานต่อ ...");
          blockmsg(10);
	  dispstrhgc(invmsg,25-CENTER_FACTOR,10,REVERSEATTR);
	  errorsound();
          ebioskey(0);
        }
      }
      break;
    case 'r' -'a' + 1 :
      itoa(rightmar,st,10);
      framebox(25-CENTER_FACTOR, 4, 25-CENTER_FACTOR+39, 6, 2);
      dispstrhgc("ต้องการตั้งกั้นขวาที่คอลัมน์ที่เท่าไหร่ ?",25-CENTER_FACTOR+4,5,2);
      if (getstring(st,58-CENTER_FACTOR,5,3,2,NUMBER)) {
        j = atoi(st);
	if ((j > leftmar) && (j <= MAXRMAR)) {
          rightmar = j;
	} else {            			 /* Invalid Right Margin */
	  if (j<=leftmar)
	    strcpy(invmsg,"กั้นขวาน้อยกว่ากั้นซ้าย !");
	  else
	    sprintf(invmsg,"กั้นขวาสูงสุดไม่เกิน %d !",MAXRMAR);
	  strcat(invmsg," กดปุ่มใดๆเพื่อทำงานต่อ ...");
	  blockmsg(10);
	  dispstrhgc(invmsg,25-CENTER_FACTOR,10,REVERSEATTR);
	  errorsound();
          ebioskey(0);
        }
      }
      break;
    case 'i' -'a' + 1:
      itoa(x+1,st,10);
      framebox(25-CENTER_FACTOR, 4, 25-CENTER_FACTOR+39, 6, 2);
      dispstrhgc("ต้องการตั้ง TAB ที่คอลัมน์ที่เท่าไหร่ ?",25-CENTER_FACTOR+4,5,2);
      if (getstring(st,58-CENTER_FACTOR,5,3,2,NUMBER)) {
        j = atoi(st);
        if ((j <= MAXCOL) && (j > 0))
          tab[j - 1]=YES;
      }
      break;
    case 'n' - 'a' + 1 :
      strcpy(st,"ALL");
      framebox(25-CENTER_FACTOR, 4, 25-CENTER_FACTOR+39, 6, 2);
      dispstrhgc("ต้องการลบ TAB ที่คอลัมน์ที่เท่าไหร่ ?",25-CENTER_FACTOR+4,5,2);
      if (getstring(st,58-CENTER_FACTOR,5,3,2,NUMBER)) {
        if (strcmp(st,"ALL") == 0) {
          for (j=0;j < MAXCOL;++j)
            tab[j] = NO;
        } else {
          j = atoi(st);
          if ((j <= MAXCOL) && (j > 0))
            tab[j - 1] = NO;
        }
      }
      break;
    case 'p' - 'a' + 1 :
      itoa(lineperpage,st,10);
      framebox(25-CENTER_FACTOR, 4, 25-CENTER_FACTOR+41, 6, 2);
      dispstrhgc("ต้องการตั้งจำนวนบรรทัดต่อหน้าเท่าไหร่ ?",25-CENTER_FACTOR+4,5,2);
      if (getstring(st,61-CENTER_FACTOR,5,3,2,NUMBER)) {
        if (atoi(st) > 0) {
          lineperpage = atoi(st);
          writepageline();
        }
      }
      break;
    case 'c' - 'a' + 1 :
        centerline(y);
        break;
    case 'x' - 'a' + 1 :
        relmargin = !relmargin;
        relmarstatus();
        break;
    }
    pagecomplete = NO;
    writestatus(x);
}

void movetotab(unsigned *x,unsigned y)
{
    int i,count;
    if ((*x + firstcol) >= (leftmar - 1)) {
      i = *x + firstcol + 1;
    } else {
      i = leftmar - 1;
    }
    for(;(tab[i] != YES) && (i < (rightmar-1));i++);
    if (i < (rightmar - 1)) {
      count = i - (*x + firstcol);
      while (count-- != 0) {
        if (insertmode)
          insertblank(*x + firstcol + 1,' ');
      }
      gocol(i,x);
      refreshline(0,y);
    }
}
void onscreen(unsigned x,unsigned y)
{
    int key;
    dispkey(CNTRL_O);
    waitkbd(3,2);
    key = ebioskey(0);
    prchar(key,0,3,2);
    doonscrn(key,x,y);
}
