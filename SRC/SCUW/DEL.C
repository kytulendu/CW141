/****************************************************************************/
/*  DEL.C 15 JAN 89                                                         */
/****************************************************************************/
#include "inc.h"
/****************************************************************************/
/*  Delete character of current line at column given.                       */
/****************************************************************************/
void delete_char(unsigned x)
{
    register unsigned i = x + firstcol + 1;
    unsigned j = strlen(workline.middle);
    if (i != j) {
      if ((workline.attr[i] & ENLARGEATTR) == ENLARGEATTR) {
        for (;i < j;i++) {
          workline.middle[i] = workline.middle[i+1];
          workline.upper[i] = workline.upper[i+1];
          workline.topest[i] = workline.topest[i+1];
          workline.below[i] = workline.below[i+1];
          workline.attr[i] = workline.attr[i+1];
        }
        if (haveblock()) {
          if (lineno == blkbegin.lineno) {
            if ((x + firstcol) < blkbegin.column) {
              blkbegin.column--;
            }
          }
          if (lineno == blkend.lineno) {
            if ((x + firstcol) < blkend.column) {
              blkend.column--;
            }
          }
        }
        i = x + firstcol + 1;
      }
      for (;i < j;i++) {
        workline.middle[i] = workline.middle[i+1];
        workline.upper[i] = workline.upper[i+1];
        workline.topest[i] = workline.topest[i+1];
        workline.below[i] = workline.below[i+1];
        workline.attr[i] = workline.attr[i+1];
      }
      if (haveblock()) {
        if (lineno == blkbegin.lineno) {
          if ((x + firstcol) < blkbegin.column) {
            blkbegin.column--;
          }
        }
        if (lineno == blkend.lineno) {
          if ((x + firstcol) < blkend.column) {
            blkend.column--;
          }
        }
      }
    } else {
      del_return();
    }
    changeflag = YES;
}
/****************************************************************************/
/*  Concatenate line given with next line.(Or delete return of line given.) */
/****************************************************************************/
void deletereturn(struct line_node *line)
{
    char *temp;
    struct line_node *linedeleted;
    if (line->next != sentinel) {
      linedeleted = line->next;
      temp = (char *) malloc(strlen(line->text) + strlen(linedeleted->text) + 1);
      strcpy(temp,line->text);
      strcat(temp,linedeleted->text);
      free(line->text);
      line->text = temp;
      line->next = linedeleted->next;
      (linedeleted->next)->previous = line;
      line->wrap = linedeleted->wrap;
      if ((linedeleted->graph != NULL) && (line->graph == NULL)) {
        line->graph = linedeleted->graph;
        linedeleted->graph = NULL;
      }
      if (linedeleted == curline) {
        curline = line;
      }
      if (linedeleted == curpage) {
        curpage = line;
      }
      free(linedeleted->text);
      if (linedeleted->graph != NULL)
        free(linedeleted->graph);
      free(linedeleted);
    }
}

void del_return(void)
{
    if (haveblock()) {
      if (lineno < blkend.lineno) {
        if ((lineno + 1) == blkend.lineno) {
          blkend.column = blkend.column + thaistrlen(curline->text);
        }
        blkend.lineno--;
        if (lineno < blkbegin.lineno) {
          if ((lineno + 1) == blkbegin.lineno) {
            blkbegin.column = blkbegin.column + thaistrlen(curline->text);
          }
          blkbegin.lineno--;
        }
      }
    }
    storeline(curline);
    deletereturn(curline);
    loadtoline(curline->text);
    pagecomplete = NO;
    changeflag = YES;
}

void backspace(unsigned *x)
{
    register int i;
    int enlargeflag = NO;
    i = *x + firstcol;
    if (i > 0) {     /* begin of line ? */
      if (workline.below[i] == ENLARGEATTR) {
        enlargeflag = YES;
        if ((workline.topest[i-1] == ' ') &&
            (workline.upper[i-1] == ' ') &&
            (workline.below[i-1] == ' ')) {
          while (i < MAXCOL) {
            workline.middle[i] = workline.middle[i+1];
            workline.upper[i] = workline.upper[i+1];
            workline.topest[i] = workline.topest[i+1];
            workline.below[i] = workline.below[i+1];
            workline.attr[i] = workline.attr[i+1];
            i++;
          }
        }
        if (haveblock()) {
          if (lineno == blkbegin.lineno)
            if ((*x + firstcol) <= blkbegin.column)
              blkbegin.column--;
          if (lineno == blkend.lineno)
            if ((*x + firstcol) <= blkend.column)
              blkend.column--;
        }
        i = *x + firstcol - 1;
      }
      if (workline.topest[i] != ' ')
              workline.topest[i] = ' ';
      else if (workline.upper[i] != ' ')
              workline.upper[i] = ' ';
      else if (workline.below[i] != ' ')
              workline.below[i] = ' ';
      else {
        while (i < MAXCOL) {
          workline.middle[i] = workline.middle[i+1];
          workline.upper[i] = workline.upper[i+1];
          workline.topest[i] = workline.topest[i+1];
          workline.below[i] = workline.below[i+1];
          workline.attr[i] = workline.attr[i+1];
          i++;
        }
        if (haveblock()) {
          if (lineno == blkbegin.lineno)
            if ((*x + firstcol) <= blkbegin.column)
              blkbegin.column--;
          if (lineno == blkend.lineno)
            if ((*x + firstcol) <= blkend.column)
              blkend.column--;
         }
         if (enlargeflag != YES) {
           if (*x > 0)
             (*x)--;
            else{
             firstcol--;
            storeline(curline);
            pagecomplete = NO;
          }
        } else {
          if (*x >= 2)
            *x = *x - 2;
          else {
            firstcol = firstcol - 2;
            storeline(curline);
            pagecomplete = NO;
          }
        }
      }
    } else {
      if (lineno != 1) {
        cursor_up();
        endline(x);
        del_return();
      }
    }
    changeflag = YES;
}

void delete_word(unsigned x)
{
    int i;
    i = x + firstcol + 1;
    if (workline.middle[i] != '\0') {
      if ((workline.middle[i] != ' ') && (workline.middle[i] != WRAPBLANK)) {
        while ((workline.middle[i] != ' ') && 
               (workline.middle[i] != WRAPBLANK) &&
               (workline.middle[i] != '\0'))
          delete_char(x);
      } else {
        while ((workline.middle[i] == ' ') || (workline.middle[i] == WRAPBLANK))
          delete_char(x);
      }
    } else {
      del_return();
    }
}

void deltoendline(unsigned x,unsigned y)
{
    register unsigned i = MAXCOL + 1;
    unsigned j = x + firstcol + 1;
    while (i <= j) {
      workline.topest[i] = ' ';
      workline.upper[i] = ' ';
      workline.middle[i] = ' ';
      workline.below[i] = ' ';
      workline.attr[i] = 0x00;
      i--;
    }
    workline.middle[j] = '\0';
    refreshline(x,y);
    changeflag = YES;
}
void deleteline(struct line_node *line)
{
    if (line->next != sentinel) {
      if (curline == line) {
        curline = curline->next;
        loadtoline(curline->text);
      }
      if (curpage == line) {
        curpage = curpage->next;
      }
      (line->previous)->next = line->next;
      (line->next)->previous = line->previous;
      free(line->text);
      if (line->graph != NULL)
        free(line->graph);
      free(line);
    } else {
      free(line->text);
      if (line->graph != NULL)
        free(line->graph);
      line->text = (char *) malloc(1);
      *(line->text) = '\0';
      if (curline == line) {
        loadtoline(curline->text);
      }
    }
}
void delete_line(void)
{
    if (haveblock()) {
      if (lineno < blkend.lineno) {
        if (lineno < blkbegin.lineno) {
          blkbegin.lineno--;
        } else {
          if (lineno == blkbegin.lineno) {
            blkbegin.column = 0;
          }
        }
        blkend.lineno--;
      } else {
        if (lineno == blkend.lineno) {
          blkend.column = 0;
        }
      }
    }
    deleteline(curline);
    pagecomplete = NO;
    changeflag = YES;
}