/*
proto.h for cufont.exe
By Suttipong Kanakakorn Fri  08-25-1989  22:34:30
*/

/***GETSTR.C*****************************************************************/
int getstring(char textst[],unsigned x,unsigned y,
                unsigned maxlen,char attr,strtype mode);
int getname(char textst[],unsigned x,unsigned y,unsigned maxlen,char attr);

/* TUTIL1.c */
unsigned thaistrlen(register unsigned char *thaist);

/* cufsetup.c */
void cufsetup(int argc, char *argv[]);
void readfont(void *font_buffer, char *fontname);

/***KBD.C********************************************************************/
void waitkbd(unsigned x,unsigned y);
int readkbd(void);

/***FILE.C*******************************************************************/
search_file_mode find_file(char *filename, char *search_path);
