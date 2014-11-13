/* --------------------------------------------------------------------- */
/*	Module		:	24PINS.H				 */
/*	Description	:	define characteristic used in 24pins.C   */
/*	Written by	:	Mr. Thotsapol Aphikulvanich ID.2818966	 */
/*	Date written	:	23 April 1989				 */
/*	Last modified	:	3  June  1989				 */
/*      Modified By CUCC                                                 */
/*      : 22 June 1990  Extend Bar Table                                 */
/* --------------------------------------------------------------------- */

/* ----- define error and success value ------- */
#define	CP_ERROR	-1
#define CP_SUCCESS	0
#define	CP_WRAPCODE	0xA0
#define	CP_BLANK	0x20
#define	CP_ENGLISH	0x80

/* ----- define	sara and wannayok ---------- */
#define	CP_SARAAR	0xD2
#define	CP_SARAUM	0xD3
#define	CP_HUNARKAD	0xD1
#define	CP_SARAI	0xD4
#define	CP_SARAE	0xD5
#define	CP_SARAUD	0xD6
#define	CP_SARAEE	0xD7

#define	CP_MIAKE	0xE8
#define	CP_MITO		0xE9
#define	CP_MITEE	0xEA
#define	CP_MIJUDTAWA	0xEB
#define	CP_KARUN	0xEC

/* ---- define printing mode of output ------- */
#define	CP_TEXT		0
#define	CP_DOWNLOAD	1
#define	CP_GRAPHICS	2

/* ---- define quality of output printing ---- */
#define	CP_DRAFT	0
#define CP_LQ		1

/* ---- define print size ----- */
#define CP_10CPI	0
#define	CP_12CPI	1
#define	CP_15CPI	3

/* ---- define head type ------ */
#define	CP_24PINS	0
#define	CP_9PINS	1
#define	CP_HPLASER	2
#define	CP_OLYMPIA	3

/* ---- define size of 24 pins printer font ---- */
#define	CP_LQ_FONTSIZE		54
#define	CP_LQSCR_FONTSIZE	36
#define	CP_LQ_TABSIZE		12096		/* SPACE -> 0xFF */
#define	CP_LQSCR_TABSIZE	8064		/* SPACE -> 0xFF */
#define	CP_LQCOMB_TABSIZE	1620		/* Combine code table */
#define	CP_LQSCRCOMB_TABSIZE	1080

/* ---- define printing buffer size ----------- */
#define	CP_MAX_TEXTBUF		272
#define	CP_LQ_MAXPRBUF		272 * CP_LQ_FONTSIZE

/* ---- define printing control code ---------- */
#define	CP_NORMALATTR		0x00
#define	CP_ITALICATTR		0x01
#define	CP_SUBATTR		0x02
#define	CP_SUPERATTR		0x04

#define	CP_UNDERLINEATTR	0x08
#define	CP_DOUBLELINEATTR	0x10

#define	CP_ENLARGEATTR		0x20
#define	CP_BOLDATTR		0x40

#define	CP_ITALIC		23	/* ^W */
#define	CP_SUBSCRIPT		22	/* ^V */
#define	CP_SUPERSCRIPT		20	/* ^T */
#define	CP_ENLARGE		05	/* ^E */
#define	CP_BOLD			02	/* ^B */
#define	CP_UNDERLINE		19	/* ^S */
#define	CP_DOUBLELINE		18	/* ^R */

#define	CP_UNDERLINE_MSK	0x18
#define	CP_DOUBLELINE_MSK	0x33

#define	MSK_CHARTYPE(ch)	( ch & 0x07 )
#define	MSK_LINE(ch)		( ch & 0x18 )
#define	MSK_CHARATTR(ch)	( ch & 0xE0 )

/* ---- used as buffer before printing to print buffer ---- */
struct	cp_textbuf {
	unsigned char	 *uppest;	/* buffer for wannayok	  */
	unsigned char	 *upper;	/* buffer for upper line  */
	unsigned char	 *middle;	/* buffer for middle line */
	unsigned char	 *lower;	/* buffer for lower line  */
};

/* ---- graphics printing buffer ------ */
#define	CP_LQCOL	18
#define	CP_LQROW	3
#define	CP_LQSCRROW	2

typedef	struct	{
	unsigned char	font[CP_LQCOL][CP_LQROW];
} FONT;
typedef struct	{
	unsigned char	font[CP_LQCOL][CP_LQSCRROW];
} FONTSCR;
struct	cp_grlqbuf {
	FONT	*upper;
	FONT	*middle;
	FONT	*lower;
        FONT    *extbar;
};
/* ---- define buffer used in 24pins.c module ----- */
extern  struct	cp_textbuf	cp_buf;
extern	struct	cp_grlqbuf	cp_lqbuf;

/* ----- define character font table  -------- */
extern	FONT	*cp_lqnorm;
extern	FONT	*cp_lqitalic;
extern	FONT	*cp_lqnorm_comb;
extern	FONT	*cp_lqitalic_comb;
extern	FONTSCR	*cp_lqscr;
extern	FONTSCR	*cp_lqscr_italic;
extern	FONTSCR	*cp_lqscr_comb;
extern	FONTSCR	*cp_lqscr_italic_comb;

/* ------ define control byte for character attribute ----- */
extern	unsigned char cp_control;

/* ----- define prototype of function in 24pins.c module ----- */
void	execerror ( char *s, char *t);
void	cp_clearbuf (void);
void	cp_split4level (unsigned char *linemsg);
void	cp_init_textbuf ( void);
void	cp_init_grpbuf  ( void);

/* ----- NORMAL and ITALIC CODE ----- */
void	cp_combine_fontlq ( FONT *combcode, FONT *sara, FONT *wannayok);
FONT	*cp_create_lqcombine ( FONT *fntable);
FONT	*cp_lq_font ( FONT *fntable, unsigned char ch);
FONT	*cp_lq_combinecode ( FONT *fntable, FONT *combtable,
	   unsigned char sara, unsigned char wannayok);

/* --- SUBCRIPT and SUPERSCRIPT --- */
void	cp_combine_fontlqscr(FONTSCR *combcode,FONTSCR *sara,FONTSCR *wannayok);
FONTSCR	*cp_create_lqscrcombine ( FONTSCR *fntable);
FONTSCR	*cp_lqscr_font ( FONTSCR *fntable, unsigned char ch);
FONTSCR	*cp_lqscr_combinecode ( FONTSCR *fntable, FONTSCR *combtable,
	   unsigned char sara, unsigned char wannayok);

/* -- print data to buffer and manage all attribute -- */
FONT	*cp_lqscr_blank( FONT *buf);

FONT	*cp_sub_middle_lower ( FONT *buf, FONTSCR *middle, FONTSCR * lower);
FONT	*cp_sub_upper_middle( FONT *buf, FONTSCR *upper, FONTSCR *middle);

FONT	*cp_super_upper_middle ( FONT *buf , FONTSCR *upper, FONTSCR *middle);
FONT	*cp_super_middle_lower ( FONT * buf, FONTSCR *middle, FONTSCR *lower);

FONT	*cp_underline ( FONT *newchar, FONT *oldchar);
FONT	*cp_doubleline ( FONT *newchar, FONT *oldchar);

int	cp_enlarge( struct cp_grlqbuf buf, int index);

void	cp_bold ( struct cp_grlqbuf buf, int index);

int	cp_pr_lqbuf ( struct cp_grlqbuf lqbuf, struct cp_grlqbuf grpchar,
		       int gindex);
int cp_printlq(void);
void	cp_printing (int cpi, int nchar);

/* --- set control flag --- */
int	cp_set_reset ( unsigned char ch, unsigned char *cntptr);
