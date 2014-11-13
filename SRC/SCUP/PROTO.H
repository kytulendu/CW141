/* function prototype for cuprint, (use in prjcuw\scup only)
   Written Suttipong Kanakakorn Mon  08-07-1989  00:29:50
*/

/* kbd.c */
void waitkbd(unsigned x, unsigned y);
int readkbd(void);

/* getstr.c */
int getstring(char textst[], int x, int y, int maxlen, font_attr attr);
int getname(char textst[], int x, int y, int maxlen, font_attr attr);
int getnumber(char textst[], int x, int y, int maxlen, font_attr attr);

/* tutil1.c */
int thaistrlen(char *thaist);

/* error.c */
void disperror(char *s);

/* pconfig.c */
int savepconfig(void);
int readpconfig(void);

/* ldfont.c */
void loadfont(void);

/* pmenu.c */
void writemenu(int row, int col, font_attr attr);
void writeallmenu(void);
void menuselect(void);
void prtwriteallmenu(void);
void prtwritemenu(int row, font_attr attr);
void prtmenuselect(void);
void setprinter(int smallpaper, int printer24pin);
void prtdocommand(int row);
void modewriteallmenu(void);
void modewritemenu(int row, int attr);
void modemenuselect(void);
void modedocommand(int row);
void titlewriteallmenu(void);
void titlewritemenu(int row, int attr);
void titlemenuselect(void);
void titledocommand(int row);

/* printing.c */
void printing(void);
char *setpageformat(char form[], int max);
void initializebufferandfont(void);
void contostd(char *t);
void clear_message(void);

/* prt.c */
void AllocateBuffer9pin(void);
int PrinterLoadFont9pin(void);
int  islevel(int *pline);
void PrinterLoadLine(char pline[]);
void PrinterLoadLine9pin(char line[]);

/* prtutil.c */
int find_line_perpage(void);
int find_max_line_perpage(void);
int find_max_userline_perpage(void);
int find_line_space(void);
void set_all_lineperpage(set_linespace_mode mode);
void line_feed(unsigned n);
void skip_line(unsigned n);

/* prtt.c */
void PrinterLoadLineText(unsigned char line[]);

/* prncntrl.c */
void putp(unsigned c);
int  PrinterReadstatus(void);
void PrinterInitialize(void);
void PrinterSetFormLenghtInch(unsigned n);
void PrinterSetFormLenghtLine(unsigned n);
void PrinterSkipLine(int n);
void PrinterFormFeed(void);
void PrinterLineFeed(unsigned n);
void PrinterMasterSelectMode(unsigned n);
void PrinterSetGraphicMode9pinQuadrupleDensity(unsigned n);
void PrinterSetGraphicMode24pinTripleDensity(unsigned n);
void PrinterSet10cpi(void);

/* dot.c */
void dotcommand(char string[]);
void mailmerge(char stringin[], char stringout[]);
int blankskip(char *s);

/* prtbuf24.c */
void PrintThree24pin(int uindex, int mindex, int lindex);
void PrintBuffer24pin(char buf1[], char buf2[], char buf3[], int maxscan);

/* prtg.c */
void PrintThree9pin(void);

/* pic.c */
void read_picture_file(void);

/* cpi.c */
void charwidth(char buf1[]);

/* pic.c */
void get_argument(char arg[][40],char st[],int *no_arg);
void analyze_picture_file(char *st);
void initialize_read(char *st);

/* cwsetup.c */
void cupsetup(int argc, char *argv[]);
void usage(void);
void set_directory(void);
void readoption(search_file_mode mode);
void saveoption(search_file_mode mode);
void readfont(void *font_buffer, char *fontname);
int handler(int errval,int ax,int bp,int si);
void *cp_loadfont (char *fname, unsigned font_size);

/* 24pins.c (24pins.h) */
void	execerror ( char *s, char *t);

/* smenu.c */
int smenu(register int x, register int y, int number,
            char *messege[], int *curmenu);
unsigned find_menu_len(register int number, char *messege[]);
