/****************************************************************************/
/*  BLOCK.C 15 JAN 89                                                       */
/****************************************************************************/
/* Updated: Suttipong Kanakakorn
            Thu  08-03-1989  10:47:20
            - Create .bak,
            - use dispprintf instead of dispstr for compress program size
            Fri  08-04-1989  22:10:45
            - reduce case in function blkcmd to reduce size of program
*/

#include "inc.h"
#include "..\common\cwgrphc.h"
#include "convert.h"

/****************************************************************************/
/*  Hide/display block. It cause the visual marking of a block to be alter- */
/*  nately switched off and on. Block manipulation commands (copy, move,    */
/*  delete, and write to a file) work only when the block is displayed.     */
/*  Block related cursor movements (jump to beginning/end of block) work    */
/*  whether the block is hidden or displayed).                              */
/****************************************************************************/
void toggleblk(void)
{
    dispblock = !dispblock;
    pagecomplete = NO;
}
/****************************************************************************/
/*  Mark the beginning of a block. The marker itself is not visible on the  */
/*  screen, and the block only becomes visibly marked when the End block    */
/*  marker is set.                                                          */
/****************************************************************************/
void markbegin(unsigned x)
{
    blkbegin.lineno = lineno;
    blkbegin.column  = firstcol + x;
    if (blkbegin.lineno > blkend.lineno) {
      blkend.lineno = blkbegin.lineno;
      blkend.column = blkbegin.column;
    } else {
      if (blkbegin.lineno == blkend.lineno) {
        if (blkbegin.column > blkend.column) {
           blkend.column = blkbegin.column;
        }
      }
    }
    dispblock = YES;
    pagecomplete = NO;
}
/****************************************************************************/
/*  Mark the end of a block. The marker itself is not visible on the screen */
/*  and the block only becomes visibly marked when the Begin block marker   */
/*  is set.                                                                 */
/****************************************************************************/
void markend(unsigned x)
{
    blkend.lineno = lineno;
    blkend.column  = firstcol + x;
    if (blkbegin.lineno > blkend.lineno) {
      blkbegin.lineno = blkend.lineno;
      blkbegin.column = blkend.column;
    } else {
      if (blkbegin.lineno == blkend.lineno) {
        if (blkbegin.column > blkend.column) {
          blkbegin.column = blkend.column;
        }
      }
    }
    dispblock = YES;
    pagecomplete = NO;
}
/****************************************************************************/
/*  Check block it is marked or not. Return YES if it is marked , else      */
/*  return NO.                                                              */
/****************************************************************************/
int haveblock(void)
{
    if ((blkbegin.lineno == blkend.lineno) &&
        (blkbegin.column == blkend.column)) {
      return(NO);
    } else {
      return(YES);
    }
}
/****************************************************************************/
/*  Check position of input to see it is in block or not. Return YES if it  */
/*  is in block , else return NO.                                           */
/****************************************************************************/
int inblock(unsigned linenum,unsigned colnum)     /* column origin 0 */
{
    if ((linenum >= blkbegin.lineno) &&
        (linenum <= blkend.lineno)) {
      if (linenum == blkbegin.lineno) {
        if (colnum < blkbegin.column) {
          return(NO);
        }
      }
      if (linenum == blkend.lineno) {
        if (colnum >= blkend.column) {
          return(NO);
        }
      }
      return(YES);
    } else {
      return(NO);
    }
}
/****************************************************************************/
/*  Copy linked list from assigned line & column (first & last) to space    */
/*  allocated from MS-DOS. Data structer of space is circular linked list,  */
/*  previous pointer of first line point to last line & next pointer of     */
/*  last line point to first line. Return value is pointer to first line    */
/*  of space. Text is copied only,no graphic image is.                      */
/****************************************************************************/
struct line_node *copytospace(fline,lline,fcol,lcol)
struct line_node *fline,*lline; /* first line & last line to be copied */
unsigned fcol,lcol; /* first column in 1'st line & last column in last line,
                       column is origin 0 */
{
    struct line_node *firstline,*templine,*newline;
    unsigned i,j;
    font_attr font;
    char fontcode[9],*temp;
    firstline = (struct line_node *) malloc(sizeof(struct line_node));
    firstline->next = firstline;               /* set up     */
    firstline->previous = firstline;           /* first node */
    firstline->wrap = fline->wrap;
    firstline->graph = NULL;                   /* graphic image is not copied */
    i = linearcolumn(fline->text,fcol,&font);
    findstrcode(fontcode,font);                /* find string of control code */
    firstline->text = (char *) malloc(strlen(fontcode) + strlen(fline->text) - i + 1);
    strcpy(firstline->text,fontcode);
    strcat(firstline->text,(fline->text) + i);
    if (fline == lline) {
      j = linearcolumn(lline->text,lcol,&font);
      temp = firstline->text;
      *(temp + strlen(fontcode) + (j - i)) = '\0';
      findstrcode(fontcode,font);
      firstline->text = (char *) malloc(strlen(temp) + strlen(fontcode) + 1);
      strcpy(firstline->text,temp);
      strcat(firstline->text,fontcode);
      free(temp);
    }
    templine = firstline;
    while (fline != lline) {
      fline = fline->next;
      newline = (struct line_node *) malloc(sizeof(struct line_node));
      newline->wrap = fline->wrap;
      newline->graph = NULL;
      if (fline != lline) {
        newline->text = (char *) malloc(strlen(fline->text) + 1);
        strcpy(newline->text,fline->text);
        insert_line(templine,newline);
        templine = templine->next;
      } else {
        i = linearcolumn(lline->text,lcol,&font);
        findstrcode(fontcode,font);
        newline->text = (char *) malloc(i + strlen(fontcode) + 1);
        strncpy(newline->text,lline->text,i);
        strcpy(newline->text + i,fontcode);
        insert_line(templine,newline);
      }
    }
    return(firstline);
}
/****************************************************************************/
/*  Insert source linked list pointed by sourceline,that previous pointer   */
/*  point to last line,to destination linked list pointed by destline and   */
/*  first column (in thai) is destcol.                                      */
/****************************************************************************/
void insertlinklist(sourceline,destline,destcol)
struct line_node *sourceline,*destline;
unsigned destcol; /* thai column */
{
    int i;
    struct line_node *lastline;
    font_attr font;
    char fontcode[9],*temp;
    lastline = sourceline->previous;
    if (sourceline->next != sourceline) {  /* > 1 line ? */
      insertreturn(destline,destcol);
      (destline->next)->previous = lastline;   /* link all before */
      lastline->next = destline->next;         /* delete first &  */
      destline->next = sourceline;             /* last line of    */
      sourceline->previous = destline;         /* source after    */
      destline->wrap = sourceline->wrap;
      temp = (char *) malloc(strlen(destline->text) + strlen(sourceline->text) + 1);
      strcpy(temp,destline->text);
      strcat(temp,sourceline->text);
      free(destline->text);
      destline->text = temp;
      temp = (char *) malloc(strlen(lastline->text) + strlen((lastline->next)->text) + 1);
      strcpy(temp,lastline->text);
      strcat(temp,(lastline->next)->text);
      free((lastline->next)->text);
      (lastline->next)->text = temp;
      deleteline(sourceline);
      deleteline(lastline);
    } else {  /* only one line */
      i = linearcolumn(destline->text,destcol,&font);
      findstrcode(fontcode,font);
      temp = (char *) malloc(strlen(sourceline->text) + strlen(destline->text)
                             + (2 * strlen(fontcode)) + 1);
      strncpy(temp,destline->text,i);
      temp[i] = '\0';
      strcat(temp,fontcode);
      strcat(temp,sourceline->text);
      strcat(temp,fontcode);
      strcat(temp,(destline->text) + i);
      free(destline->text);
      destline->text = temp;
    }
}
/****************************************************************************/
/*  Delete part of linked list from struct from assigned parameter.         */
/****************************************************************************/
void deletelinklist(fline,lline,fcol,lcol)
struct line_node *fline,*lline; /* first line & last line to be deleted */
unsigned fcol,lcol; /* first column in 1'st line & last column in last line,
                       origin 0 */
{
    struct line_node *templine,*freeline;
    insertreturn(lline,lcol);
    lline = lline->next;
    insertreturn(fline,fcol);
    templine = fline->next;
    fline->next = lline;
    lline->previous = fline;
    while (templine != lline) {
      if (templine == curline)
        curline = fline;
      if (templine == curpage)
        curpage = fline;
      freeline = templine;
      templine = templine->next;
      if (freeline->text != NULL)
        free(freeline->text);
      if (freeline->graph != NULL)
        free(freeline->graph);
      free(freeline);
    }
    deletereturn(fline);
}
/****************************************************************************/
/*  Insert block at current cursor position.                                */
/****************************************************************************/
void copyblk(unsigned *x)
{
    struct line_node *fline,*lline,*space;
    if (haveblock() && dispblock) {
      storeline(curline);
      fline = linepointer(blkbegin.lineno);
      lline = linepointer(blkend.lineno);
      space = copytospace(fline,lline,blkbegin.column,blkend.column);
      insertlinklist(space,curline,*x + firstcol);
      blkend.lineno = lineno + blkend.lineno - blkbegin.lineno;
      blkbegin.lineno = lineno;
      if (blkbegin.lineno == blkend.lineno) {
        blkend.column = blkend.column - blkbegin.column + *x + firstcol;
      }
      blkbegin.column = *x + firstcol;
      pagecomplete = NO;
      changeflag = YES;
      dispblock = YES;
      loadtoline(curline->text);
    }
}
/****************************************************************************/
/*  Delete block at current cursor position.                                */
/****************************************************************************/
void deleteblk(void)
{
    struct line_node *firstline,*lastline;
    if (haveblock() && dispblock) {
      storeline(curline);
      firstline = linepointer(blkbegin.lineno);
      lastline = linepointer(blkend.lineno);
      deletelinklist(firstline,lastline,blkbegin.column,blkend.column);
      lineno = findlineno(curline);
      loadtoline(curline->text);
      blkbegin.lineno = 1;
      blkbegin.column = 0;
      blkend.lineno = 1;
      blkend.column = 0;
      changeflag = YES;
      pagecomplete = NO;
    } /* if haveblock */
}
/****************************************************************************/
/*  Move block to insert to current cursor position.                        */
/****************************************************************************/
void moveblk(unsigned *x)
{
    struct line_node *space,*fline,*lline;
    if (haveblock() && dispblock) {
      storeline(curline);
      fline = linepointer(blkbegin.lineno);
      lline = linepointer(blkend.lineno);
      space = copytospace(fline,lline,blkbegin.column,blkend.column);
      deletelinklist(fline,lline,blkbegin.column,blkend.column);
      if (lineno == blkend.lineno) {
        if (blkend.column > blkbegin.column) {
          if (*x > blkend.column) {
            *x = *x - (blkend.column - blkbegin.column);
          } else {
            if (*x > blkbegin.column) {
              *x = blkbegin.column;
            }
          }
        }
        if (blkbegin.column > blkend.column) {
          *x = *x + blkbegin.column - blkend.column;
        }
      }
      lineno = findlineno(curline);
      blkbegin.lineno = lineno;
      blkbegin.column = *x + firstcol;
      blkend.lineno = lineno + spacesize(space);
      if (space->next != space) {
        blkend.column = thaistrlen((space->previous)->text);
      } else {
        blkend.column = blkbegin.column + thaistrlen(space->text);
      }
      insertlinklist(space,curline,*x + firstcol);
      loadtoline(curline->text);
      changeflag = YES;
      pagecomplete = NO;
      dispblock = YES;
    }
}
/****************************************************************************/
/*  Read file from disk. Parameter is filename wanted to read. Space is     */
/*  allocated from MS-DOS. Data structer of space is circular linked list,  */
/*  previous pointer of first line point to last line & next pointer of     */
/*  last line point to first line. Return value is pointer to space,        */
/*  if error return NULL.                                                   */
/****************************************************************************/
struct line_node *rdfiletospace(char file_name[])
{
    FILE *fip,*fopen();
    register int i;
    struct line_node *space,*newline,*currentline,*freeline;
    static char text_str[MAXCOL*5]; /******* Modified ***********/
    if ((fip = fopen(file_name,"rt")) != NULL) {
      blockmsg(11);
      dispstrhgc("กำลังอ่านข้อมูลจากแผ่นจานแม่เหล็กอยู่ กรุณารอสักครู่..."
                 ,25-CENTER_FACTOR ,11,2);
      space = (struct line_node *) malloc(sizeof(struct line_node));
      if (space == NULL) goto no_mem_avail;
      space->next = space;
      space->previous = space;
      space->wrap = NO;
      space->graph = NULL;
      space->text = (char *) malloc(1);
      if (space->text == NULL) goto no_mem_avail;
      *(space->text) = '\0';
      if (fgets(text_str,MAXCOL*5,fip) != NULL) {
        i = strlen(text_str);
        if ((i > 0) && (text_str[i-1] == '\n')) {
          text_str[i-1] = '\0';
          i--;
        }
        if ((text_str[0] == '.') &&
            (text_str[1] == 'G') &&
            (text_str[2] == 'R')) {
           space->graph = NULL;
/*           space->graph = readgraph(text_str+4);   */
           free(space->text);
           space->text = NULL;
        } else {
          if ((i > 0) && (text_str[i-1] == WRAPCODE)) {
            text_str[i-1] = '\0';
            space->wrap = YES;
          }
          free(space->text);
          space->text = (char *) malloc(strlen(text_str) + 1);
          if (space->text == NULL) goto no_mem_avail;
          if (!stdcode) {
            for (i=0;text_str[i] != '\0';i++)
              text_str[i] = kutostd(text_str[i]);
          }
          strcpy(space->text,text_str);
        }
        currentline = space;
        while (fgets(text_str,MAXCOL*5,fip) != NULL) {
          if (currentline->text == NULL) {
            newline = currentline;
          } else {
            newline = (struct line_node *) malloc(sizeof(struct line_node));
            if (newline == NULL) goto no_mem_avail;
            newline->text = NULL;
            newline->graph = NULL;
            newline->wrap = NO;
            insert_line(currentline,newline);
          }
          i = strlen(text_str);
          if ((i > 0) && (text_str[i-1] == '\n')) {
            text_str[i-1] = '\0';
            i--;
          }
          /*
          if ((text_str[0] == '.') &&
              (text_str[1] == 'G') &&
              (text_str[2] == 'R')) {
/*             newline->graph = readgraph(text_str+4);    */
          } else { */
          if ((i > 0) && (text_str[i-1] == WRAPCODE)) {
            text_str[i-1] = '\0';
            newline->wrap = YES;
          }
          newline->text = (char *) malloc(strlen(text_str) + 1);
          if (newline->text == NULL) goto no_mem_avail;
          if (!stdcode) {
            for (i=0;text_str[i] != '\0';i++)
              text_str[i] = kutostd(text_str[i]);
          }
          strcpy(newline->text,text_str);
          /*}*/
          currentline = newline;
        }
      }
      fclose(fip);
      return(space);
    } else {
      errorsound();
      blockmsg(10);
      dispstrhgc("ไม่พบแฟ้มข้อมูลนี้ ! กด <ESC> เพื่อทำงานต่อ"
                  ,27-CENTER_FACTOR,10,2);
      while (ebioskey(0) != ESCKEY);
      return(NULL);
    }
no_mem_avail:
    fclose(fip);
    errorsound();
    blockmsg(11);
    dispstrhgc("หน่วยความจำไม่พอ ! กด <ESC> เพื่อทำงานต่อ",26-CENTER_FACTOR,11,2);
    while (ebioskey(0) != ESCKEY);
    dispstrhgc("กำลังคืนหน่วยความจำให้ระบบ กรุณารอสักครู่ ...",26-CENTER_FACTOR,11,2);
    if (space != NULL) {
      currentline = space->next;
      while (currentline != space) {
        freeline = currentline;
        currentline = currentline->next;
        if (freeline->text != NULL)
          free(freeline->text);
        if (freeline->graph != NULL)
          free(freeline->graph);
        free(freeline);
      }
      if (space->text != NULL)
        free(space->text);
      if (space->graph != NULL)
        free(space->graph);
      free(space);
    }
    return(NULL);
}
/****************************************************************************/
/*  Find size of space that allocated form MS-DOS. Data structer of space   */
/*  is circular linked list, previous pointer of first line point to last   */
/*  line & next pointer of last line point to first line. Return value is   */
/*  number of line of space.                                                */
/****************************************************************************/
unsigned spacesize(struct line_node *space)
{
    struct line_node *temp;
    unsigned size = 0;
    temp = space;
    while (temp->next != space) {
      temp = temp->next;
      size++;
    }
    return(size);
}
/****************************************************************************/
/*  Read block from file to insert to current cursor position               */
/****************************************************************************/
readblk(unsigned *x)
{
    char file_name[22];
    struct line_node *space;
    int i;
    storeline(curline);
    pagecomplete = NO;
    /*
    dispstrhgc(" •••••••••••••••••••••••••••••••••••••••••••••••••• ",18-CENTER_FACTOR,4,2);
    dispstrhgc(" –  ใส่ชื่อแฟ้มข้อมูลที่ต้องการอ่าน :                         – ",18-CENTER_FACTOR,5,2);
    dispstrhgc(" •••••••••••••••••••••••••••••••••••••••••••••••••• ",18-CENTER_FACTOR,6,2);
    */
    framebox(18-CENTER_FACTOR, 4, 18-CENTER_FACTOR+53, 6, 2);
    dispstrhgc("ใส่ชื่อแฟ้มข้อมูลที่ต้องการอ่าน :", 18-CENTER_FACTOR+4, 5, 2);
    strcpy(file_name,"*.*");
    i = getname(file_name,46-CENTER_FACTOR,5,22,2);
    if ((i == YES) && (file_name[0] != '\0')){
      if (havewild(file_name)) {
        selectfile(file_name);
      }
      space = rdfiletospace(file_name);
      if (space != NULL) {
        blkbegin.lineno = lineno;
        blkbegin.column = *x + firstcol;
        blkend.lineno = lineno + spacesize(space);
        if (space->next != space) {
          blkend.column = thaistrlen((space->previous)->text);
        } else {
          blkend.column = blkbegin.column + thaistrlen(space->text);
        }
        insertlinklist(space,curline,*x + firstcol);
        changeflag = YES;
        dispblock = YES;
        loadtoline(curline->text);
      }
    }
    return;
}

unsigned long getfilesize(void)
{
    struct line_node *keepline, *templine = sentinel->next;

    unsigned long fsize = 0;
    unsigned int lineno = 0;

    keepline = templine;
    if (templine == NULL)
    {
      dispstrhgc ("getfilesize err",1,1,2);
      ebioskey(0);
    }
    ++lineno;
    while (templine->next != sentinel) {
      fsize = fsize + strlen(templine->text) + 2;
      if (templine->wrap == YES)
        fsize++;
      templine = templine->next;
      ++lineno;

      if (templine == NULL)
      {
        dispstrhgc ("getfilesize err",1,1,2);
        dispstrhgc (keepline->text,1,2,2);
        dispprintf (1,3,2,"Line no = %u",lineno);
        ebioskey(0);
        settext ();
        exit (1);
        break;
      }
      keepline = templine;
    }
    fsize += strlen(templine->text);
    return(fsize);
}

struct dfree *dfreep;
unsigned long diskfree(int driveno)
{
    unsigned long avail,bsec,sclus;
    getdfree(driveno,dfreep);
    avail = dfreep->df_avail;
    bsec = dfreep->df_bsec;
    sclus = dfreep->df_sclus;
    return(avail * bsec * sclus);
}

int chkspace(fname)
char fname[];
{
  unsigned long diskspace,filesize;
  int i,flag;
  struct ffblk fdat;
  char drv[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];

  flag = fnsplit(fname,drv,dir,name,ext);              /* Analyse Fname */
  filesize = getfilesize();
  framebox(17-CENTER_FACTOR, 8, 17-CENTER_FACTOR+55, 12,REVERSEATTR);
  dispstrhgc(" –             ขนาดแฟ้มข้อมูล =                   ไบต์    – ",17-CENTER_FACTOR,9,2);
  dispstrhgc(" –   เนื้อที่ว่างบนแผ่นจานแม่เหล็ก =                   ไบต์    – ",17-CENTER_FACTOR,10,2);
  dispprintf(48 - CENTER_FACTOR, 9, 2,"%#10lu", filesize);

  if (flag & DRIVE)                                    /* Has it Drive */
    i=drv[0]-'A'+1;
  else
    i=0;
  diskspace = diskfree(i);

  if(!create_bak && file_exist(fname)) {       /* If .BAK is not necessary */
    findfirst(fname,&fdat,0);                  /* DiskSpace can be increased */
    diskspace += (unsigned long)fdat.ff_fsize; /* By old file size */
  }

  dispprintf(48 - CENTER_FACTOR, 10, 2, "%10lu", diskspace);
  if (diskspace < filesize) {
    dispstrhgc("เนื้อที่บนแผ่นจานแม่เหล็กไม่พอ ! กดปุ่ม ESC เพื่อทำงานต่อ...",22-CENTER_FACTOR,11,2);
    errorsound();
    while (ebioskey(0) != ESCKEY);
    return(0);
  } else
    return(1);
}


void writeblk(file_name,linebegin,colbegin,lineend,colend)
char file_name[];
struct line_node *linebegin,*lineend;
unsigned colbegin,colend;          /* physical column */
{
    FILE *fip;
    struct line_node *currentline;
    int key,i,j,firstround,count;   /*  fileerror = NO;  */
    char *templine, fontcode[9];
    font_attr font;
    char drv[MAXDRIVE],dir[MAXDIR],name[MAXFILE],ext[MAXEXT];
    char name_bak[MAXPATH];

    static char bakext[] = ".BAK";
    static char text_str[MAXCOL*5];      /********* Modified ************/

    pagecomplete = NO;
    storeline(curline);

    framebox(17-CENTER_FACTOR, 4, 17-CENTER_FACTOR+55, 6, 2);
    dispstrhgc("ใส่ชื่อแฟ้มข้อมูลที่ต้องการจัดเก็บ :", 17-CENTER_FACTOR+4, 5, 2);
    i = getname(file_name,47-CENTER_FACTOR,5,22,2);
    if ((i != YES) || (file_name[0] == '\0'))
        return;

    framebox(17-CENTER_FACTOR, 8, 17-CENTER_FACTOR+55, 12, 2);
    fnsplit(file_name,drv,dir,name,ext);

    if (file_exist(file_name)) {
        dispstrhgc(" –    แฟ้มข้อมูลเดิมมีอยู่แล้ว ต้องการเขียนทับหรือไม่ ? (Y/N)      – ",17-CENTER_FACTOR,10,2);
        do {
            key = toupper(ebioskey(0) & 0xff);
        } while ((key != 'N') && (key != 'Y'));

        if (key == 'Y') {                    /* Save edited file */
          if (create_bak) {                  /* Create .BAK */
            fnmerge(name_bak, drv, dir, name, bakext);
            unlink(name_bak);                /* Delete Old .BAK */
            if (chkspace(file_name))         /* Enough Space ? */
              rename(file_name, name_bak);
            else
              return;                        /* File too large */
          } else {                           /* Ignore .BAK */
            if(chkspace(file_name))          /* Enough Space ? */
              unlink(file_name);
            else
              return;
          }
        } else {                             /* Don't save edited file */
            return;
        }
    } else {                                 /* Save new File */
       if(!chkspace(file_name))              /* Not enough space */
         return;
    }
    if ((fip = fopen(file_name,"wt")) != NULL) {
      dispstrhgc("กำลังจัดเก็บแฟ้มข้อมูลอยู่ กรุณารอสักครู่...",22-CENTER_FACTOR,11,2);
      currentline = linebegin;
    /*      if (currentline->graph != NULL)
                fprintf(fip,".GR %s\n",currentline->graph);     */
      firstround = linearcolumn(currentline->text,colbegin,&font);
      findstrcode(fontcode,font);
      fputs(fontcode,fip);
      while (currentline != lineend) {
        if (!stdcode) {
          templine = currentline->text;
          i = 0;
          while (*(templine + firstround + i) != '\0') {
            text_str[i] = *(templine + firstround + i);
            i++;
          }
          text_str[i] = '\0';
          i = 0;
          while (text_str[i] != '\0') {
            text_str[i] = stdtoku(text_str[i]);
            i++;
          }
          fputs(text_str,fip);
        } else {
          templine = currentline->text;
          fputs(templine + firstround,fip);
        }
        if (currentline->wrap) {
          putc(WRAPCODE,fip);
        }
        putc('\n',fip);
        firstround = 0;
        currentline = currentline->next;
    /*          if (currentline->graph != NULL)
                    fprintf(fip,".GR %s\n",currentline->graph);    */
      }
      templine = currentline->text;
      count = linearcolumn(templine,colend,&font);
      i = firstround;
      j = 0;
      while ((i != count) &&
        (*(templine + i) != '\0')) {
        text_str[j++] = *(templine + i);
        i++;
      }
      text_str[j] = '\0';
      if (!stdcode) {
        i = 0;
        while (text_str[i] != '\0') {
          text_str[i] = stdtoku(text_str[i]);
          i++;
        }
      }
      fputs(text_str,fip);
      if (currentline->wrap) {
        putc(WRAPCODE,fip);
      }
      putc(0x1a,fip);
      fclose(fip);
    }  /*   else {
        fileerror = YES; }  */
/*
    if (fileerror == YES) {
        errorsound();
        blockmsg(10);
        dispstrhgc("จัดเก็บแฟ้มข้อมูลผิดพลาด ! กด <ESC> เพื่อทำงานต่อ",24-CENTER_FACTOR,10,2);
        while (ebioskey(0) != ESCKEY);
    }
*/
}

void writeblock(void)
{
    int linenum;
    char file_name[22];
    struct line_node *firstline,*lastline;
    if (haveblock()){
      file_name[0] = '\0';
      linenum = blkbegin.lineno;           /* get displacement */
      firstline = sentinel->next;          /* then set pointer */
      while (linenum-- > 1)                /* to first line of */
        firstline = firstline->next;   /* block position   */
      linenum = blkend.lineno;             /* get displacement */
      lastline = sentinel->next;           /* of end block to  */
      while (linenum-- > 1)                /* set pointer to   */
        lastline = lastline->next;     /* last line of block */
      writeblk(file_name,firstline,blkbegin.column,lastline,blkend.column);
  }
}

void blkcmd(register unsigned key,unsigned *x)
{
    key &= 0xff;
    if (!isalpha(key) && !iscntrl(key))     
        return;
    switch(key & 0x1f) {
    case 'b' - 'a' + 1 :
        markbegin(*x);
        break;
    case 'k' - 'a' + 1:
        markend(*x);
        break;
    case 'c' - 'a' + 1 :
        copyblk(x);
        break;
    case 'y' - 'a' + 1 :
        deleteblk();
        break;
    case 'v' - 'a' + 1 :
        moveblk(x);
        break;
    case 'h' - 'a' + 1 :
        toggleblk();
        break;
    case 'p' - 'a' + 1 :
        print_file (); /****** in menu.c *****/
        break;
    case 'r' - 'a' + 1 :
        readblk(x);
        break;
    case 'w' - 'a' + 1 :
        writeblock();
        break;
    case 's' - 'a' + 1 :
        writeblk(filename,sentinel->next,0,sentinel->previous,MAXCOL);
        changeflag = NO;
        break;
    }
}

void blockcommand(unsigned *x)
{
    unsigned key;
    dispkey (CNTRL_K);
    waitkbd(3,2);
    key = ebioskey(0);
    blkcmd(key,x);
}