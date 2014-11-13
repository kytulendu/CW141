/****************************************************************************/
/*  EDIT.C 15 JAN 89                                                        */
/****************************************************************************/
#include "inc.h"
#include "..\common\cwgrphc.h"

/****************************************************************************/
/*  From attribute given,find string of ascii control code.                 */
/****************************************************************************/
void findstrcode(char fontcode[], font_attr attr)
{
    int i = 0;
    if ((attr & ONELINEATTR) != 0){
      fontcode[i] = ONELINECODE;
      i++;
    }
    if ((attr & SUPERATTR) != 0){
      fontcode[i] = SUPERCODE;
      i++;
    }
    if ((attr & SUBATTR) != 0){
      fontcode[i] = SUBCODE;
      i++;
    }
    if ((attr & ITALICATTR) != 0){
      fontcode[i] = ITALICCODE;
      i++;
    }
    if ((attr & BOLDATTR) != 0){
      fontcode[i] = BOLDCODE;
      i++;
    }
    if ((attr & TWOLINEATTR) != 0){
      fontcode[i] = TWOLINECODE;
      i++;
    }
    if ((attr & ENLARGEATTR) != 0){
      fontcode[i] = ENLARGECODE;
      i++;
    }
    fontcode[i] = '\0';
}

/****************************************************************************/
/*  Find line number from line pointer given.                               */
/****************************************************************************/
unsigned findlineno(struct line_node *line)
{
    register unsigned linenumber = 1;
    struct line_node *templine;
    templine = sentinel->next;
    while (templine != line){
      templine = templine->next;
      linenumber++;
    }
    return(linenumber);
}

/****************************************************************************/
/*  Find line pointer from line number given.                               */
/****************************************************************************/
struct line_node *linepointer(unsigned linenum)
{
    struct line_node *line;
    line = sentinel->next;
    while (linenum-- > 1) {
      line = line->next;
    }
    return(line);
}

/****************************************************************************/
/*  Display line from link list at row given. First column to be displayed  */
/*  is in global variable "firstcol".                                       */
/****************************************************************************/
void displine(struct line_node *line,unsigned y,unsigned linenum)
{
    register unsigned x = 0;
    int count = firstcol;
    font_attr tempfont, curfont = 0;
    char *st;

    st = line->text;
    y += wind.row;
    while ((count > 0) && (*st != '\0')) {
      if (whatlevel(*st) == MIDDLE) {
        if (*st < 32) {
          togglefont(&curfont,*st);
        } else {
          if ((curfont & ENLARGEATTR) == ENLARGEATTR) {
            count--;
          }
          count--;
        }
      }
      st++;
    }
    if ((count < 0) && ((curfont & ENLARGEATTR) == ENLARGEATTR)) {
      x++;
    }
    while ((*st != '\0') && (x < (wind.length - 2))){
      if (*st < 32) {
        togglefont(&curfont,*st);
      } else {
        tempfont = curfont;
        if (haveblock() && inblock(linenum,x + firstcol) && dispblock) {
          curfont = curfont | REVERSEATTR;
        }
        if (whatlevel(*st) != MIDDLE) {
          if (x > 0) {
            if ((curfont & ENLARGEATTR) == ENLARGEATTR) {
              if (x >= 2) {
                prchar(*st,curfont,wind.col+x-2,y);
              }
            } else {
              prchar(*st,curfont,wind.col+x-1,y);
            }
          }
        } else {
          if ((*st != ' ') || (curfont != 0)) {
            if ((curfont & ENLARGEATTR) != ENLARGEATTR) {
              prchar(*st,curfont,wind.col+x,y);
              x++;
            } else {
              prchar(*st,curfont,wind.col+x,y);
              x += 2;
            }
          } else { /* if it's blank and normal attribute,use prblank to speed up */
            prblank(wind.col+x,y);
            x++;
          }
        }
        curfont = tempfont;
      }
      st++;
    }
    clrline(wind.col+x,y,wind.col+wind.length-1);
    if (line->wrap == NO) {
       prchar('<',0,wind.col + wind.length - 1,y);
    }
    if (*st != '\0') {
      prchar('+',0,wind.col + wind.length - 1,y);
    }
    y -= wind.row;
    /*
    if (graphbuff[y] != NULL)
      paintlinegraph(graphbuff[y],y);
    */
}

/****************************************************************************/
/*  Display page break line at row given.                                   */
/****************************************************************************/
void disppagebreak(unsigned y)
{
    register unsigned i = 0,j;
    y += wind.row;
    j = wind.col + wind.length - 1;
    while (i != j) {
      prchar('-',0,wind.col + i,y);
      i++;
    }
    prchar('P',0,i,y);
}

/****************************************************************************/
void set_graphic_buffer()
{
/*    unsigned count,i,j,m,n;
    unsigned linenum = lineno,linenum2;
    struct line_node *templine;
    char *gr;

    templine = curpage;
    while (templine != curline) {    /* find lineno of curpage */
      linenum--;
      templine = templine->next;
    }
    j = 0;
    while (j < wind.width) {
      if (graphbuff[j] != NULL)
        free(graphbuff[j]);
      graphbuff[j] = NULL;
      j++;
    }

 return;

    templine = curpage;
    count = 0;
    while ((templine->graph == NULL) &&
           (count != 66) && (templine->previous != sentinel)) {
       templine = templine->previous;
       count++;
    }
    if (templine->graph != NULL) {
      gr = templine->graph;
      gr += strlen(gr) + 4;
      while ((count != 0) && (*gr != 0x80)) {
        m = 0;
        while ((m != 20) && (*gr != 0x80)) {
          gr += strlen(gr) + 1;
          m++;
        }
        count--;
      }
      linenum2 = linenum;
      i = 0;
      while ((*gr != 0x80) && (i < wind.width)) {
        m = 0;
        n = 0;
        while ((m != 20) && (gr[n] != 0x80)) {
          n += strlen(gr + n) + 1;
          m++;
        }
        graphbuff[i] = (char *) malloc(n);
        memcpy(graphbuff[i],gr,n);
        gr += n;
        i++;
        linenum2++;
        if ((pagebreak == YES) &&
          (((linenum2 - 1) % lineperpage) == 0)) {
          if (i != wind.width)
            i++;
        }
      }
    }

    templine = curpage;
    i = 0;
    while ((i < wind.width) && (templine != sentinel)) {
      if (templine->graph != NULL) {
        j = i;
        linenum2 = linenum;
        gr = templine->graph;
        gr += strlen(templine->graph) + 4;
        while ((*gr != 0x80) && (j < wind.width)) {
          m = 0;
          n = 0;
          while ((m != 20) && (gr[n] != 0x80)) {
            n += strlen(gr + n) + 1;
            m++;
          }
          graphbuff[j] = (char *) malloc(n);
          memcpy(graphbuff[j],gr,n);
          gr += n;
          linenum2++;
          j++;
          if ((pagebreak == YES) &&
            (((linenum2 - 1) % lineperpage) == 0)) {
            if (j != wind.width)
              j++;
          }
        }
      }
      templine = templine->next;
      i++;
      linenum++;
      if ((pagebreak == YES) &&
        (((linenum - 1) % lineperpage) == 0)) {
        if (i != wind.width)
          i++;
      }
    }   */
}

/****************************************************************************/
/*  Display all line in page from linked list. Stop display if keyboard is  */
/*  pressed,then set flag "pagecomplete" to NO,else "pagecomplete" is set   */
/*  to YES.                                                                 */
/****************************************************************************/
void showpage(void)
{
    register unsigned y=0,linenum = lineno;
    struct line_node *temppage = curpage;
/*    set_graphic_buffer();   */
    while (temppage != curline) {    /* find lineno of curpage */
      linenum--;
      temppage = temppage->next;
    }
    temppage = curpage;
    while ((y != wind.width) && (temppage != sentinel)){
      if (keypressed()){
        pagecomplete = NO;
        return;
      }
      if (temppage != curline) {
        displine(temppage,y,linenum);  /* display line from linked list */
      } else {
        refreshline(0,y);              /* display line from workline */
      }
      temppage = temppage->next;
      linenum++;
      y++;
      if ((pagebreak == YES) &&
         (((linenum - 1) % lineperpage) == 0)){
        if (y != wind.width){
          disppagebreak(y);
          y++;
        }
      }
    }
    if (temppage == sentinel){
      while (y != wind.width){
        if (keypressed()){
          pagecomplete = NO;
          return;
        }
        clrline(wind.col,wind.row+y,wind.col+wind.length-1);
        prchar('.',0,wind.col + wind.length - 1,wind.row + y);
        y++;
      }
    }
    writetab();
    pagecomplete = YES;
    return;
}

/****************************************************************************/
/*  Show all lines in page,ignore pressing keyboard.                        */
/****************************************************************************/
void showpageall(void)
{
    if (filename[0] != '\0') {
      do {
        showpage();
        if (!pagecomplete) {
          ebioskey(0);
        }
      } while (!pagecomplete);
    } else {
      cls();
    }
}

/****************************************************************************/
/*  Find value of current row from global variable "curpage" and "curline". */
/****************************************************************************/
unsigned findrow(void)
{
  register unsigned row = 0,linenum = lineno;
  struct line_node *temppage = curpage;
  while (temppage != curline) {
    linenum--;
    temppage = temppage->next;
  }
  temppage = curpage;
  while (temppage != curline) {
    if ((pagebreak == YES) &&
      (((linenum - 1) % lineperpage) == (lineperpage - 1))) {
      row++;
    }
    temppage = temppage->next;
    linenum++;
    row++;
  }
  return(row);
}

/****************************************************************************/
/*  Adjust column to right position.                                        */
/****************************************************************************/
void adjustcol(unsigned *x)
{
  if ((*x + firstcol) >= strlen(workline.middle)) {
    if (firstcol > (strlen(workline.middle) - 1)) {
      firstcol = strlen(workline.middle) - 1;
      pagecomplete = NO;
    }
    *x = strlen(workline.middle) - 1 - firstcol;
    if (*x > (wind.length - 2)) {
      firstcol += *x - (wind.length - 2);
      pagecomplete = NO;
    }
  }
  if (*x > 0){
    if (workline.below[*x + firstcol + 1] == ENLARGEATTR)
      (*x)--;
  } else {
    if ((workline.below[*x + firstcol + 1] == ENLARGEATTR) &&
       (firstcol != 0)) {
      firstcol--;
      pagecomplete = NO;
    }
  }
}

/****************************************************************************/
/*  Load line from linked list to workline.                                 */
/****************************************************************************/
void loadtoline(char *address)
{
    register unsigned index = 1;
    font_attr curfont,oldfont;

    workline.middle[0] = ' ';
    while (index <= (MAXCOL+1)) {
      workline.topest[index] = ' ';
      workline.upper[index] = ' ';
      workline.middle[index] = ' ';
      workline.below[index] = ' ';
      workline.attr[index] = 0x00;
      index++;
    }
    index = 0;
    curfont = 0;
    oldfont = 0;
    while ((*address != '\0') && (index <= MAXCOL)) {
      if (*address < 32) {
         togglefont(&curfont,*address);
      } else {
         switch(whatlevel(*address)) {
         case MIDDLE : index++;
                       if ((oldfont & ENLARGEATTR) == ENLARGEATTR){
                         workline.attr[index] = oldfont;
                         workline.middle[index] = workline.middle[index-1];
                         workline.below[index] = ENLARGEATTR;
                         index++;
                       }
                       workline.middle[index] = (*address);
                       workline.attr[index] = curfont;
                       break;
         case UPPER  : workline.upper[index] = (*address);
                       break;
         case TOPEST : workline.topest[index] = (*address);
                       break;
         case BELOW  : workline.below[index] = (*address);
                       break;
         }
         oldfont = curfont;
      }
      address++;
    }
    index++;
    if ((oldfont & ENLARGEATTR) == ENLARGEATTR) {
      workline.attr[index] = oldfont;
      workline.middle[index] = workline.middle[index-1];
      workline.below[index] = ENLARGEATTR;
      index++;
    }
    workline.middle[index] = '\0';
}
/****************************************************************************/
/*  Store line from workline to linked list                                 */
/****************************************************************************/
void storeline(struct line_node *curline)
{
     unsigned count,col;
     static char oneline[MAXCOL*5];
     char oldfont,fontcode[9];
     char *keep_ptr;             /************ Modified *************/
     count = 0;
     col = 1;
     oldfont = 0;
     while ((workline.middle[col] != '\0') && (col <= MAXCOL)) {
       if (workline.attr[col] != oldfont){
         if (oldfont != 0){
            findstrcode(fontcode,oldfont);
            strcpy(&oneline[count],fontcode);
            count += strlen(fontcode);
         }
         findstrcode(fontcode,workline.attr[col]);
         strcpy(&oneline[count],fontcode);
         count += strlen(fontcode);
       }
       oldfont = workline.attr[col];
       oneline[count] = workline.middle[col];
       count++;
       if (workline.below[col] != ' '){
         oneline[count] = workline.below[col];
         count++;
       }
       if (workline.upper[col] != ' '){
         oneline[count] = workline.upper[col];
         count++;
       }
       if (workline.topest[col] != ' '){
         oneline[count] = workline.topest[col];
         count++;
       }
       if ((oldfont & ENLARGEATTR) == ENLARGEATTR) {
         col++;
       }
       col++;
     }
     if (oldfont != 0){
       findstrcode(fontcode,oldfont);
       strcpy(&oneline[count],fontcode);
       count += strlen(fontcode);
     }
     oneline[count] = '\0';

     keep_ptr = curline->text;
     curline->text = (char *) malloc(count+1);
     if (curline->text != NULL)             /****** Modified ******/
     {
        if (keep_ptr != NULL)
           free(keep_ptr);
        strcpy(curline->text,oneline);
     }
     else
     {
       curline->text = keep_ptr;
       dispstrhgc ("Internal error in storeline",1,1,2);
       getch ();
     }
}
/****************************************************************************/
/*  Display line from workline at row given.Start display from column given.*/
/****************************************************************************/
void refreshline(unsigned x,unsigned y)
{
    register int i,len;
    char attr;
    len = wind.length - 2;
    y += wind.row;
    if (x == 0) {
      if (workline.below[firstcol+x+1] == ENLARGEATTR){
        prchar(' ',workline.attr[firstcol+x+1],wind.col+x,y);
        x++;
      }
    } else {
      if (workline.below[firstcol+x] != ENLARGEATTR) {
         x--;
      } else {
        if (x >= 2) {
          x = x - 2;
        }
      }
    }
    i = firstcol + x + 1;
    while ((x < len) && (workline.middle[i] != '\0')) {
      attr = workline.attr[i];
      if (haveblock() && inblock(lineno,i-1) && dispblock) {
        attr = attr | REVERSEATTR;
      }
      if ((workline.middle[i] != ' ') || (attr != 0)) {
        prchar(workline.middle[i],attr,wind.col+x,y);
        if (workline.below[i] != ' ')
          prchar(workline.below[i],attr,wind.col+x,y);
        if (workline.upper[i] != ' ')
          prchar(workline.upper[i],attr,wind.col+x,y);
        if (workline.topest[i] != ' ')
          prchar(workline.topest[i],attr,wind.col+x,y);
      } else {
        prblank(wind.col+x,y);
      }
      if ((workline.attr[i] & ENLARGEATTR) == ENLARGEATTR) {
        x++;
      }
      x++;
      i = firstcol + x + 1;
    }
    prblank(wind.col + wind.length - 2,y);
    if (workline.middle[i] == '\0') {
      for(;x < len;x++)
        prblank(wind.col+x,y);
      if (curline->wrap == NO) {
        prchar('<',0,wind.col + wind.length - 1,y);
      } else {
        prblank(wind.col + wind.length - 1,y);
      }
    } else {
      prchar('+',0,wind.col + wind.length - 1,y);
    }
    y -= wind.row;
    /*
    if (graphbuff[y] != NULL) {
      paintlinegraph(graphbuff[y],y);
    }
    */
}
