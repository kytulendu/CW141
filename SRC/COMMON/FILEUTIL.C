/* Written: Suttipong Kanakakorn */
/* Updated: Sun  08-27-1989  22:40:12 */

#include <io.h>
#include <fcntl.h>
#include <dir.h>
#include <stdio.h>

#include "cwtype.h"

boolean file_exist(char *filename)
{
    int fd;

    fd = open(filename, O_RDONLY || O_BINARY);
    if (fd == -1)
        return NO;
    else {
        close(fd);
        return YES;
    }
}
/*
search_file_mode find_file(char *filename, char *search_path)
{
    struct ffblk ffblk;
    char p[MAXPATH];

    if (findfirst(filename, &ffblk, 0) == 0)
        return CUR_DIR;
    sprintf(p, "%s\\%s", search_path, filename);
    if (findfirst(p, &ffblk, 0) == 0)
        return CW_DIR;
    return NOT_FOUND;
}
*/
int havewild(char *filname)
{
    while (*filname) {
        if ((*filname == '*') || (*filname == '?'))
            return(YES);
        filname++;
    }
    return(NO);
}

/* read screen font from current dir first,
   if not found try reading from search_path
*/
/* Tue  08-01-1989  13:52:17 */
void readscrfont(void *font_buffer, char *fontname, char *search_path)
{
    register int handle;
    /* long filelen; */
    char fn[MAXPATH];

    if ((handle = open(fontname,O_RDONLY || O_BINARY)) == -1) {
        sprintf(fn, "%s\\%s", search_path, fontname);
        handle = open(fn, O_RDONLY || O_BINARY);
    }
    if (handle != -1) {
        /* filelen = filelength(handle); */
        read(handle, font_buffer, 20*256);
        close(handle);
    } else {
        fprintf(stderr,"FONT FILE (%s) NOT FOUND !\n", fontname);
        exit(1);
    }
}
