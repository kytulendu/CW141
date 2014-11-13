/* Updated: Suttipong Kanakakorn
            Sun  08-06-1989  18:24:10
*/

#include <stdlib.h>
#include <dir.h>
#include <stdio.h>
#include <string.h>

#include "kbdcode.h"
#include "cwtype.h"
#include "proto.h"

int selectfile(char mask[]);

/*****  private prototype function */
int createdir(char mask[]);
void showfile(int col,int row,char attr);
void showpagedir(void);
void freedir(void);
void dirpgup(void);
void setdirpos(struct dirnode *namewant);
void dirpgdn(void);
void dirup(void);
void dirdown(void);
void dirright(void);
void dirleft(void);


#define DIRCOLMAX 5
#define DIRROWMAX 5

struct dirnode {
       char filename[13];
       struct dirnode *previous;
       struct dirnode *next;
};

static struct dirnode *headdir, *dirpage;

static int dircol;
static int dirrow;

int createdir(char mask[])
{
    struct dirnode *tempdir;
    struct ffblk ffblock;
    int done,i;
    char *source;
    headdir->previous = headdir;
    headdir->next = headdir;
    tempdir = headdir;
    done = findfirst(mask,&ffblock,0);
    while (!done) {
      tempdir->next = (struct dirnode *) malloc(sizeof(struct dirnode));
      if (tempdir->next == NULL) {
        errorsound();
        blockmsg(10);
        dispstrhgc("ÀπË«¬§«“¡®”‰¡ËæÕ ! °¥ <ESC> ‡æ◊ËÕ∑”ß“πµËÕ",22,10,2);
        while (ebioskey(0) != ESCKEY);
        freedir();
        return(NO);
      }
      source = ffblock.ff_name;
      i = 0;
      while (*source != '\0') {
        (tempdir->next)->filename[i] = *(source++);
        i++;
      }
      (tempdir->next)->filename[i] = '\0';
      (tempdir->next)->previous = tempdir;
      tempdir = tempdir->next;
      tempdir->next = headdir;
      headdir->previous = tempdir;
      done = findnext(&ffblock);
    }
    return(YES);
}

void showfile(int col,int row,char attr)
{
    struct dirnode *tempdir;
    int count;
    tempdir = dirpage;
    for (count = (DIRCOLMAX * row) + col;(count != 0) && (tempdir != headdir);count--)
      tempdir = tempdir->next;
    if (tempdir != headdir)
      dispstrhgc(tempdir->filename,col*13 + 9,row+9,attr);
}

void showpagedir(void)
{
    int col,row;
    /*
    dispstrhgc(" òïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïô ",9-CENTER_FACTOR,8,2);
    dispstrhgc(" ñ                                                                   ñ ",9-CENTER_FACTOR,9,2);
    dispstrhgc(" ñ                                                                   ñ ",9-CENTER_FACTOR,10,2);
    dispstrhgc(" ñ                                                                   ñ ",9-CENTER_FACTOR,11,2);
    dispstrhgc(" ñ                                                                   ñ ",9-CENTER_FACTOR,12,2);
    dispstrhgc(" ñ                                                                   ñ ",9-CENTER_FACTOR,13,2);
    dispstrhgc(" öïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïïõ ",9-CENTER_FACTOR,14,2);
    */
    framebox(4, 8, 4+70, 14, 2);
    for (row = 0;row != DIRROWMAX;row++)
      for (col = 0;col != DIRCOLMAX;col++)
        showfile(col,row,2);
}

void freedir(void)
{
    struct dirnode *tempdir,*tempdir2;
    tempdir = headdir->next;
    while (tempdir != headdir) {
      tempdir2 = tempdir;
      tempdir = tempdir->next;
      free(tempdir2);
    }
}
void dirpgup(void)
{
    struct dirnode *tempdir;
    int count,countrow;
    tempdir = dirpage;
    for (countrow = DIRROWMAX;(countrow != 0) && (tempdir->previous != headdir);countrow--)
      for (count = DIRCOLMAX;(count != 0) && (tempdir->previous != headdir);count--)
        tempdir = tempdir->previous;
    if (tempdir->previous != headdir)
      dirpage = tempdir;
    else {
      dirpage = headdir->next;
      dirrow = 0;
    }
    showpagedir();
}
void setdirpos(struct dirnode *namewant)
{
    int count;
    struct dirnode *tempdir;
    tempdir = dirpage;
    dirrow = 0;
    dircol = 0;
    while(tempdir != namewant) {
      for (count = (DIRCOLMAX - 1);(count != 0) && (tempdir != namewant);count--) {
        tempdir = tempdir->next;
        dircol++;
      }
      if (tempdir != namewant){
        dirrow++;
        dircol = 0;
        tempdir = tempdir->next;
      }
    }
}
void dirpgdn(void)
{
  struct dirnode *tempdir;
  int count,countrow;
  tempdir = dirpage;
  for (countrow = DIRROWMAX;(countrow != 0) && (tempdir->next != headdir);countrow--){
    for (count = DIRCOLMAX;(count != 0) && (tempdir->next != headdir);count--)
        tempdir = tempdir->next;
    if (tempdir->next != headdir)
       dirpage = tempdir;
  }
  for (count = dircol;(count != 0) && (tempdir->next != headdir);count--)
      tempdir = tempdir->next;
  showpagedir();
  setdirpos(tempdir);
}
void dirup(void)
{
  struct dirnode *tempdir;
  int count;
  tempdir = dirpage;
  for (count = DIRCOLMAX;(count != 0) && (tempdir->previous != headdir);count--)
       tempdir = tempdir->previous;
  if (count == 0){
     dirpage = tempdir;
     showpagedir();
  }
}

void dirdown(void)
{
  struct dirnode *tempdir,*tempdir2;
  int count;
  tempdir = dirpage;
  for (count = (DIRCOLMAX * dirrow);(count != 0) && (tempdir->next != headdir);count--)
      tempdir = tempdir->next;
  tempdir2 = dirpage;
  for (count = (DIRCOLMAX - 1);(count != 0) && (tempdir->next != headdir);count--){
      tempdir = tempdir->next;
      tempdir2 = tempdir2->next;
  }
  if (tempdir->next != headdir){
     tempdir2 = tempdir2->next;
     if (dirrow == (DIRROWMAX - 1)){
        dirpage = tempdir2;
        showpagedir();
     }
     for (count = (dircol + 1);(count != 0) && (tempdir->next != headdir);count--)
         tempdir = tempdir->next;
     setdirpos(tempdir);
  }
}
void dirright(void)
{
  struct dirnode *tempdir;
  int count;
  tempdir = dirpage;
  for (count = (DIRCOLMAX * dirrow) + dircol;count != 0;count--)
      tempdir = tempdir->next;
  if (tempdir->next != headdir)
     dircol++;
}
void dirleft(void)
{
  struct dirnode *tempdir;
  int count;
  tempdir = dirpage;
  for (count = (DIRCOLMAX * dirrow) + DIRCOLMAX - 1;(count != 0) && (tempdir->next != headdir);count--)
      tempdir = tempdir->next;
  setdirpos(tempdir);
}
int selectfile(char mask[])
{
    struct dirnode *tempdir;
    int count,c,i,j;
    if (createdir(mask) == NO) return(NO);
    if (headdir->next != headdir){
      dirpage = headdir->next;
      dircol = 0;
      dirrow = 0;
      showpagedir();
      showfile(dircol,dirrow,0);
      do{
        c = ebioskey(0);
        switch(c) {
        case UPKEY : showfile(dircol,dirrow,2);
                     if (dirrow == 0)
                        dirup();
                     else
                        dirrow--;
                     showfile(dircol,dirrow,0);
                     break;
        case DNKEY : showfile(dircol,dirrow,2);
                     dirdown();
                     showfile(dircol,dirrow,0);
                     break;
        case RIKEY : showfile(dircol,dirrow,2);
                     if (dircol == (DIRCOLMAX-1))
                        dircol = 0;
                     else
                        dirright();
                     showfile(dircol,dirrow,0);
                     break;
        case LEKEY : showfile(dircol,dirrow,2);
                     if (dircol == 0)
                        dirleft();
                     else
                        dircol--;
                     showfile(dircol,dirrow,0);
                     break;
        case PGUPKEY : dirpgup();
                       showfile(dircol,dirrow,0);
                       break;
        case PGDNKEY : dirpgdn();
                       showfile(dircol,dirrow,0);
                       break;
        case ESCKEY : freedir();
                      mask[0] = '\0';
                      return(NO);
        };
      } while (c != RETKEY);
      tempdir = dirpage;
      for (count = (DIRCOLMAX * dirrow) + dircol;(count != 0) && (tempdir->next != headdir);count--)
        tempdir = tempdir->next;
      j = strlen(mask);
      while ((j != 0) && (mask[j] != '\\') && (mask[j] != ':'))
        j--;
      if (j != 0) j++;
      for (i = 0;i < 13;i++)
        mask[j++] = tempdir->filename[i];
      freedir();
      return(YES);
    } else {
      mask[0] = '\0';
      return(NO);
    }
}
