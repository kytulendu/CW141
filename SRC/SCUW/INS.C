/****************************************************************************/
/*  INS.C 15 JAN 89                                                         */
/*  Update: Suttipong Kanakakorn                                            */
/*            Sat  08-05-1989  03:59:22                                     */
/*          CUCC Debug 256 right margin                                     */
/****************************************************************************/
#include "inc.h"
#include "..\common\cwgrphc.h"
#include "convert.h"

/****************************************************************************/
/*  Insert blank to workline at specified column(origin 1).                 */
/****************************************************************************/
void insertblank(register unsigned j,char code)
{
    register int i;
    i = strlen(workline.middle);
    while (i >= j) {
      workline.middle[i+1] = workline.middle[i];  /* shift right */
      workline.upper[i+1] = workline.upper[i];    /*  by ignore  */
      workline.topest[i+1] = workline.topest[i];  /*  character  */
      workline.below[i+1] = workline.below[i];    /*   at last   */
      workline.attr[i+1] = workline.attr[i];      /*   column    */
      i--;
    }
    workline.middle[j] = code;         /* put blank */
    workline.upper[j] = ' ';           /* put blank */
    workline.topest[j] = ' ';          /* put blank */
    workline.below[j] = ' ';           /* put blank */
    workline.attr[j] = 0;
    if (haveblock()) {
      if (lineno == blkbegin.lineno) {
        if ((j - 1) <= blkbegin.column) {
          blkbegin.column++;
        }
      }
      if (lineno == blkend.lineno) {
        if ((j - 1) < blkend.column) {
          blkend.column++;
        }
      }
    }
    changeflag = YES;
}
/****************************************************************************/
/*  Insert new line after line specified.                                   */
/****************************************************************************/
void insert_line(struct line_node *currentline,struct line_node *newline)
{
    newline->previous = currentline;
    newline->next = currentline->next;
    (currentline->next)->previous = newline;
    currentline->next = newline;
}
/****************************************************************************/
/*  Shift right screen by number given.                                     */
/****************************************************************************/
void shiftscrn(unsigned count,unsigned *x)
{
    storeline(curline);
    while ((count != 0) && ((firstcol + wind.length) <= MAXCOL)) {
      firstcol++;
      (*x)--;
      count--;
    }
    pagecomplete = NO;  /* set flag to refresh screen */
}
/****************************************************************************/
/*  Sound beeper then display message "line too long" and wait keyboard     */
/*  for ESCAPE key.                                                         */
/****************************************************************************/
void linetoolong(void)
{
    errorsound();
    framebox(22-CENTER_FACTOR, 6, 22-CENTER_FACTOR+44, 8,REVERSEATTR);
    dispstrhgc("ºÃÃ·Ñ´ÂÒÇà¡Ô¹ä» ! ¡´ <ESC> à¾×èÍ·Ó§Ò¹µèÍ",22-CENTER_FACTOR+5,7,REVERSEATTR);
    while (ebioskey(0) != ESCKEY);
    pagecomplete = NO;
}
/****************************************************************************/
/*  Insert print control character at column given.                         */
/****************************************************************************/
void inscntrl(char cntrl,unsigned x,unsigned y)
{
    register unsigned i = MAXCOL;
    register unsigned j = x + firstcol + 1;
    while (i >= j) {
      workline.middle[i+1] = workline.middle[i];  /* shift right */
      workline.upper[i+1] = workline.upper[i];    /*  by ignore  */
      workline.topest[i+1] = workline.topest[i];  /*  character  */
      workline.below[i+1] = workline.below[i];    /*   at last   */
      workline.attr[i+1] = workline.attr[i];      /*   column    */
      i--;
    }
    workline.middle[j] = cntrl;   /* put print control character */
    workline.upper[j] = ' ';      /* put blank */
    workline.topest[j] = ' ';     /* put blank */
    workline.below[j] = ' ';      /* put blank */
    workline.attr[j] = fontused;
    storeline(curline);           /* store workline to linked list then  */
    loadtoline(curline->text);    /* load back and redisplay line        */
    refreshline(0,y);             /* to show new attribute of character  */
}
/****************************************************************************/
/*  Wait keyboard for insert control character then insert it to workline   */
/*  at current cursor position.                                             */
/****************************************************************************/
void printcntrl(register unsigned x, register unsigned y)
{
    register int key;

    dispkey(CNTRL_P);
    waitkbd(3,2);
    key = ebioskey(0) & 0xff;
    prchar(key,0,3,2);
    if (!isalpha(key) && !iscntrl(key))
        return;
    switch(key & 0x1f) {
    case 'w' - 'a' + 1 :
        inscntrl(CNTRL_W,x,y);
        break;
    case 's' - 'a' + 1  :
        inscntrl(CNTRL_S,x,y);
        break;
    case 'r' - 'a' + 1  :
        inscntrl(CNTRL_R,x,y);
        break;
    case 'b' - 'a' + 1  :
        inscntrl(CNTRL_B,x,y);
        break;
    case 'e' - 'a' + 1  :
        inscntrl(CNTRL_E,x,y);
        break;
    case 't' - 'a' + 1  :
        inscntrl(CNTRL_T,x,y);
        break;
    case 'v' - 'a' + 1  :
        inscntrl(CNTRL_V,x,y);
        break;
    };
}

/****************************************************************************/
/* insert character 'c' at cursor position *x,*y                            */
/****************************************************************************/
int insert_char(char c,unsigned *x,unsigned *y)
{
    register unsigned i;
    unsigned j;
    if ((*x + firstcol + 1) <= MAXCOL) {   /* if out of line , not insert */
      if (wordwrap && ((*x + firstcol + 1) >= (rightmar + 5))) {
        if (relmargin == NO) {
          autowrap(x,y);
        }
      }
      switch(whatlevel(c)) {
      case MIDDLE :
        if ((*x + firstcol + 1) < leftmar) {  /* cursor before left margin ? */
          if (!relmargin) {    /* release margin ? */
            for (i = *x + firstcol + 1;i != leftmar;i++) {
              insertblank(1,WRAPBLANK);
            }
            gocol(leftmar - 1,x);
            refreshline(0,*y);
          }
        }
        i = strlen(workline.middle);
        j = *x + firstcol + 1;
        while (i >= j) {
          workline.middle[i+1] = workline.middle[i];  /* shift right */
          workline.upper[i+1] = workline.upper[i];    /*  by ignore  */
          workline.topest[i+1] = workline.topest[i];  /*  character  */
          workline.below[i+1] = workline.below[i];    /*   at last   */
          workline.attr[i+1] = workline.attr[i];      /*   column    */
          i--;
        }
        workline.middle[j] = c;      /* put character */
        workline.upper[j] = ' ';     /* put blank */
        workline.topest[j] = ' ';    /* put blank */
        workline.below[j] = ' ';     /* put blank */
        workline.attr[j] = fontused;
        if (haveblock()) {
          if (lineno == blkbegin.lineno) {
            if ((*x + firstcol) <= blkbegin.column) {
              blkbegin.column++;
            }
          }
          if (lineno == blkend.lineno) {
            if ((*x + firstcol) < blkend.column) {
              blkend.column++;
            }
          }
        }
        if ((fontused & ENLARGEATTR) == ENLARGEATTR) {
          i = strlen(workline.middle); /*  if enlarge , insert special column  */
          i++;
          j = *x + firstcol + 2;
          while (i >= j) {
            workline.middle[i+1] = workline.middle[i];
            workline.upper[i+1] = workline.upper[i];
            workline.topest[i+1] = workline.topest[i];
            workline.below[i+1] = workline.below[i];
            workline.attr[i+1] = workline.attr[i];
            i--;
          }
          workline.middle[j] = c;
          workline.below[j] = ENLARGEATTR;
          workline.attr[j] = fontused;
          if (haveblock()) {
            if (lineno == blkbegin.lineno) {
              if ((*x + firstcol) <= blkbegin.column) {
                blkbegin.column++;
              }
            }
            if (lineno == blkend.lineno) {
              if ((*x + firstcol) < blkend.column) {
                blkend.column++;
              }
            }
          }
        }
        break;
      case UPPER :
        i = *x + firstcol;
        if ((workline.middle[i] >= '¡') &&   /* possible character ? */
           (workline.middle[i] <= 'Î')) {
          if (workline.below[i] == ENLARGEATTR) {
            if (c != HUNAKADMITO) {
              workline.upper[i - 1] = c;
            } else {
              workline.topest[i - 1] = 0xe9;
              workline.upper[i - 1] = 0xd1;
            }
          } else {
            if (c != HUNAKADMITO) {
              workline.upper[i] = c;
            } else {
              workline.topest[i] = 0xe9;
              workline.upper[i] = 0xd1;
            }
          }
        } else {
          errorsound();
        }
        break;
      case TOPEST :
        i = *x + firstcol;
        if ((workline.middle[i] >= '¡') &&
           (workline.middle[i] <= 'Î')) {
          if (workline.below[i] == ENLARGEATTR) {
            workline.topest[i - 1] = c;
          } else {
            workline.topest[i] = c;
          }
        } else {
          errorsound();
        }
        break;
      case BELOW :
        i = *x + firstcol;
        if ((workline.middle[i] >= '¡') &
           (workline.middle[i] <= 'Î')) {
          if (workline.below[i] == ENLARGEATTR) {
            workline.below[i - 1] = c;
          } else {
            workline.below[i] = c;
          }
        } else {
          errorsound();
        }
        break;
      }
      if (*x >= (wind.length - 2)) {
        if (whatlevel(c) == MIDDLE) {
          shiftscrn(20,x);
        }
      }
      if (whatlevel(c) == MIDDLE) {
        if ((fontused & ENLARGEATTR) == ENLARGEATTR) {
          (*x)++;
        }
        (*x)++;
      }
      changeflag = YES;
      return(YES);
    } else {
      return(NO);
    }
}
/****************************************************************************/
/* Overwrite character 'c' at cursor position *x,*y                         */
/****************************************************************************/
int ovrwrite_char(char c,unsigned *x,unsigned *y)
{
    register unsigned i;
    unsigned j;
    if ((*x + firstcol + 1) <= MAXCOL) {   /* no write at last column */
      if (wordwrap && ((*x + firstcol + 1) >= (rightmar + 5))) {
        if (relmargin == NO) {
          autowrap(x,y);
        }
      }
      switch(whatlevel(c)) {
      case MIDDLE :
        if ((*x + firstcol + 1) < leftmar) {  /* cursor before left margin ? */
          if (!relmargin) {    /* release margin ? */
            for (i = *x + firstcol + 1;i != leftmar;i++) {
              insertblank(1,WRAPBLANK);
            }
            gocol(leftmar - 1,x);
            refreshline(0,*y);
          }
        }
        if ((workline.attr[*x + firstcol + 1] & ENLARGEATTR) == ENLARGEATTR) {
          i = *x + firstcol + 2;
          while (i < MAXCOL) {
            workline.middle[i] = workline.middle[i+1];
            workline.upper[i] = workline.upper[i+1];
            workline.topest[i] = workline.topest[i+1];
            workline.below[i] = workline.below[i+1];
            workline.attr[i] = workline.attr[i+1];
            i++;
          }
          if (haveblock()) {
            if (lineno == blkbegin.lineno) {
              if ((*x + firstcol) <= blkbegin.column) {
                blkbegin.column--;
              }
            }
            if (lineno == blkend.lineno) {
              if ((*x + firstcol) < blkend.column) {
                blkend.column--;
              }
            }
          }
        }
        i = *x + firstcol + 1;
        if (workline.middle[i] == '\0')
          workline.middle[i+1] = '\0';
        workline.middle[i] = c;
        workline.upper[i] = ' ';
        workline.below[i] = ' ';
        workline.topest[i] = ' ';
        workline.attr[i] = fontused;
        if ((fontused & ENLARGEATTR) == ENLARGEATTR) {
          i = MAXCOL+5;
          j = *x + firstcol + 2;
          while (i >= j) {
            workline.middle[i+1] = workline.middle[i];
            workline.upper[i+1] = workline.upper[i];
            workline.topest[i+1] = workline.topest[i];
            workline.below[i+1] = workline.below[i];
            workline.attr[i+1] = workline.attr[i];
            i--;
          }
          workline.middle[j] = c;
          workline.below[j] = ENLARGEATTR;
          workline.attr[j] = fontused;
          if (haveblock()) {
            if (lineno == blkbegin.lineno) {
              if ((*x + firstcol) <= blkbegin.column) {
                blkbegin.column++;
              }
            }
            if (lineno == blkend.lineno) {
              if ((*x + firstcol) < blkend.column) {
                blkend.column++;
              }
            }
          }
        }
        break;
      case UPPER :
        i = *x + firstcol;
        if ((workline.middle[i] >= '¡') &&
           (workline.middle[i] <= 'Î')) {
          if (workline.below[i] == ENLARGEATTR) {
            if (c != HUNAKADMITO) {
              workline.upper[i - 1] = c;
            } else {
              workline.topest[i - 1] = 0xe9;
              workline.upper[i - 1] = 0xd1;
            }
          } else {
            if (c != HUNAKADMITO) {
              workline.upper[i] = c;
            } else {
              workline.topest[i] = 0xe9;
              workline.upper[i] = 0xd1;
            }
          }
        } else {
          errorsound();
        }
        break;
      case TOPEST :
        i = *x + firstcol;
        if ((workline.middle[i] >= '¡') &&
           (workline.middle[i] <= 'Î')) {
          if (workline.below[i] == ENLARGEATTR) {
            workline.topest[i - 1] = c;
          } else {
            workline.topest[i] = c;
          }
        } else {
          errorsound();
        }
        break;
      case BELOW :
        i = *x + firstcol;
        if ((workline.middle[i] >= '¡') &&
           (workline.middle[i] <= 'Î')) {
          if (workline.below[i] == ENLARGEATTR) {
            workline.below[i - 1] = c;
          } else {
            workline.below[i] = c;
          }
        } else {
          errorsound();
        }
        break;
      }
      if (*x >= (wind.length - 2)) {
        if (whatlevel(c) == MIDDLE) {
          shiftscrn(20,x);
        }
      }
      if  (whatlevel(c) == MIDDLE) {
        if ((fontused & ENLARGEATTR) == ENLARGEATTR) {
          (*x)++;
        }
        (*x)++;
      }
      changeflag = YES;
      return(YES);
    } else {
      return(NO);
    }
}

/****************************************************************************/
/*  Insert return at line & column given,don't update block position.       */
/****************************************************************************/
void insertreturn(struct line_node *line,unsigned thaicol)
{
    font_attr font = 0;
    char fontcode[9],*text;
    struct line_node *line2;
    register int i;

    i = linearcolumn(line->text,thaicol,&font);
    findstrcode(fontcode,font);
    line2 = (struct line_node *) malloc(sizeof(struct line_node));
    line2->wrap = line->wrap;
    line2->graph = NULL;
    line2->text = (char *) malloc(strlen(line->text) + strlen(fontcode) - i + 1);
    strcpy(line2->text,fontcode);
    strcpy((line2->text) + strlen(fontcode),(line->text) + i);
    text = (char *) malloc(i + strlen(fontcode) + 1);
    strncpy(text,line->text,i);
    strcpy(text + i,fontcode);
    free(line->text);
    line->text = text;
    insert_line(line,line2);
}

/****************************************************************************/
/*  Insert return at current line & column given,also update block position.*/
/****************************************************************************/
void insert_ret(unsigned *x)
{
    storeline(curline);
    insertreturn(curline,*x + firstcol);
    curline->wrap = NO;
    if (haveblock()) {
      if (lineno <= blkend.lineno) {
        if (lineno == blkend.lineno) {
          if ((*x + firstcol) < blkend.column) {
            blkend.lineno++;
            blkend.column = blkend.column - *x - firstcol;
            if (lineno == blkbegin.lineno) {
              if ((*x + firstcol) <= blkbegin.column) {
                blkbegin.lineno++;
                blkbegin.column = blkbegin.column - *x - firstcol;
              }
            }
          }
        } else {
          if (lineno <= blkbegin.lineno) {
            if (lineno == blkbegin.lineno) {
              if ((*x + firstcol) <= blkbegin.column) {
                blkbegin.lineno++;
                blkbegin.column = blkbegin.column - *x - firstcol;
              }
            } else {  /* so -> lineno < blkbegin.lineno */
              blkbegin.lineno++;
            }
          }
          blkend.lineno++;
        }
      }
    }
    loadtoline(curline->text);
    pagecomplete = NO;
    changeflag = YES;
}

/****************************************************************************/
/*  Return cursor to first column of next line of current line.             */
/****************************************************************************/
void returnkey(unsigned *x, register unsigned y)
{
    if (firstcol != 0) {
      firstcol = 0;
      pagecomplete = NO;
    }
    *x = 0;
    if (curline->next != sentinel) {
      storeline(curline);
      lineno++;
      if (y == (wind.width - 1)) {
        if ((pagebreak == YES) && (((lineno - 1) % lineperpage) == 0)) {
          curpage = curpage->next;
        }
        curpage = curpage->next;
        curline = curline->next;
        pagecomplete = NO;
      } else {
        if ((pagebreak == YES) && (y == (wind.width - 2)) &&
           (((lineno - 1) % lineperpage) == 0)) {
              curpage = curpage->next;
              pagecomplete = NO;
           }
        curline = curline->next;
      }
      loadtoline(curline->text);
    } else {
      errorsound();
    }
}

/****************************************************************************/
/*  Insert return then return cursor.                                       */
/****************************************************************************/
void ret_with_ins(unsigned *x,unsigned y)
{
    insert_ret(x);
    returnkey(x,y);
}

/****************************************************************************/
/*  Insert macro word at current cursor position.                           */
/****************************************************************************/
void insertmacro(char *macro,unsigned *x,unsigned *y)
{
    int quit = NO;
    while ((*macro != '\0') && (quit == NO)){
      if (!insert_char(*macro,x,y)) {
        linetoolong();
        quit = YES;
      }
      macro++;
    }
    refreshline(0,*y);
}

void blankmaro(int y)
{
    /*
    dispstrhgc(" – CTRL-F   =                                     – ",19-CENTER_FACTOR,y,2);
    */
    dispprintf(19-CENTER_FACTOR, y, 2, " – CTRL-F   =%37s– ", " ");
}

/* Modify by Suttpong Sat  08-05-1989  03:07:24 */
void dispmacro(register int i)
{
    static char *numstr[] = {
        "1", "2", "3", "4", "5",
        "6", "7", "8", "9", "10"
    };

    blankmaro(5+i);
    dispstrhgc(numstr[i],28-CENTER_FACTOR,5+i,2);
    dispstrhgc(&macro[i][0],33-CENTER_FACTOR,5 + i,2);
}

void editmacro(void)
{
    register int i,c;
    pagecomplete = NO;
    dispstrhgc(" ˜••••••••••••••••••••••••••••••••••••••••••••••••™ ",19-CENTER_FACTOR,4,2);
    for (i = 0;i < 10;i++) {
      dispmacro(i);
    }
    dispstrhgc(" š•¡´ <ESC> à¾×èÍàÅÔ¡¡ÒÃá¡éä¢¤ÓÂèÍ•••••••••••••••••••••› ",19-CENTER_FACTOR,15,2);
    i = 0;
    do {
      c = getstring(&macro[i][0],33-CENTER_FACTOR,5 + i,35,0,THAIENG);
      switch(c) {
      case UPKEY  : dispmacro(i);
                    if (i == 0)
                      i = 9;
                    else
                      i--;
                    break;
      case YES    :
      case DNKEY  : dispmacro(i);
                    if (i == 9)
                      i = 0;
                    else
                      i++;
                    break;
      case ESCKEY : writelanguage();
                    return;
      }
    } while(1);
}
