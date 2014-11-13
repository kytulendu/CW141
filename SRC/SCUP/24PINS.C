/* -------------------------------------------------------------------- */
/*      Module          :       24PINS.C                                */
/*      Description     :       CU-PRINT support routine for printing   */
/*                              output to printer 9 pins and 24 pins    */
/*      Written by      :       Mr. Thotsapol Aphikulvanich ID.2818966  */
/*      Date written    :       26 March 1989                           */
/*      Last modified   :       18 April 1989                           */
/*      Modified by     :       Mr. Suttipong Kanakakorn    ID.2718098  */
/*      Last modified   :       Tue  08-15-1989  10:10:08               */
/*      Modified by     :       CUCC                                    */
/*      Last modified   :       22 June 1990                            */
/* -------------------------------------------------------------------- */
#include        <io.h>
#include        <fcntl.h>
#include        <stdio.h>
#include        <string.h>
#include        <alloc.h>
#include        <dos.h>
#include        "24pins.h"
#include        "cpthai.h"
#include        "table.h"
#include        "..\common\cwgrphc.h"    /* modified */
#include        "global.ext"

/* ---- define buffer used in 24pins.c module ----- */
struct  cp_textbuf      cp_buf;
struct  cp_grlqbuf      cp_lqbuf;

/* ----- define character font table  -------- */
FONT    *cp_lqnorm;
FONT    *cp_lqitalic;
FONT    *cp_lqnorm_comb;
FONT    *cp_lqitalic_comb;
FONTSCR         *cp_lqscr;
FONTSCR         *cp_lqscr_italic;
FONTSCR         *cp_lqscr_comb;
FONTSCR         *cp_lqscr_italic_comb;

/* ------ define control byte for character attribute ----- */
unsigned char cp_control;

/* --------------------------------------------------------------------         */
/*      Function        :       execerror                               */
/*      Description     :       report error and terminate program      */
/*      Parameters      :       s - general error                       */
/*                              t - extended error                      */
/*      Return values   :       NONE                                    */
/*      Remark          :       This function causes the program to stop*/
/* -------------------------------------------------------------------- */
void    execerror (char *s, char *t)
{
        char se[80];
        strcpy(se,s);
        strcat(se,t);
        disperror(se);
/*      settext();              /* Change to text mode */
        fprintf (stderr, "%s", s);
        if (t)
                fprintf (stderr, "%s", t);   */
        settext();
        exit (1);
}
/* -------------------------------------------------------------------- */
/*      Function        :       cp_clearbuf                             */
/*      Description     :       clear 4 lines buffer                    */
/*      Parameters      :       NONE                                    */
/*      Return value    :       NONE                                    */
/*      Remark          :       used global variable cp_buf             */
/* -------------------------------------------------------------------- */
void    cp_clearbuf()
{
        memset (cp_buf.uppest, CP_BLANK, CP_MAX_TEXTBUF);
        memset (cp_buf.upper,  CP_BLANK, CP_MAX_TEXTBUF);
        memset (cp_buf.middle, CP_BLANK, CP_MAX_TEXTBUF);
        memset (cp_buf.lower,  CP_BLANK, CP_MAX_TEXTBUF);
        /* ---- clear printing font buffer ----- */
        memset (cp_lqbuf.upper,  0, CP_LQ_MAXPRBUF);
        memset (cp_lqbuf.middle, 0, CP_LQ_MAXPRBUF);
	memset (cp_lqbuf.lower,  0, CP_LQ_MAXPRBUF);
	memset (cp_lqbuf.extbar, 0, CP_LQ_MAXPRBUF);
}
/* -------------------------------------------------------------------- */
/*      Function        :       cp_split4level                          */
/*      Description     :       split thai message into 4 levels        */
/*      Parameters      :       linemsg         - message to be printed */
/*      Return values   :       NONE                                    */
/*      Remark          :       global variable "cp_buf" is result      */
/* -------------------------------------------------------------------- */
void     cp_split4level (unsigned char *linemsg)
{
        int      index = 0;     /* index of thai 3 lines message */
        int      lineind = 0;   /* index to line message         */
        unsigned char prv;      /* previous upper sara           */
        unsigned char ch,tch;   /* temporary character           */

        while ( (ch = linemsg[lineind]) != 0) {
                tch = ch - CP_ENGLISH;
                if ( (ch < CP_ENGLISH) || cp_ismidlev(tch) ) {
                     if ( ch < CP_BLANK )
                        cp_buf.upper[index] = cp_buf.lower[index] =
                        cp_buf.middle[index] = cp_buf.uppest[index] = ch;
                     else {
                        if ( ch == CP_SARAUM) {
                             cp_buf.middle[index] = CP_SARAAR;
                             if (index != 0 )
                                cp_buf.upper[index-1] = CP_SARAUM;
                        }
                        else cp_buf.middle[index] = ch;
                     };
                     index++;
                }
                else {
                     if ( cp_islowlev(tch)) {
                        if (cp_buf.lower[index-1] != CP_BLANK ) index++;
                        cp_buf.lower [index - 1] = ch;
                     }
                     else {
                        if ( cp_isuplev(tch))  {
                           if (cp_buf.upper[index-1] != CP_BLANK ) index++;
                           cp_buf.upper [index - 1] = ch;
                        }
                        else { /* -- Uppest level -- */
                            if (cp_buf.uppest[index-1] != CP_BLANK ) index++;
                            cp_buf.uppest [ index - 1 ] = ch;
                        }; /* -- End Upper level -- */
                     }; /* ---- end Low level IF ---- */
                }; /* --- end CONTROL IF ---- */
                lineind++;      /* ---  increase linemsg index --- */
        }; /* -- end while -- */
/* ---- Add NULL string to end of buffer ---- */
        cp_buf.upper[index] = cp_buf.lower[index] = cp_buf.middle[index] = cp_buf.uppest[index] = 0;
}
/* -------------------------------------------------------------------- */
/*      Function        :       cp_lq_font                              */
/*      Description     :       search character in font table          */
/*      Parameters      :       fntable - pointer to the font table     */
/*                      :       ch      - character to be search        */
/*      Return values   :       pointer to searching font               */
/* -------------------------------------------------------------------- */
FONT    *cp_lq_font( FONT *fntable, unsigned char ch)
{
        int     index;                  /* index to ch in font table */

        index = ch - CP_BLANK;
        return ( fntable + index );
}

/* -------------------------------------------------------------------- */
/*      Function        :       cp_lq_combinecode                       */
/*      Description     :       searching combine sara and wannayok     */
/*      Parameters      :       fntable   - pointer to font table       */
/*                              combtable - pointer to combine table    */
/*                              sara      - sara ASCII code             */
/*                              wannayok  - wannayok ASCII code         */
/*      Return values   :       pointer to new combine character        */
/* -------------------------------------------------------------------- */
FONT     *cp_lq_combinecode ( fntable, combtable, sara, wannayok)
FONT     *fntable, *combtable;
unsigned char sara, wannayok;
{
        int     index;                  /* index to ch in font table */

        if (sara == CP_BLANK )
                index = wannayok - CP_BLANK;
        else if ( wannayok == CP_BLANK )
                index = sara - CP_BLANK;
        else {                          /* Combine function must be here */
                switch (sara) {
                        case CP_HUNARKAD :
                                index = wannayok - CP_MIAKE;
                        break;
                        case CP_SARAUM :
                                index = 5 + wannayok - CP_MIAKE;
                        break;
                        case CP_SARAI  : case CP_SARAE  :
                        case CP_SARAUD : case CP_SARAEE :
                                index = 5*(sara-CP_SARAI+2)+(wannayok-CP_MIAKE);
                        break;
                        default :
                                return ( fntable + wannayok);
                };
                return ( combtable + index);
        }
        return ( fntable + index);
}
/* --------------------------------------------------------------------         */
/*      Function        :       cp_lqscr_font                           */
/*      Description     :       search character in font table          */
/*      Parameters      :       fntable         - pointer to the font table     */
/*                      :       ch      - character to be search        */
/*      Return values   :       pointer to searching font               */
/* -------------------------------------------------------------------- */
FONTSCR         *cp_lqscr_font( FONTSCR *fntable, unsigned char ch)
{
        int     index;                  /* index to ch in font table */

        index = ch - CP_BLANK;
        return ( fntable + index );
}
/* --------------------------------------------------------------------         */
/*      Function        :       cp_lqscr_combinecode                    */
/*      Description     :       searching combine sara and wannayok     */
/*      Parameters      :       fntable           - pointer to font table       */
/*                              combtable - pointer to combine table    */
/*                              sara      - sara ASCII code                     */
/*                              wannayok  - wannayok ASCII code                 */
/*      Return values   :       pointer to new combine character        */
/* -------------------------------------------------------------------- */
FONTSCR          *cp_lqscr_combinecode ( fntable, combtable, sara, wannayok)
FONTSCR          *fntable, *combtable;
unsigned char sara, wannayok;
{
        int     index;                  /* index to ch in font table */

        if (sara == CP_BLANK )
                index = wannayok - CP_BLANK;
        else if ( wannayok == CP_BLANK )
                index = sara - CP_BLANK;
        else {                          /* Combine function must be here */
                switch (sara) {
                        case CP_HUNARKAD :
                                index = wannayok - CP_MIAKE;
                        break;
                        case CP_SARAUM :
                                index = 5 + wannayok - CP_MIAKE;
                        break;
                        case CP_SARAI  : case CP_SARAE  :
                        case CP_SARAUD : case CP_SARAEE :
                                index = 5*(sara-CP_SARAI+2)+(wannayok-CP_MIAKE);
                        break;
                        default :
                                return ( fntable + wannayok);
                };
                return ( combtable + index);
        }
        return ( fntable + index);
}
/* ---------------------------------------------------------------------- */
/*      Function        :       cp_pr_lqbuf                               */
/*      Description     :       printing data to buffer                           */
/*      Parameters      :       buffer  - pointer to printing buffer      */
/*                              data    - data to printing                */
/*                              index   - index of last printing position */
/*      Return values   :       CP_ERROR  - when error occurs             */
/*                              gindex    - last to printing position     */
/* ---------------------------------------------------------------------- */
int     cp_pr_lqbuf ( grpbuf, grpchar, index )
struct  cp_grlqbuf grpbuf;    /* printing buffer                        */
struct  cp_grlqbuf grpchar;     /* data to be printing in buffer        */
int     index;                  /* index to the last printing position  */
{
        if (index >= CP_MAX_TEXTBUF )
                return ( CP_ERROR );
        memcpy (grpbuf.upper+index , grpchar.upper,  CP_LQ_FONTSIZE);
        memcpy (grpbuf.middle+index, grpchar.middle, CP_LQ_FONTSIZE);
        memcpy (grpbuf.lower+index , grpchar.lower,  CP_LQ_FONTSIZE);
        memcpy (grpbuf.extbar+index, grpchar.extbar, CP_LQ_FONTSIZE);
        index++;                /* increase index of buffer */
        return (index);
}

/* ---------------------------------------------------------------------- */
/*      Function        :       cp_set_reset                              */
/*      Description     :       set or reset mask bits of attrib control  */
/*      Parameters      :       ch       - input character                */
/*                              cntptr   - pointer to control             */
/*      Return values   :       NONE                                      */
/* ---------------------------------------------------------------------- */
int      cp_set_reset(ch, cntptr)
unsigned char   ch;
unsigned char   *cntptr;
{
        switch (ch) {
                case    CP_ITALIC :
                        if ((*cntptr) & CP_ITALICATTR)
                                *cntptr &= ~CP_ITALICATTR;
                        else
                                *cntptr         |= CP_ITALICATTR;
                break;
                case    CP_SUBSCRIPT :
                        if ((*cntptr) & CP_SUBATTR)
                                *cntptr &= ~CP_SUBATTR;
                        else {
                                *cntptr         |= CP_SUBATTR;
                                *cntptr &= ~CP_SUPERATTR;
                        }
                break;
                case    CP_SUPERSCRIPT :
                        if ((*cntptr) & CP_SUPERATTR)
                                *cntptr &= ~CP_SUPERATTR;
                        else {
                                *cntptr         |= CP_SUPERATTR;
                                *cntptr &= ~CP_SUBATTR;
                        }
                break;
                case    CP_ENLARGE :
                        if ((*cntptr) & CP_ENLARGEATTR)
                                *cntptr &= ~CP_ENLARGEATTR;
                        else
                                *cntptr         |= CP_ENLARGEATTR;
                break;
                case    CP_BOLD :
                        if ((*cntptr) & CP_BOLDATTR)
                                *cntptr &= ~CP_BOLDATTR;
                        else
                                *cntptr         |= CP_BOLDATTR;
                break;
                case    CP_UNDERLINE :
                        if ((*cntptr) & CP_UNDERLINEATTR)
                                *cntptr &= ~CP_UNDERLINEATTR;
                        else {
                                *cntptr &= ~CP_DOUBLELINEATTR;
                                *cntptr         |= CP_UNDERLINEATTR;
                        };
                break;
                case    CP_DOUBLELINE :
                        if ((*cntptr) & CP_DOUBLELINEATTR)
                                *cntptr &= ~CP_DOUBLELINEATTR;
                        else {
                                *cntptr &= ~CP_UNDERLINEATTR;
                                *cntptr         |= CP_DOUBLELINEATTR;
                        };
                break;

        }
}
/* ------------------------------------------------------------------------ */
/*      Function        :       cp_printlq(void)                            */
/*      Description     :       printing 4 level data to printing buffer    */
/*      Parameters      :       NONE                                        */
/*      Return values   :       CP_ERROR - when error occurs or buffer full */
/*                              gindex  - number of printing character      */
/*      Remark          :       use GLOBAL variable "cp_lqbuf"              */
/* ------------------------------------------------------------------------ */
int     cp_printlq(void)
{
        int     index=0, gindex=0;              /* index to character line */

        struct  cp_grlqbuf grpchar;

        FONT    upper_buf, middle_buf, lower_buf;
        FONTSCR         *scr_upper, *scr_middle, *scr_lower;

        unsigned char uppest, upper, middle, lower, ch;

        while ((ch = cp_buf.middle[index]) != NULL) {
           if ( ch >= CP_BLANK ) {
              if ( cp_istablechar(ch) ) {  /* ---- table treating ---- */
                switch (ch) {
                  case CP_TOPLEFT : case CP_TOPRIGHT : case CP_TOPMIDDLE :
                  case CP_LEFTUP  : case CP_RIGHTUP  :
                        upper = CP_BLANK;
                        middle = ch;
                        lower = CP_VERTICAL;
                  break;
                  case CP_MIDLEFT : case CP_MIDMIDDLE : case CP_MIDRIGHT :
                  case CP_VERTICAL :
                        upper = lower = CP_VERTICAL;
                        middle = ch;
                  break;
                  case CP_BOTTOMLEFT  : case CP_BOTTOMMIDDLE :
                  case CP_BOTTOMRIGHT :
                  case CP_LEFTBOTTOM  : case CP_RIGHTBOTTOM :
                        upper = CP_VERTICAL ;
                        middle = ch;
                        lower = CP_BLANK ;
                  break;
                  case CP_HORIZONTAL :
                        upper = lower = CP_BLANK;
                        middle = ch;
                  break;
                };
                grpchar.upper  = cp_lq_font (cp_lqnorm, upper);
                grpchar.middle = cp_lq_font (cp_lqnorm, middle);
                grpchar.lower  = cp_lq_font (cp_lqnorm, lower);
                if(extbarprinting)
                  grpchar.extbar = cp_lq_font (cp_lqnorm, lower);
              }
              else {    /* ---- look up attribute and do it ---- */
                if(extbarprinting)
                  grpchar.extbar = cp_lq_font (cp_lqnorm,CP_BLANK);
                uppest = cp_buf.uppest[index];
                upper  = cp_buf.upper[index];
                middle = ch;
                lower  = cp_buf.lower[index];

                switch ( MSK_CHARTYPE(cp_control) ) {
                  case CP_NORMALATTR :
                     grpchar.upper  = cp_lq_combinecode( cp_lqnorm,
                                      cp_lqnorm_comb, upper, uppest);
                     grpchar.middle = cp_lq_font( cp_lqnorm, middle);
                     grpchar.lower  = cp_lq_font( cp_lqnorm, lower);

                     if ( lower == CP_BLANK )
                     switch ( MSK_LINE (cp_control) ) {
                        case CP_UNDERLINEATTR :
                                grpchar.lower = cp_underline (&lower_buf,
                                                              grpchar.lower);
                        break;
                        case CP_DOUBLELINEATTR :
                                grpchar.lower = cp_doubleline (&lower_buf,
                                                               grpchar.lower);
                        break;
                     };
                  break;
                  case CP_ITALICATTR :
                     grpchar.upper  = cp_lq_combinecode( cp_lqitalic,
                                      cp_lqitalic_comb, upper, uppest);
                     grpchar.middle = cp_lq_font( cp_lqitalic, middle);
                     grpchar.lower  = cp_lq_font( cp_lqitalic, lower);
                     if ( lower == CP_BLANK )
                     switch ( MSK_LINE(cp_control) ) {
                        case CP_UNDERLINEATTR :
                                grpchar.lower = cp_underline (&lower_buf,
                                                              grpchar.lower);
                        break;
                        case CP_DOUBLELINEATTR :
                                grpchar.lower = cp_doubleline (&lower_buf,
                                                               grpchar.lower);
                        break;
                     };
                  break;
                  case CP_SUBATTR :
                     scr_upper  = cp_lqscr_combinecode( cp_lqscr,
                                                        cp_lqscr_comb,
                                                        upper, uppest);
                     scr_middle = cp_lqscr_font ( cp_lqscr, middle);
                     scr_lower  = cp_lqscr_font ( cp_lqscr, lower);

                     grpchar.upper  = cp_lqscr_blank ( &upper_buf);
                     grpchar.middle = cp_sub_upper_middle ( &middle_buf ,
                                                            scr_upper ,
                                                            scr_middle );
                     grpchar.lower  = cp_sub_middle_lower ( &lower_buf ,
                                                            scr_middle ,
                                                            scr_lower );
                  break;
                  case CP_SUBATTR | CP_ITALICATTR :
                     scr_upper  = cp_lqscr_combinecode( cp_lqscr_italic,
                                                        cp_lqscr_italic_comb,
                                                        upper, uppest);
                     scr_middle = cp_lqscr_font ( cp_lqscr_italic, middle);
                     scr_lower  = cp_lqscr_font ( cp_lqscr_italic, lower);

                     grpchar.upper  = cp_lqscr_blank ( &upper_buf);
                     grpchar.middle = cp_sub_upper_middle ( &middle_buf ,
                                                            scr_upper ,
                                                            scr_middle );
                     grpchar.lower  = cp_sub_middle_lower ( &lower_buf ,
                                                            scr_middle ,
                                                            scr_lower );
                  break;
                  case CP_SUPERATTR :
                     scr_upper  = cp_lqscr_combinecode( cp_lqscr,
                                                       cp_lqscr_comb,
                                                       upper, uppest);
                     scr_middle = cp_lqscr_font ( cp_lqscr, middle);
                     scr_lower  = cp_lqscr_font ( cp_lqscr, lower);

                     grpchar.upper  = cp_super_upper_middle ( &upper_buf ,
                                                            scr_upper ,
                                                            scr_middle );
                     grpchar.middle = cp_super_middle_lower (&middle_buf,
                                                            scr_middle,
                                                            scr_lower);
                     grpchar.lower  = cp_lqscr_blank (&lower_buf);
                  break;
                  case CP_SUPERATTR | CP_ITALICATTR :
                     scr_upper  = cp_lqscr_combinecode( cp_lqscr_italic,
                                                        cp_lqscr_italic_comb,
                                                        upper, uppest);
                     scr_middle = cp_lqscr_font ( cp_lqscr_italic, middle);
                     scr_lower  = cp_lqscr_font ( cp_lqscr_italic, lower);

                     grpchar.upper = cp_super_upper_middle ( &upper_buf ,
                                                            scr_upper ,
                                                            scr_middle );
                     grpchar.middle = cp_super_middle_lower (&middle_buf,
                                                            scr_middle,
                                                            scr_lower);
                     grpchar.lower = cp_lqscr_blank (&lower_buf);
                  break;
                };
              };
              gindex = cp_pr_lqbuf ( cp_lqbuf, grpchar, gindex);
              if ( gindex == CP_ERROR )
                   return (CP_MAX_TEXTBUF);
        /* ---- Enlarge and Boldface adjust in printing buffer ---- */
              switch (MSK_CHARATTR (cp_control)) {
                  case CP_BOLDATTR :
                     cp_bold ( cp_lqbuf, gindex);
                  break;
                  case CP_ENLARGEATTR :
                     gindex = cp_enlarge ( cp_lqbuf, gindex);
                     if ( gindex == CP_ERROR)
                        return (CP_MAX_TEXTBUF);
                  break;
                  case (CP_BOLDATTR | CP_ENLARGEATTR) :
                     cp_bold ( cp_lqbuf, gindex);
                     gindex = cp_enlarge ( cp_lqbuf, gindex);
                     if ( gindex == CP_ERROR)
                        return (CP_MAX_TEXTBUF);
                  break;
              };
            }
            else   /* --- Control Character --- */
              cp_set_reset (ch, &cp_control);

            index++;            /* increase index of text buffer */
        };

        return (gindex);
}

