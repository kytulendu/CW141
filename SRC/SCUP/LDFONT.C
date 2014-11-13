#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <alloc.h>
#include <string.h>
#include "global.ext"

char dfont [5121];
char ditalicfont [5121];
int protect1;

void loadfont(void)
{
  int handle;
  char s1[55];
  strcpy(s1,cuprintpath);
  strcat(s1,"normal.fon");
  if ((handle = open(s1,O_RDONLY || O_BINARY)) != -1) {
	read(handle,dfont,5120);
    close(handle);
  } else {
    puts("FONT FILE (NORMAL.FON) NOT FOUND !");
    exit(1);
  }
  if ((handle = open("italic.fon",O_RDONLY || O_BINARY)) != -1) {
    read(handle,ditalicfont,5120);
    close(handle);
  } else {
    puts("FONT FILE (italic.FON) NOT FOUND !");
    exit(1);
  }
}
