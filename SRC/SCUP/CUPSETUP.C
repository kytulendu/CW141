/*
   Written: Suttipong Kanakakorn
            Fri  08-11-1989  01:56:46
*/

#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <stdio.h>
#include <dir.h>
#include <process.h>
#include "inc.h"
#include "global.ext"
#include "..\common\grdetect.h"
#include "..\common\cwgrphc.h"
#include "..\common\kbdcode.h"

#define ALL_FONT_SIZE 5120

char dfont[ALL_FONT_SIZE];
char ditalicfont[ALL_FONT_SIZE];
int  protect1;

extern int herc_align; /* from hdisp.asm */

void cupsetup(int argc, char *argv[])
{
    register int i;

    progname = argv[0];

    /* Sorry for the pointer, but easy to expand argument
                        Suttipong
    */
    while ((--argc > 0) && ((i = (*++argv)[0]) == '/' || i == '-')) {
        strupr (++argv[0]);
        while (i = *(argv[0]++))
            switch(i) {
            case 'H' :  scrmode = HERCMONO;
                        break;
            /* e alone = ega, em = ega monochrome */
            case 'E' :  scrmode = EGA;
                        break;
            /* m alone = mcga */
            case 'M' :  if (scrmode == EGA)
                            scrmode = EGAMONO;
                        else
                            scrmode = MCGA;
                        break;
            case 'V' :  scrmode = VGA;
                        break;
            case 'A' :  scrmode = ATT400;
                        break;
            /* /HL for Hercules, left-justified  */
            case 'L' :  herc_align = 0;
                        break;
            case 'N' :
            case 'W' :
            case 'P' :
                        break;
            default  :  usage();
            }
    }
    if (argc >= 1 && (file_exist(argv[0]) ||
        strchr(argv[0], '*') || strchr(argv[0], '?'))) {
        placekey(RETKEY);
        while (i = *argv[0]++)
            placekey(i);
        placekey(RETKEY);
    }

    set_directory();
    readoption(AUTO_FIND);
    readscrfont(dfont, "NORMAL.FON", cup_dir);
    readscrfont(ditalicfont, "ITALIC.FON", cup_dir);

    harderr(handler);
    filename[0] = '\0';
}

void usage(void)
{
    fputs("Usage: cuprint [option] [file]\n", stderr);
    fputs("Options\n", stderr);
    fputs("\t-h,\t/h  Hercules graphic adapter\n", stderr);
    fputs("\t-hl,\t/hl Hercules graphic adapter, left justify\n", stderr);
    fputs("\t-v,\t/v  vga\n", stderr);
    fputs("\t-e,\t/e  ega with ega display\n", stderr);
    fputs("\t-em,\t/em ega with monochrome display\n", stderr);
    fputs("\t-m,\t/m  mcga\n", stderr);
    fputs("\t-a,\t/a  at&t400\n", stderr);
    exit(1);
}
    
void set_directory(void)
{
    char drive[MAXDRIVE], dir[MAXDIR], name[MAXFILE], ext[MAXEXT];

    /* build cup_dir like this -> c:\edit\cuwriter */
    fnsplit(progname, drive, dir, name, ext);
    sprintf(cup_dir, "%s%s", drive, dir);
    cup_dir[strlen(cup_dir) - 1] = '\0'; /* clear \ */
}

typedef struct  {
    char *option_name;
    void *p_option_value;
} each_option_setup;

each_option_setup option_setup[] = {
    "graphicprint", &graphicprint,
    "prtcodestd",  &prtcodestd,
    "character/inch", &cpi,
    "userlineperpage", &userlineperpage,
    "pagelength", &pagelength,
    "leftmargin", &leftmargin,
    "rightmargin", &rightmargin,
    "locpagetitle", &locpagetitle,
    "locheading", &locheading,
    "locfooting", &locfooting,
    "printer", &printer,
    "printer24pin",  &printer24pin,
    "maxcol", &maxcol,
    "maxdot", &maxdot,
    "maxbuffer", &maxbuffer,
    "nlqmode", &nlqmode,
    "pagebreak", &pagebreak,
    "pagebegin", &pagebegin,
    "pageend", &pageend,
    "pagenumberoffset", &pagenumberoffset,
    "copytoprint", &copytoprint,
    "stdcode", &stdcode,
    "smallpaper", &smallpaper,
    NULL, NULL,
    "pagetitle", pagetitle,
    "pageformat", pageformat,
    "heading", heading,
    "footing", footing,
    NULL, NULL
};

/* number of element of option_setup */
#define NELEM_OPTION (sizeof(option_setup)/sizeof(option_setup[0]))

/*  read option from file cuprint.cfg
    mode == AUTO try reading from current directoy first, then cw_dir
                if not found use the default value
    mode == CUR_DIR read from current dir
                if not found display error
    mode == CW_DIR  read from cw.exe directory
                if not found display error
    Tue  08-01-1989  12:10:39
*/
void readoption(search_file_mode mode)
{
    FILE *fp;
    char fname[MAXPATH];
    int  field, temp;
    char opname[25];
    char tstr[60];
    each_option_setup *op;

    set_all_lineperpage(BATCH);

    sprintf(fname, "%s\\CUPRINT.CFG", cup_dir);
    if (mode == CUR_DIR || mode == AUTO_FIND) {
        fp = fopen("CUPRINT.CFG","rt");
    } else {
        fp = fopen(fname, "rt");
    }
    if (fp == NULL && mode == AUTO_FIND) {
        /* fopen file not success */
        /* continue searching in cup_dir */
        if ((fp = fopen(fname, "rt")) == NULL)
            return; /* if not found in both dir use default value */
    }
    if (fp == NULL) {
        showerrno();
        return ;
    }
    /* If we reach here we succesfully open cw.cfg file */
    for (op = option_setup; op->option_name != NULL; op++) {
        field = fscanf(fp, "%s %d", opname, &temp);
        if (field == 2) /* succesfully scan */
            * (int *) op->p_option_value = temp;
        else
            break;
    }
    for (op++; op->option_name != NULL; op++) {
        fscanf(fp, "%s", opname); /* get option name */
        fgets(tstr, 60, fp);      /* get the rest */
        if (tstr[0]) {
            tstr[strlen(tstr) - 1] = '\0'; /* delete \n */
            strcpy(op->p_option_value, tstr+1); /* skip first blank */
        }
    }
    fclose(fp);

    set_all_lineperpage(BATCH);
}

/* save option to file cw.cfg in either current directory or cw_dir */
void saveoption(search_file_mode mode)
{
    FILE *fp;
    char config_file[MAXPATH];
    each_option_setup *op;

    if (mode == CUR_DIR || mode == AUTO_FIND) {
        fp = fopen("CUPRINT.CFG","wt");
    } else {
        sprintf(config_file, "%s\\CUPRINT.CFG", cup_dir);
        fp = fopen(config_file, "wt");
    }
    if (fp == NULL)
        return ;
    for (op = option_setup; op->option_name != NULL; op++) {
        fprintf(fp, "%s %d\n", op->option_name, * (int *) op->p_option_value);
    }
    for (op++; op->option_name != NULL; op++)
        fprintf(fp, "%s %s\n", op->option_name, (char *) op->p_option_value);
    fprintf(fp,"\n");
    fclose(fp);
}

int handler(int errval,int ax,int bp,int si)
{
    if (ax >= 0) {
      disperror(" error on disk drive   ! กดปุ่มใดๆเพื่อทำงานต่อ");
      ebioskey(0);
    }
    hardretn(-1);
}

/* -------------------------------------------------------------------- */
/*      Function        :       cp_loadfont                             */
/*      Description     :       load printer font                       */
/*      Parameters      :       fname   - font filename                 */
/*      Return value    :               - pointer to font table         */
/*      Remark          :       This function allocate the font table   */
/*      Written         :       Suttipong Kanakakorn                    */
/*                              Tue  08-15-1989  00:23:58               */
/* -------------------------------------------------------------------- */
void *cp_loadfont (char *fname, unsigned font_size)
{
    void *p;
    int handle;
    char fn[80];

    p = (void *) malloc (font_size);
    if ( p == NULL)
        execerror ("Insufficient memory\n","");
    if ((handle = open(fname,O_RDONLY | O_BINARY)) == -1) {
        sprintf(fn, "%s\\%s", cup_dir, fname);
        handle = open(fn, O_RDONLY | O_BINARY);
    }
    if (handle != -1) {
        read(handle, p, font_size);
        close(handle);
    } else {
        execerror ("Printer font file not found : ", fname);
    }
    return (p);
}
