/****************************************************************************/
/*  VAR.C 15 JAN 89                                                         */
/*  var.c combine with var.h By Noom Suttipong Sun  07-30-1989  15:37:35    */
/*  Updated: Suttipong Kanakakorn                                           */
/*         : Mon  07-31-1989  20:13:05                                      */
/*  CUCC   : Add more comments						    */
/****************************************************************************/

#ifndef __VAR_H_
#define __VAR_H_

#ifdef  DEFINE_VAR
#define EXTERN
#else
#define EXTERN extern
#endif

/* =================================================================== */
/* First part can define and declare with the same attribute           */

struct line_node {
       struct line_node *previous;     /* pointer to previous line */
       struct line_node *next;         /* pointer to next line */
       unsigned char *text;            /* pointer to text stream */
       unsigned char *graph;           /* pointer to graph */
       char wrap;                      /* wrap flag for each line */
};

EXTERN struct line_node *sentinel;  /* sentinel of linked list */
EXTERN struct line_node *curpage;   /* pointer to current page */
EXTERN struct line_node *curline;   /* pointer to current line */

struct  windowtype {
        unsigned row;              /* row of upper left corner */
        unsigned col;              /* column of upper left corner */
        unsigned length;           /* no. of maximum column */
        unsigned width;            /* no. of line per page */
};
EXTERN struct  windowtype wind;

struct Workline {
       char topest[MAXCOL+3];
       char upper[MAXCOL+3];
       char middle[MAXCOL+3];
       char below[MAXCOL+3];
       char attr[MAXCOL+3];
};
EXTERN struct Workline workline;

EXTERN char macro[10][36];

struct blockstruct {
       unsigned lineno;
       unsigned column;
};
EXTERN struct blockstruct blkbegin,blkend;

EXTERN char *graphbuff[13];

EXTERN char dfont[20 * 256];
EXTERN char ditalicfont[20 * 256];

EXTERN char *progname;   /* point to argv[0] */
EXTERN char cw_dir[MAXDRIVE+MAXDIR];  /* which directory is cw.exe in */
EXTERN char filename[MAXPATH]; /* filename that is currently edited */

EXTERN char source[80];  /* source of string to find */
EXTERN char replace[80]; /* string to replace the souce text */
EXTERN char option[10];  /* replace option */
EXTERN int keymain;

EXTERN int tab[MAXCOL];   /* tab set */

/* End first part                                                       */
/* ==================================================================== */
/* Second part                                                          */
/* We can't init extern variables , just declare it with extern         */
/* We'll define it in one place only                                    */

#ifndef DEFINE_VAR

/* Just declare these variables only */

EXTERN boolean thaimode;    /*  current mode is thai or eng */
EXTERN boolean insertmode;  /*  toggle insert mode ON/OFF   */
EXTERN boolean replaceflag;
EXTERN boolean relmargin;
/* กำหนดขอบเขตการแสดงผลบนจอภาพเก็บไว้ในตัวแปร window */

EXTERN unsigned firstcol;      /* first column(physical) to be displayed onscreen */
EXTERN unsigned lineno;        /* line number of current line (origin 1)          */
EXTERN unsigned fontused;      /* current attribute of character e.g. BOLD ITALIC */
EXTERN boolean dispblock;          /* display block or not                            */

EXTERN boolean quitprog;
EXTERN boolean changeflag;    /* ตัวแปรที่ใช้บอกว่า File ที่อ่านเข้ามา EDIT มีการเปลี่ยนแปลงแล้วหรือยัง    */
EXTERN boolean pagecomplete;  /* ตัวแปรที่ใช้บอกว่าการแสดงผลในการ refresh screen สมบูรณ์แล้วหรือยัง   */

EXTERN boolean cu_song;
EXTERN boolean stdcode;     /*  stdcode or KUcode */
EXTERN boolean pagebreak;   /*  show pagebreak line or not */
EXTERN boolean wordwrap;    /*  tudcome automatic or not */
EXTERN boolean create_bak;  /*  create .bak  or not */
EXTERN int leftmar;     /*  left margin */
EXTERN int rightmar;    /*  right margin */
EXTERN int lineperpage; /*  number of lines perpage */

EXTERN unsigned command_tab[];
    
EXTERN char protect1;
EXTERN char protect2;

#else
/* EXTERN will be declared as null already */

EXTERN boolean thaimode = YES;    /*  current mode is thai or eng */
EXTERN boolean insertmode = YES;  /*  toggle insert mode ON/OFF   */
EXTERN boolean replaceflag = NO;
EXTERN boolean relmargin = NO;

/* กำหนดขอบเขตการแสดงผลบนจอภาพเก็บไว้ในตัวแปร window */

EXTERN unsigned firstcol = 0;      /* first column(physical) to be displayed onscreen */
EXTERN unsigned lineno = 1;        /* line number of current line (origin 1)          */
EXTERN unsigned fontused = 0;      /* current attribute of character e.g. BOLD ITALIC */
EXTERN boolean dispblock = YES;    /* display block or not                            */

EXTERN boolean quitprog = NO;
EXTERN boolean changeflag = NO;    /* ตัวแปรที่ใช้บอกว่า File ที่อ่านเข้ามา EDIT มีการเปลี่ยนแปลงแล้วหรือยัง    */
EXTERN boolean pagecomplete = YES; /* ตัวแปรที่ใช้บอกว่าการแสดงผลในการ refresh screen สมบูรณ์แล้วหรือยัง   */

/* configuration value */
EXTERN boolean cu_song = -1;
EXTERN boolean stdcode = -1;     /*  stdcode or KUcode */
EXTERN boolean pagebreak = -1;   /*  show pagebreak line or not */
EXTERN boolean wordwrap = -1;    /*  tudcome automatic or not */
EXTERN boolean create_bak = -1;  /*  create .bak  or not */
EXTERN int leftmar  = -1;     /*  left margin */
EXTERN int rightmar = -1;     /*  right margin */
EXTERN int lineperpage = -1;  /*  number of lines perpage */

EXTERN unsigned command_tab[] = {
    0x1400,0x1401,
    0x1800,ALTX,

    0x2110,CNTRL_S,
    0x2120,CNTRL_D,
    0x2130,CNTRL_E,
    0x2140,CNTRL_X,
    0x2150,CNTRL_F,
    0x2160,CNTRL_A,
    0x2210,CNTRL_W,
    0x2220,CNTRL_Z,
    0x2230,CNTRL_R,
    0x2240,CNTRL_C,
    0x2310,HOMEKEY,                              /* Begin of Line */
    0x2320,0x2301,                               /* Top of Page */
    0x2330,CPGUPKEY,                             /* Top of File */
    0x2410,ENDKEY,                               /* End of Line */
    0x2420,0x2401,                               /* Bottom of Page */
    0x2430,CPGDNKEY,                             /* Bottom of File */
    0x2500,CNTRL_I,                              /* Goto TAB pos */
    0x2600,ALTP, 				 /* Goto Page */
    0x2700,ALTL,                                 /* Goto Line */

    0x3100,CNTRL_G,
    0x3200,BSKEY,
    0x3300,CNTRL_T,
    0x3400,CNTRL_Y,
    0x3500,ALTY,
    0x3600,CNTRL_N,
    0x3700,CNTRL_V,
    0x4100,F1KEY,
    0x4200,F2KEY,
    0x4300,F3KEY,
    0x4400,F4KEY,
    0x4500,F5KEY,
    0x4600,F6KEY,
    0x4700,F7KEY,
    0x4800,F8KEY,
    0x4900,F10KEY,

/***** Modified *****/
    0x4a10,ALTA,
    0x4a20,ALTB,
    0x4a30,ALTC,
    0x4a40,ALTD,
    0x4a50,ALTE,
    0x4a60,ALTF,
    0x4a70,ALTH,
    0x4a80,ALTI,
    0x4a90,ALTJ,
    0x4aA0,ALTN,
    0x4aB0,ALTO,
    0x4aC0,ALTQ,
    0x4aD0,ALTR,
    0x4aE0,ALTS,
    0x4aF0,ALTT,
    0x4a01,ALTU,
    0x4a11,ALTV,
    0x4a21,ALTY,
/************************/
    0x5100,0x5101,
    0x5200,0x5201,
    0x5300,CNTRL_L,
    0x6100,0x6101,
    0x6200,0x6201,
    0x6300,0x6301,
    0x6400,0x6401,
    0x6500,0x6501,
    0x6600,0x6601,
    0x6700,0x6701,
    0x6800,0x6801,
    0x7100,0x7101,
    0x7200,0x7201,
    0x7300,0x7301,
    0x7400,0x7401,
    0x7500,0x7501,
    0x7600,0x7601,
    0x7700,0x7701,

    /********** Modified ************/
    0x8310,0x8111,
    0x8320,0x8211,
    0x8330,0x8311,
    0x8340,0x8411,
    0x8350,0x8511,
    0x8360,0x8611,
    0x8370,0x8711,
/*
    0x8100,ALTG,
    0x8200,ALTD,
    0x9100,CNTRL_B,
    0x9200,F9KEY,
    0x9500,ALTY,
    0x9600,ALTM,
*/
    0x8100,CNTRL_B,
    0x8200,F9KEY,
    0x8500,0x8501,
    0x8600,ALTM,
    0,0
    };
    
EXTERN char protect1 = 0x00;
EXTERN char protect2 = 0x00;
#endif

#undef EXTERN /* restore it */

#endif /* __VAR_H */
