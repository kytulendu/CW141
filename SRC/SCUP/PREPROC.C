/* Modified: Suttipong Kanakakorn
             Tue  08-15-1989  00:10:21
             Search for printer font easier
*/

#include        <alloc.h>
#include        <io.h>
#include        <fcntl.h>
#include        <string.h>
#include        <stdio.h>
#include        "24pins.h"
#include        "global.ext"

/* -------------------------------------------------------------------- */
/*      Function        :       cp_init_textbuf                         */
/*      Description     :       initialize memory for text buffer       */
/*      Parameters      :       NONE                                    */
/*      Return values   :       NONE                                    */
/*      Remark          :       use GLOBAL variable "cp_buf"            */
/* -------------------------------------------------------------------- */
void    cp_init_textbuf()
{
        cp_buf.uppest = (unsigned char *) malloc ( CP_MAX_TEXTBUF);
        if (cp_buf.uppest == NULL)
                execerror ("Insufficient memory\n","");

        cp_buf.upper  = (unsigned char *) malloc ( CP_MAX_TEXTBUF);
        if (cp_buf.upper == NULL)
                execerror ("Insufficient memory\n","");

        cp_buf.middle = (unsigned char *) malloc ( CP_MAX_TEXTBUF);
        if (cp_buf.middle == NULL)
                execerror ("Insufficient memory\n","");

        cp_buf.lower  = (unsigned char *) malloc ( CP_MAX_TEXTBUF);
        if (cp_buf.lower == NULL)
                execerror ("Insufficient memory\n","");

        /* ----- clear attribute of text character ----- */
        cp_control = 0;
}
/* -------------------------------------------------------------------- */
/*      Function        :       cp_init_grpbuf                          */
/*      Description     :       initialize memory for text buffer       */
/*      Parameters      :       NONE                                    */
/*      Return values   :       NONE                                    */
/*      Remark          :       use GLOBAL variable "cp_lqbuf"          */
/* -------------------------------------------------------------------- */
void    cp_init_grpbuf()
{
        cp_lqbuf.upper  = (FONT *) malloc (CP_LQ_MAXPRBUF);
        if (cp_lqbuf.upper == NULL)
                execerror ("Insufficient memory\n","");

        cp_lqbuf.middle = (FONT *) malloc (CP_LQ_MAXPRBUF);
        if (cp_lqbuf.middle == NULL)
                execerror ("Insufficient memory\n","");

        cp_lqbuf.lower  = (FONT *) malloc (CP_LQ_MAXPRBUF);
        if (cp_lqbuf.lower == NULL)
                execerror ("Insufficient memory\n","");

        cp_lqbuf.extbar = (FONT *) malloc (CP_LQ_MAXPRBUF);
        if (cp_lqbuf.extbar == NULL)
                execerror ("Insufficient memory\n","");
}
/* -------------------------------------------------------------------- */
/*      Function        :       cp_combine_fontlq                       */
/*      Description     :       Combine upper sara and wannayok         */
/*      Parameters      :       combcode - pointer to the result code   */
/*                      :       sara     - pointer to upper sara        */
/*                              wannayok - pointer to wannayok          */
/*      Return values   :       NONE                                    */
/* -------------------------------------------------------------------- */
void    cp_combine_fontlq(FONT *combcode, FONT *sara, FONT *wannayok)
{
        int     index;

        /* -- copy wannayok to combcode  */
        memcpy ( combcode, ((char *) wannayok) + 1, CP_LQ_FONTSIZE);

        /* -- clear byte #2 in every column */
        for (index = 0; index < CP_LQCOL; index++)
                combcode->font[index][2] = 0;
        /* -- COMBINE SARA AND COMBCODE --- */
        for (index = 0; index < CP_LQ_FONTSIZE; index++)
                combcode->font[0][index] |= sara->font[0][index];
}

/* -------------------------------------------------------------------- */
/*      Function        :       cp_create_lqcombine                     */
/*      Description     :       create combine code printer table for   */
/*                              easily use in print_lq function         */
/*      Parameters      :       NONE                                    */
/*      Return values   :       pointer to combine code table           */
/* -------------------------------------------------------------------- */
FONT    *cp_create_lqcombine( FONT *fntable)
{
        int      x, y;                   /* index to lq combine code table */
        int      index;                          /* index to address in lq combine */
        static   unsigned char *sara="—”‘’÷◊";    /* sara set     */
        static   unsigned char *wannayok="ËÈÍÎÏ"; /* wannayok     */
        FONT     *fontptr;               /* font pointer  */

        fontptr = (FONT * ) malloc ( CP_LQCOMB_TABSIZE );
        for (x = 0; x < 6;x++) {
            for (y = 0; y < 5; y++) {
                index = 5*x + y;
                cp_combine_fontlq (fontptr + index,
                                   cp_lq_font ( fntable, sara[x] ),
                                   cp_lq_font ( fntable, wannayok[y] ));
            };
        };
        return (fontptr);
}
/* --------------------------------------------------------------------- */
/*      Function        :       cp_loadfont_lqscr                        */
/*      Description     :       load subscript and italic subscript font */
/*      Parameters      :       fname   - font filename                  */
/*      Return value    :       fntptr  - pointer to font table          */
/*      Remark          :       This function allocate the font table    */
/* --------------------------------------------------------------------- */
FONTSCR         *cp_loadfont_lqscr ( fname)
char    *fname;
{
        FONTSCR *p;
        int  fd;

        p = ( FONTSCR * ) malloc (CP_LQSCR_TABSIZE);
        if ( p == NULL )
                execerror ("Insufficient memory\n","");
        fd = open (fname, O_RDONLY| O_BINARY);
        if ( fd < 0 )
                execerror ("Printer font file not found : ", fname);
        read (fd, p, CP_LQSCR_TABSIZE);
        close (fd);
        return (p);
}
/* -------------------------------------------------------------------- */
/*      Function        :       cp_combine_fontlqscr                    */
/*      Description     :       Combine upper sara and wannayok         */
/*      Parameters      :       combcode - pointer to the result code   */
/*                      :       sara     - pointer to upper sara        */
/*                              wannayok - pointer to wannayok          */
/*      Return values   :       NONE                                    */
/* -------------------------------------------------------------------- */
void    cp_combine_fontlqscr(FONTSCR *combcode,FONTSCR *sara,FONTSCR *wannayok)
{
        int     index;

        /* -- clear combcode -- */
        memset ( combcode, 0, CP_LQSCR_FONTSIZE);

        /* -- combine sara and wannayok with shifting up wannayok 6 bits
              and or with sara -- */
        for (index = 0; index < CP_LQCOL; index++) {
                combcode->font[index][0] = 0x0F &
                                           ( ( wannayok->font[index][1]>>3 ) |
                                             ( sara->font[index][0] ) );
                combcode->font[index][1] = ( ( wannayok->font[index][1]<<5 ) |
                                             ( sara->font[index][1] ) );
        };
}

/* -------------------------------------------------------------------- */
/*      Function        :       cp_create_lqscrcombine                  */
/*      Description     :       create combine code printer table for   */
/*                              easily use in print_lq function         */
/*      Parameters      :       NONE                                    */
/*      Return values   :       pointer to combine code table           */
/* -------------------------------------------------------------------- */
FONTSCR         *cp_create_lqscrcombine( FONTSCR *fntable)
{
        int      x, y;                   /* index to lq combine code table */
        static   unsigned char *sara="—”‘’÷◊";    /* sara set     */
        static   unsigned char *wannayok="ËÈÍÎÏ"; /* wannayok     */
        int      index;                          /* index to address in lq combine */
        FONTSCR          *fontptr;               /* font pointer                   */

        fontptr = (FONTSCR * ) malloc ( CP_LQSCRCOMB_TABSIZE );
        for (x = 0; x < 6;x++) {
            for (y = 0; y < 5; y++) {
                index = 5*x + y;
                cp_combine_fontlqscr (fontptr + index,
                                   cp_lqscr_font ( fntable, sara[x] ),
                                   cp_lqscr_font ( fntable, wannayok[y] ));
            };
        };
        return (fontptr);
}
