#ifndef WANT_TO_USE_GRAPH

#include <io.h>
#include "convert.h"
#include "inc.h"

char *readgraph(char *filename)
{
  long filelen,namelen;
  int handle;
  char *buffer;
  if ((handle = open(filename,O_RDONLY || O_BINARY)) == -1) return(NULL);
  filelen = filelength(handle);
  namelen = strlen(filename);
  buffer = (char *) malloc(filelen + namelen + 2);
  strcpy(buffer,filename);
  read(handle,buffer + namelen + 1,filelen);
  close(handle);
  buffer[namelen + filelen] = '\0';
  buffer[namelen + filelen + 1] = 0x80;       /* code of end of graph */
  return(buffer);
}

paintlinegraph(char *graph,unsigned y)
{
  unsigned endy,x;
  char tp;

  x = wind.col * 8;
  y = (y * 20) + 84;
  if (y >= 344)
    return;
  endy = y + 20;
  if (endy > 344)
    endy = 344;
  while ((y != endy) && (*graph != 0x80)) {
    tp = *graph;
    if (tp == 0) {
      graph++;
      y++;
      x = wind.col * 8;
    } else {
      if (tp < 128) {
        h_line_g(x,x+tp,y);
      } else {
        tp -= 128;
      }
      x += tp;
      graph++;
    }
  }
}

void notavailable(void)
{
    savepic();
    blockmsg(10);
    dispstrhgc("¤ÓÊÑè§¹ÕéÍÂÙèÃÐËÇèÒ§¡ÒÃ¾Ñ²¹Ò ! ¡´»ØèÁã´æà¾×èÍ·Ó§Ò¹µèÍ"
               ,25-CENTER_FACTOR,10,2);
    ebioskey(0);
    retpic();
}

insertgraph()
{
    notavailable();
    int i;
    char graphname[30];
    storeline(curline);
    dispstrhgc(" ˜••••••••••••••••••••••••••••••••••••••••••••••••••™ ",18-CENTER_FACTOR,4,2);
    dispstrhgc(" – ãÊèª×èÍá¿éÁÃÙ»ÀÒ¾·ÕèµéÍ§¡ÒÃÍèÒ¹ :                         – ",18-CENTER_FACTOR,5,2);
    dispstrhgc(" š••••••••••••••••••••••••••••••••••••••••••••••••••› ",18-CENTER_FACTOR,6,2);
    graphname[0] = '*';
    graphname[1] = '.';
    graphname[2] = '*';
    graphname[3] = '\0';
    i = getname(graphname,46,5,22,2);
    if ((i == YES) && (graphname[0] != '\0')){
      if (havewild(graphname)) {
        selectfile(graphname);
      }
      curline->graph = readgraph(graphname);
    }
    changeflag = YES;
    pagecomplete = NO;
    loadtoline(curline->text);
}

deletegraph()
{
    notavailable();
    if (curline->graph != NULL) {
      free(curline->graph);
      curline->graph = NULL;
      changeflag = YES;
      pagecomplete = NO;
    } else {
      errorsound();
    }
}
#endif
