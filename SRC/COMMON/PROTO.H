/* cproto.h: common prototype.h, function prototype of file
             from common directory
           from assembly module
                hdisp.asm, edisp.asm
           from C module
                ekbd.c, cscrn.c cwgraphc.c
*/
/* Updated: Suttipong Kanakakorn
Sun  07-30-1989  17:56:53
*/

/* From cwgraphc.c edisp.asm hdisp.asm */
void prchar(unsigned c,unsigned attr,unsigned x,unsigned y);
void prblank(unsigned x,unsigned y);
void setcurpos(unsigned x,unsigned y,int thaimode);
void plot(unsigned x,unsigned y);
void setgraph(void);
void settext(void);
char stdtoku (char c);
char kutostd (char c);
int  thaikey(unsigned c);
int  asm_keypressed(void);
void savepic(void);
void retpic(void);
void clsall(void);
void clsgraph(unsigned x1,unsigned y1,unsigned x2,unsigned y2);
void clrline(unsigned x1,unsigned y1,unsigned x2);
void prakeaw(void);
int whatlevel(char c);
void rdchardot (unsigned int x, unsigned int y);
void wrchardot (unsigned int x, unsigned int y);

/***EKBD.C****************************************************************/
int ebioskey(int cmd);
void placekey(int k);
boolean keypressed(void);

/***CSCRN.C***************************************************************/
int sign(int x);
void _line(int x1,int y1,int x2,int y2);
void box(int x1,int y1,int x2,int y2);
void dispstrhgc(char *st, unsigned x, unsigned y, font_attr attr);
void dispprintf(unsigned x, unsigned y, font_attr attr, char *format,...);
void dispblank(unsigned x, unsigned y, unsigned count, font_attr attr);
void togglefont(font_attr *curfont, font_code code);
void framebox(unsigned x1, unsigned y1,
              unsigned x2, unsigned y2,
              unsigned attr);
void blockmsg(int y);
void showerrno(void);
void resscrn(char *scrnindex,int x1,int y1,int x2,int y2);
char *savescrn(int x1,int y1,int x2,int y2);

/**** FILEUTIL.C ****/
boolean file_exist(char *filename);
int havewild(char *filname);
void readscrfont(void *font_buffer, char *fontname, char *search_path);
search_file_mode find_file(char *filename, char *search_path);

/***DIR.C********************************************************************/
int selectfile(char mask[]);

/***SOUND.C******************************************************************/
void soundnoise(unsigned freq,unsigned time);
void errorsound(void);

/* cusong.c */
void cusong(void);
