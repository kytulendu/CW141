#define BYTE_X(X)	(_ceil((X),8))
#define STOP	0
#define EDIT_FONT	1
#define SELECT_FONT	2

#define MAX_X   64          /*  maximum of font size  */
#define MAX_Y   64

#define SCR_FONT	1
#define ITALIC_SCR_FONT	2
#define PRN_FONT	3
#define ITALIC_PRN_FONT	4
#define LQ_FONT		5
#define ITALIC_LQ_FONT	6
#define SUP_SUB_LQ_FONT	7
#define ITALIC_SUP_SUB_LQ_FONT	8
#define FED_FONT	9

extern int first_file;
extern unsigned pos_x,pos_y,BYTE,sel_x,sel_y;
extern char WIDTH_X,WIDTH_Y,X,Y;
extern char *all_font;
extern char *char_block_image;
extern int align;
extern int loaded;
extern int new_load;
extern char fname[];
extern unsigned int is_herc;
extern char cufontpath[];
extern char cuwordpath[];
extern char cuprintpath[];

#define SIGN "FONT EDITOR Version 1.0"
#define	SIGN_LENGTH strlen(SIGN)
