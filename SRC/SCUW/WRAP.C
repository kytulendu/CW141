/* Updated: Suttipong Kanakakorn
            Thu  08-03-1989  01:05:26
*/

#include "inc.h"

/****************************************************************************/
/*  Insert blank code (bit 8 = 1) to justify text to right margin. Text     */
/*  must be in workline,not linked list.                                    */
/****************************************************************************/
void justify_right(void)
{
    register int count,i,j,k,already = NO;
    for(i=strlen(workline.middle) - 1;
        ((workline.middle[i] == ' ') || (workline.middle[i] == WRAPBLANK)) &&
         (i > 0);
        i--);
    if ((i < rightmar) && (i > 0)) {
      for(j=1;
          (workline.middle[j] == ' ') || (workline.middle[j] == WRAPBLANK);
          j++);
      k = j;
      count = rightmar - i;
      while (count > 0) {
        while ((workline.middle[k] != ' ') &&
               (workline.middle[k] != WRAPBLANK) &&
               (k < i)) {
          k++;
        }
        if (k < i) {
          insertblank(k,WRAPBLANK);
          count--;
          i++;
          already = YES;
        }
        while (((workline.middle[k] == ' ') || (workline.middle[k] == WRAPBLANK))
               && (k < i)) {
          k++;
        }
        if (k >= i) {
          if (already == NO) return;
          k = j;
        }
      }
    }
}

/****************************************************************************/
/*  Reform paragraph from line specified to end of paragraph.               */
/****************************************************************************/
void reform(void)
{
    struct line_node *templine,*firstline,*freeline;
    unsigned char *temp1,*temp2,*temp3,*cuthere;
    unsigned i,j;
    font_attr font = 0;
    char fontcode[9];

    storeline(curline);
    firstline = curline;
    templine = firstline;  /* start to concatenate all lines to one line only */
    temp1 = (char *) malloc(1);
    *temp1 = '\0';
    while (1) {
        temp2 = (char *) malloc(strlen(templine->text) + 1);
        i = 0;
        j = 0;
        while (templine->text[i] != '\0') {
          if (templine->text[i] != WRAPBLANK) {
            temp2[j] = templine->text[i];
            j++;
          }
          i++;
        }
        temp2[j] = '\0';
        temp3 = (char *) malloc(strlen(temp1) + strlen(temp2) + 1);
        strcpy(temp3,temp1);
        strcat(temp3,temp2);
        free(temp1);
        free(temp2);
        if (templine->wrap == NO) break;
        temp1 = temp3;
        templine = templine->next;
    }
/* now paragraph is in the one line only pointed by temp3 */
/* so we will compress two or more blanks to one blank only */
    compress_blank(temp3);

    templine->previous = firstline->previous;
    (templine->previous)->next = templine;
    free(templine->text);
    templine->text = temp3;
    if (curline == curpage) {
      curpage = templine;
    }
    curline = templine;
    while (firstline != templine) {
      freeline = firstline;
      firstline = firstline->next;
      free(freeline->text);
      if (freeline->graph != NULL)
        free(freeline->graph);
      free(freeline);
    }
    while (thaistrlen(curline->text) > (rightmar - leftmar + 1)) {
      i = rightmar - leftmar;   /*   + 1;   */
      temp1 = curline->text;
      while ((i != 0) && (*temp1 != '\0')) {
        for (temp1++;whatlevel(*temp1) != MIDDLE;temp1++);
        i--;
      }
      temp3 = temp1;
      for (i = 10;(i != 0) && (*temp3 != '\0');i--)
        temp3++;
      cuthere = FINDCUT(curline->text,temp3,temp1);
      cuthere++;
      font = 0;
      for (temp1 = curline->text;temp1 != cuthere;temp1++) {
        if (*temp1 < 32) togglefont(&font,*temp1);
      }
      findstrcode(fontcode,font);
      templine = (struct line_node *) malloc(sizeof(struct line_node));
      templine->text = (char *) malloc(strlen(fontcode) + strlen(cuthere) + 1);
      templine->wrap = NO;
      templine->graph = NULL;
      strcpy(templine->text,fontcode);
      strcat(templine->text,cuthere);
      *cuthere = '\0';
      temp3 = (char *) malloc(leftmar + strlen(curline->text) + strlen(fontcode));
      i = 0;
      while (i != (leftmar - 1)) {
        temp3[i] = WRAPBLANK;
        i++;
      }
      temp3[i] = '\0';
      strcat(temp3,curline->text);
      strcat(temp3,fontcode);
      free(curline->text);
      curline->text = temp3;
      templine->next = curline->next;
      (curline->next)->previous = templine;
      templine->previous = curline;
      curline->next = templine;
      curline->wrap = YES;
      loadtoline(curline->text);
      justify_right();
      storeline(curline);
      curline = curline->next;
    }
    temp3 = (char *) malloc(strlen(curline->text) + leftmar);
    i = 0;
    while (i != (leftmar - 1)) {
      temp3[i] = WRAPBLANK;
      i++;
    }
    temp3[i] = '\0';
    strcat(temp3,curline->text);
    free(curline->text);
    curline->text = temp3;
    if (curline->next != sentinel)
      curline = curline->next;
    lineno = findlineno(curline);
    loadtoline(curline->text);
    while (findrow() > (wind.width - 1)) {
      curpage = curpage->next;
    }
    changeflag = YES;
    pagecomplete = NO;
    blkbegin.lineno = 1;
    blkend.lineno = 1;
    blkbegin.column = 0;
    blkend.column = 0;
}

void manualwrap(unsigned *x,unsigned *y)
{
    int i;
    insert_ret(x);
    curline->wrap = YES;
    *x = (leftmar - 1) + (*x - thaistrlen(curline->text));
    justify_right();
    *y = findrow();
    cursor_down(*y);
    for (i=1;i != leftmar;i++)
      workline.middle[i] = WRAPBLANK;
}

void autowrap(unsigned *x,unsigned *y)
{
    unsigned i,j,already = NO,diff;
    char *temp1,*temp3,*cuthere,fontcode[9];
    font_attr font = 0;
    struct line_node *templine;

    storeline(curline);
    diff = strlen(workline.middle) - (*x  + firstcol + 1);
    i = 0;
    j = 0;
    while (curline->text[i] != '\0') {
      if (curline->text[i] != WRAPBLANK) {
        curline->text[j] = curline->text[i];
        j++;
        already = YES;
      } else {
        if (already == NO) {
          curline->text[j] = curline->text[i];
          j++;
        }
      }
      i++;
    }
    curline->text[j] = '\0';
    temp1 = curline->text;
    for (i = rightmar - 2;(i > 0) && (*temp1 != '\0');i--) {
      temp1++;
      while (whatlevel(*temp1) != MIDDLE)
        temp1++;
    }
    temp3 = temp1;
    for (i = 10;(i != 0) && (*temp3 != '\0');i--)
      temp3++;
    cuthere = FINDCUT(curline->text,temp3,temp1);
    cuthere++;
    for (temp1 = curline->text;temp1 != cuthere;temp1++) {
      if (*temp1 < 32) togglefont(&font,*temp1);
    }
    findstrcode(fontcode,font);
    templine = (struct line_node *) malloc(sizeof(struct line_node));
    templine->text = (char *) malloc(leftmar + strlen(fontcode) + strlen(cuthere));
    for (i=0;i != (leftmar - 1);i++)
      templine->text[i] = WRAPBLANK;
    templine->text[i] = '\0';
    strcat(templine->text,fontcode);
    strcat(templine->text,cuthere);
    templine->graph = NULL;
    templine->wrap = (curline->next)->wrap;
    *cuthere = '\0';
    temp3 = (char *) malloc(strlen(curline->text) + strlen(fontcode) + 1);
    strcpy(temp3,curline->text);
    strcat(temp3,fontcode);
    free(curline->text);
    curline->text = temp3;
    templine->next = curline->next;
    (curline->next)->previous = templine;
    templine->previous = curline;
    curline->next = templine;
    curline->wrap = YES;
    loadtoline(curline->text);
    justify_right();
    *y = findrow();
    cursor_down(*y);
    firstcol = 0;
    gocol(strlen(workline.middle) - 1 - diff,x);
    pagecomplete = NO;
}

/* Thu  08-03-1989  01:05:37 */
/* leave the leading blank alone, just compress the rest 
   and trim the trailing blank
*/
void compress_blank(register unsigned char *s)
{
    register unsigned char *r;

    for (; *s == ' '; s++)
        ;
    r = s;
    while (*s++ = *r) {
        if (*r == ' ') {
            for (; *r == ' '; r++)
                ;
        } else
            r++;
    }
    s -= 2;
    if (*s == ' ')
         *s = '\0';
}
