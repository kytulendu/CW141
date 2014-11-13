/*  add function file_exist by Suttipong Kanakakorn
    Thu  08-03-1989  00:16:25
    use framebox instead of dispstrhgc
    Sat  08-05-1989  02:59:38
*/

#include <io.h>

#include "inc.h"
#include "..\common\cwgrphc.h"
#include "convert.h"

int handler(int errval,int ax,int bp,int si)
{
    /* char drive; */
    char *scrn;

    /* errorsound(); */
    if (ax >= 0) {
      /* drive = 'A' + (ax & 0x00FF); */
      scrn = savescrn (18-CENTER_FACTOR, 9, 71-CENTER_FACTOR, 11);
      blockmsg(10);
      dispprintf(25-CENTER_FACTOR,10,2,
                 "Disk error on drive %c ! กดปุ่มใดๆเพื่อทำงานต่อ", 
                  'A' + (ax & 0x00FF));
      /* prchar(drive,REVERSEATTR,45-CENTER_FACTOR,10); */
      ebioskey(0);
      resscrn(scrn,18-CENTER_FACTOR, 9, 71-CENTER_FACTOR, 11);
    }
    hardretn(-1);
}

int read_file(char *file_name)
{
    struct line_node *space;
    space = rdfiletospace(file_name);
    if (space != NULL) {
      insertlinklist(space,curline,0);
      return(!ERROR);
    } else {
      return(ERROR);
    }
}

void abandonfile(void)
{
    struct line_node *currentline,*templine;
    blockmsg(10);
    dispstrhgc("กำลังทำการยกเลิกแฟ้มข้อมูลเดิมอยู่ กรุณารอสักครู่..."
               ,26-CENTER_FACTOR,10,2);
    currentline = sentinel->next;
    while (currentline != sentinel) {
      templine = currentline;
      currentline = currentline->next;
      free(templine->text);
      if (templine->graph != NULL)
        free(templine->graph);
      free(templine);
    }
    free(sentinel);
}