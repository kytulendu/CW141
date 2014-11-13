/* Update: Suttipong Kanakakorn
Sun  07-30-1989  17:56:53
Sun  08-06-1989  11:20:06
*/

/***SCRN.C*******************************************************************/
void cls(void);

/***EDIT.C*******************************************************************/
void findstrcode(char fontcode[], font_attr attr);
unsigned findlineno(struct line_node *line);
struct line_node *linepointer(unsigned linenum);
void displine(struct line_node *line,unsigned y,unsigned linenum);
void disppagebreak(unsigned y);
void showpage(void);
void showpageall(void);
unsigned findrow(void);
void adjustcol(unsigned *x);
void loadtoline(char *text);
void storeline(struct line_node *line);
void refreshline(unsigned x,unsigned y);

/***INS.C********************************************************************/
void insertblank(unsigned j,char code);
void insert_line(struct line_node *currentline,struct line_node *newline);
void shiftscrn(unsigned count,unsigned *x);
void linetoolong(void);
void inscntrl(char cntrl,unsigned x,unsigned y);
void printcntrl(unsigned x,unsigned y);
int insert_char(char c,unsigned *x,unsigned *y);
int ovrwrite_char(char c,unsigned *x,unsigned *y);
void insertreturn(struct line_node *line,unsigned thaicol);
void insert_ret(unsigned *x);
void returnkey(unsigned *x,unsigned y);
void ret_with_ins(unsigned *x,unsigned y);
void insertmacro(char *macro,unsigned *x,unsigned *y);
void blankmaro(int y);
void dispmacro(int i);
void editmacro(void);

/***DEL.C********************************************************************/
void delete_char(unsigned x);
void deletereturn(struct line_node *line);
void del_return(void);
void backspace(unsigned *x);
void delete_word(unsigned x);
void deltoendline(unsigned x,unsigned y);
void deleteline(struct line_node *line);
void delete_line(void);

/***STATUS.C*****************************************************************/
void relmarstatus(void);
void arabictothai(char *st);
void writepage(void);
void writeline(void);
void writelineno(void);
void writepageline(void);
void writecolno(unsigned x);
void writeinsmode(void);
void writelanguage(void);
void writeattr(void);
void writefilename(void);
void writedriveno(void);
void writestatus(unsigned x);

/***KBD.C********************************************************************/
void waitkbd(unsigned x,unsigned y);
int changekey(int key);
int readkbd(void);
int alt_char_map (int c);

/***MOVE.C*******************************************************************/
void goline(unsigned linetogo);
void gocol(unsigned coltogo,unsigned *x);
void home(unsigned *x);
void endline(unsigned *x);
void topfile(unsigned *x);
void endfile(unsigned *x);
void backword(unsigned *x);
void nextword(unsigned *x,unsigned y);
void gotopage(void);
void gotoline(void);
void gobeginblk(unsigned *x);
void goendblk(unsigned *x);
void quick(unsigned *x,unsigned *y);

/***MOVEMENT.C***************************************************************/
void cursor_up(void);
void cursor_down(unsigned y);
void cursor_left(unsigned *x);
void cursor_right(unsigned *x,unsigned y);
void page_up(void);
void page_down(void);
void scroll_up(void);
void scroll_down(void);
void top_of_page(void);
void bottom_of_page(void);

/***WRAP.C*******************************************************************/
void justify_right(void);
unsigned genrule1(unsigned nominal);
unsigned genrule3(unsigned nominal);
unsigned genrule4(unsigned nominal);
unsigned genrule5(unsigned nominal);
unsigned genrule7(unsigned nominal);
unsigned breakpoint(void);
void manualwrap(unsigned *x,unsigned *y);
void autowrap(unsigned *x,unsigned *y);
void reform(void);
void compress_blank(unsigned char *);

/***ONSCRN.C*****************************************************************/
void writetab(void);
void centerline(unsigned y);
void doonscrn(unsigned key,unsigned x,unsigned y);
void movetotab(unsigned *x,unsigned y);
void onscreen(unsigned x,unsigned y);
void readconfig(void);

/***FILE.C*******************************************************************/
int handler(int errval,int ax,int bp,int si);
int havewild(char filname[]);
int read_file(char *file_name);
void abandonfile(void);
boolean file_exist(char *filename);

/***SEARCH.C*****************************************************************/
int searchonlyinfo(void);
int searchreplaceinfo(void);
int optionglobal(void);
int optionupper(void);
int optionword(void);
int optionnoask(void);
char *searchline(char *textline,int startpos);
void wordnotfound(void);
int searchfwd(unsigned *x,unsigned *y);
void addblank(void);
void searching(unsigned *x,unsigned *y);
void replaceword(unsigned *x,unsigned *y);
int searchreplace(unsigned *x,unsigned *y);
void replacing(unsigned *x,unsigned *y);

/***BLOCK.C******************************************************************/
void toggleblk(void);
void markbegin(unsigned x);
void markend(unsigned x);
int haveblock(void);
int inblock(unsigned linenum,unsigned colnum);
struct line_node *copytospace(struct line_node *fline,struct line_node *lline,unsigned fcol,unsigned lcol);
void insertlinklist(struct line_node *sourceline,struct line_node *destline,unsigned destcol);
void deletelinklist(struct line_node *fline,struct line_node *lline,unsigned fcol,unsigned lcol);
unsigned spacesize(struct line_node *space);
struct line_node *rdfiletospace(char file_name[]);
void copyblk(unsigned *x);
void deleteblk(void);
void moveblk(unsigned *x);
readblk(unsigned *x);
unsigned long getfilesize(void);
unsigned long diskfree(int driveno);
void writeblk(char file_name[],struct line_node *linebegin,unsigned colbegin,struct line_node *lineend,unsigned colend);
void writeblock(void);
void blkcmd(unsigned key,unsigned *x);
void blockcommand(unsigned *x);
int chkspace(char *);

/***CW.C*********************************************************************/
void dispkey(unsigned key);
void setupnode(void);
char *chulaname(void);
void initscrn(void);
unsigned menu_to_key(unsigned curmenu);
void norighttoused(void);
void main(int argc, char *argv[]);


/***THAICUT.C****************************************************************/
unsigned char *FINDCUT(char *lm,char *eot,char *rm);

/***GRPH.C*******************************************************************/
char *readgraph(char *filename);
paintlinegraph(char *graph,unsigned linewant);
paintgraph(char *graph,unsigned linewant);

/* cwsetup.c */
void cwsetup(int argc, char *argv[]);
void usage(void);
void set_directory(void);
int  readoption(search_file_mode mode);
void saveoption(search_file_mode mode);
void blankoption(int y);
void dispoption(int i);
void setoption(void);
void initvalue(unsigned *x, unsigned *y);
void edit_font(void);
void offbreak(void);

/***MENU.C*******************************************************************/
char *savescrn(int x1,int y1,int x2,int y2);
void resscrn(char *scrnindex,int x1,int y1,int x2,int y2);
int selectmenu(unsigned x,unsigned y,unsigned number,char *message[],unsigned *curmenu);
void headmenu(unsigned topmenu,char attr);
unsigned findcurmenu(unsigned lev1,unsigned lev2,unsigned lev3,unsigned lev4);
void findcurlevel(unsigned curmenu,unsigned *lev1,unsigned *lev2,unsigned *lev3,unsigned *lev4);
unsigned pulled_down_menu(unsigned *curmenu,unsigned *x,unsigned *y);
void print_file(void);
void showerrno(void);

/* TUTIL1.c */
unsigned thaistrlen(register unsigned char *thaist);
unsigned linearcolumn(char *textstream, unsigned thaicol, font_attr *font);

/***GETSTR.C*****************************************************************/
int getstring(char textst[],unsigned x,unsigned y,
                unsigned maxlen,char attr,strtype mode);
int getname(char textst[],unsigned x,unsigned y,unsigned maxlen,char attr);
